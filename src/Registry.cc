#include "../include/Registry.h"

Registry::Registry(void) {
  std::cout << "[Registry] Constructor Executing.\n";  // TEST
}

Registry::~Registry(void) {
  std::cout << "[Registry] Destructor Executing.\n";  // TEST

  for (IPool* pool : this->entries) {
    delete pool;  // Free each dynamic Pool instance safely.
  }

  this->entries.clear();
}

Entity Registry::create_entity(void) {
  // TODO: Reuse the ID of destroyed entities.
  size_t id = this->total_entities++;

  if (id >= this->entity_component_signatures.size()) {
    this->entity_component_signatures.resize(id + EXTRA_SIZE);  // TEST
  }

  Entity entity(id);
  this->entities_to_add.insert(entity);

  std::cout << "[Registry] New entity created with id " << id << ".\n";  // TEST

  return entity;
}

void Registry::delete_entity(Entity entity) {
  this->entities_to_remove.insert(entity);
}

void Registry::add_entity_to_systems(Entity entity) {
  const auto id = entity.get_id();
  const std::bitset<MAX_COMPONENTS>& entity_component_signature = this->entity_component_signatures[id];

  for (auto& system_pair : this->systems) {
    const auto& system = system_pair.second;
    const auto& system_component_signature = system->get_component_signature();

    bool match = (entity_component_signature & system_component_signature) == system_component_signature;

    if (match) {
      system->add_entity(entity);
    }
  }
}

void Registry::remove_entity_from_systems(Entity entity) {
  for (auto& system_pair : this->systems) {
    system_pair.second->remove_entity(entity);
  }
}

void Registry::update(void) {
  for (auto entity : this->entities_to_add) {
    this->add_entity_to_systems(entity);
  }

  this->entities_to_add.clear();

  for (auto entity : this->entities_to_remove) {
    this->remove_entity_from_systems(entity);

    // Set the whole bitset to 0:
    this->entity_component_signatures[entity.get_id()].reset();

    // TODO: add an ID to the deque of free ID's.
  }

  this->entities_to_remove.clear();
}

// void Registry::add_system(Entity) {}
