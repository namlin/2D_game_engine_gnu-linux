#ifndef ENTITY_H
#define ENTITY_H

#include <cstddef>

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

# endif  // ENTITY_H
