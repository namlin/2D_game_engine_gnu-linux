#ifndef ENTITY_H
#define ENTITY_H

#include <cstddef>
#include <utility>

// Forward-declare Registry so Entity can hold a pointer:
class Registry;

class Entity {
  friend class Registry;

 private:
  size_t id;
  Registry* registry = nullptr;

 public:
  Entity(size_t id) : id(id) {}
  size_t get_id(void) const;

  bool operator==(const Entity& other) const { return id == other.id; }
  bool operator!=(const Entity& other) const { return id != other.id; }
  bool operator>(const Entity& other) const { return id > other.id; }
  bool operator<(const Entity& other) const { return id < other.id; }

  template <typename TComponent, typename... TArgs>
  void add_component(TArgs&&... args);

  template <typename TComponent>
  void remove_component(void);

  template <typename TComponent>
  bool has_component(void) const;

  template <typename TComponent>
  TComponent& get_component(void) const;
};

#include "Registry.h"

template <typename TComponent, typename... TArgs>
void Entity::add_component(TArgs&&... args) {
  this->registry->template add_component<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent>
void Entity::remove_component(void) {
  this->registry->template remove_component<TComponent>(*this);
}

template <typename TComponent>
bool Entity::has_component(void) const {
  return this->registry->template has_component<TComponent>(*this);
}

template <typename TComponent>
TComponent& Entity::get_component(void) const {
  return this->registry->template get_component<TComponent>(*this);
}

#endif  // ENTITY_H
