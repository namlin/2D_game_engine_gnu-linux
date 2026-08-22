#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <deque>
#include <cstdint>
#include <iostream>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "../include/Pool.h"

// Forward declaration:
class System;

const size_t MAX_COMPONENTS = 64;

// Signature:
// typedef std::bitset<MAX_COMPONENTS> signature_t;

struct IComponent {
 protected:
  static size_t next_id;
};

template <typename TComponent>
class Component : IComponent {
 public:
  static size_t get_id(void) {
    return next_id++;
  }
};

class Entity {
 private:
  size_t id;

 public:
  Entity(size_t id) : id(id) {};
  size_t get_id(void) const;

  bool operator==(const Entity& other) const {
    return id == other.id;
  }

  bool operator!=(const Entity& other) const {
    return id != other.id;
  }

  bool operator>(const Entity& other) const {
    return id > other.id;
  }

  bool operator<(const Entity& other) const {
    return id < other.id;
  }
};

class Registry {
 public:
  Registry(void);
  ~Registry(void);

  void update();

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

// Abstract class for all the systems.
class System {
 private:
  std::bitset<MAX_COMPONENTS> signature;
  std::vector<Entity> entities;

 public:
  System(void) = default;
  ~System(void) = default;

  void add_entity(Entity entity);
  void remove_entity(Entity entity);

  std::vector<Entity> get_entities(void) const;

  std::bitset<MAX_COMPONENTS>& get_component_signature(void);

  template <typename TComponent>
  void required_component(void);
};

template <typename TComponent>
void System::required_component(void) {
  const size_t component_id = Component<TComponent>::get_id();
  // component_signature.set(component_id);
}

# endif
