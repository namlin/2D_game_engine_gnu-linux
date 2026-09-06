#ifndef REGISTRY_H
#define REGISTRY_H

#include <bitset>
#include <deque>
#include <cstdint>
#include <iostream>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "Component.h"
#include "Constants.h"
#include "Entity.h"
#include "Pool.h"
// #include "System.h"

class System;  // Forward declaration;

class Registry {
 public:
  Registry(void);
  ~Registry(void);

  void update(void);

  // Entity Management:
  Entity create_entity(void);
  void delete_entity(Entity entity);

  // Component Management:
  template <typename TComponent, typename... TArgs>
  void add_component(Entity entity, TArgs&&... args);

  template <typename TComponent>
  void remove_component(Entity entity);

  template <typename TComponent>
  bool has_component(Entity entity) const;

  template <typename TComponent>
  TComponent& get_component(Entity entity);

  // System Management:
  template <typename TSystem, typename... TArgs>
  void add_system(TArgs&&... args);

  template <typename TSystem>
  void remove_system(void);

  template <typename TSystem>
  bool has_system(void) const;

  template <typename TSystem>
  IComponent& get_system(void) const;

  // Add and remove entities to systems:
  void add_entity_to_systems(Entity entity);
  void remove_entity_from_systems(Entity entity);

 private:
  // Attributes:
  size_t total_entities = 0;
  std::vector<IPool*> entries;
  std::vector<std::bitset<MAX_COMPONENTS>> entity_component_signatures;
  std::unordered_map<std::type_index, System*> systems;
  std::deque<size_t> free_IDs;

  // Functions:
  std::set<Entity> entities_to_add;
  std::set<Entity> entities_to_remove;
};

// Templates implementations:
template <typename TComponent, typename... TArgs>
void Registry::add_component(Entity entity, TArgs&&... args) {
  const size_t component_id = Component<TComponent>::get_id();
  const size_t entity_id = entity.get_id();

  // Resize the pool container if component_id is out of bounds before accessing
  // entries[component_id]:
  if (component_id >= this->entries.size()) {
    this->entries.resize(component_id + 10, nullptr);
  }

  // Instantiate pool if it doesn't exist yet:
  if (!this->entries[component_id]) {
    Pool<TComponent>* new_component = new Pool<TComponent>();

    if (new_component == nullptr) {
      std::cerr << "[Registry] ERROR: No dynamic memory was allocated for 'new_component' pointer.\n";
      std::exit(EXIT_FAILURE);
    }

    this->entries[component_id] = new_component;
  }

  // Resize the entity signatures container if needed;
  if (entity_id >= this->entity_component_signatures.size()) {
    this->entity_component_signatures.resize(entity_id + 100);
  }

  TComponent component(std::forward<TArgs>(args)...);

  // Cast generic pool to specific component pool type and add component:
  Pool<TComponent>* pool = static_cast<Pool<TComponent>*>(this->entries[component_id]);

  // Assuming your Pool class has a set() or add() method:
  pool->set(entity_id, component);

  // Set the bitmask bit in entity_component_signatures vector:
  this->entity_component_signatures[entity_id].set(component_id);
}

template <typename TComponent>
void Registry::remove_component(Entity entity) {
  const size_t component_id = Component<TComponent>::get_id();
  const size_t entity_id = entity.get_id();

  this->entity_component_signatures[entity_id].set(component_id, false);
}

template <typename TSystem>
bool Registry::has_component(Entity entity) const {
  const size_t component_id = Component<IComponent>::get_id();
  const size_t entity_id = entity.get_id();

  return this->entity_component_signatures[entity_id].test(component_id);
}

template <typename TComponent>
TComponent& Registry::get_component(Entity entity) {
  const size_t component_id = Component<TComponent>::get_id();
  const size_t entity_id = entity.get_id();

  auto* component_pool = static_cast<Pool<TComponent>*>(this->entries[component_id]);

  return component_pool->get(entity_id);
}

// System Management:
template <typename TSystem, typename... TArgs>
void Registry::add_system(TArgs&&... args) {
  TSystem* system = new TSystem(std::forward<TArgs>(args)...);
  this->systems.insert(std::make_pair(std::type_index(typeid(TSystem)), system));
}

template <typename TSystem>
void Registry::remove_system(void) {
  auto system = this->systems.find(std::type_index(typeid(TSystem)));
  this->systems.erase(system);
}

template <typename TSystem>
bool Registry::has_system(void) const {
  return this->systems.find(std::type_index(typeid(TSystem))) != this->systems.end();
}

template <typename TSystem>
IComponent& Registry::get_system(void) const {
  auto system = this->systems.find(std::type_index(typeid(TSystem)));
  return *(static_cast<TSystem>(system->second));
}

#endif  // REGISTRY_H
