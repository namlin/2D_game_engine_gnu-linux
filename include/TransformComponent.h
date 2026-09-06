#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>

struct TransformComponent {
  glm::vec2 position = glm::vec2(0.0, 0.0);
  glm::vec2 scale = glm::vec2(0.0, 0.0);
  double rotation = 0.0;

  TransformComponent(glm::vec2 pos = glm::vec2(0.0f),
                       glm::vec2 scl = glm::vec2(1.0f),
                       double rot = 0.0)
        : position(pos), scale(scl), rotation(rot) {}
};

#endif  // TRANSFORMCOMPONENT_H
