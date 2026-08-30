#include "../include/Registry.h"

Registry::Registry(void) {
  std::cout << "[Registry] Constructor Executing.\n";  // TEST
}

Registry::~Registry(void) {
  std::cout << "[Registry] Destructor Executing.\n";  // TEST
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

// void Registry::add_system(Entity) {}
