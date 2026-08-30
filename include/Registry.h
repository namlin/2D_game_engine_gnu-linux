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

#include "../include/Constants.h"
#include "../include/Component.h"
#include "../include/Entity.h"
#include "../include/Pool.h"
#include "../include/System.h"

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
  void add_system(Entity entity, TArgs&&... args);

  template <typename TSystem>
  void remove_system(Entity entity);

  template <typename TSystem>
  bool has_system(Entity entity) const;

  template <typename TSystem>
  IComponent& get_system(Entity entity);

  // Add and remove entities to systems:
  void add_entity_to_systems(Entity entity);
  void remove_entity_from_systems(Entity entity);

 private:
  size_t total_entities = 0;
  std::vector<IPool*> entries;
  std::vector<std::bitset<MAX_COMPONENTS>> entity_component_signatures;
  std::unordered_map<std::type_index, System*> systems;

  std::set<Entity> entities_to_add;
  std::set<Entity> entities_to_remove;
};

#endif  // REGISTRY_H
