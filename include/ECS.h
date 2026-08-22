#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <cstdint>
#include <vector>

#include "../include/Pool.h"

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
  //

 private:
  size_t total_entities = 0;
  std::vector<IPool> entries;
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
