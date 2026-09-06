#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstddef>

struct IComponent {
 protected:
  inline static size_t next_id = 0;
};

template <typename TComponent>
class Component : IComponent {
 public:
  static size_t get_id(void) {
    return next_id++;
  }
};

#endif  // COMPONENT_H
