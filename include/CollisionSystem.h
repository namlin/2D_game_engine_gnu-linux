#ifndef COLLISSIONSYSTEM_H
#define COLLISSIONSYSTEM_H

#include <algorithm>
#include <iostream>  // TEST

#include <glm/glm.hpp>

#include "CircleColliderComponent.h"
#include "CollisionEvent.h"
#include "EventManager.h"
#include "System.h"
#include "TransformComponent.h"

class CollisionSystem : public System {
 public:
  CollisionSystem(void) {
    this->require_component<CircleColliderComponent>();
    this->require_component<TransformComponent>();
  }

  void update(EventManager& event_manager) {
    auto entities = this->get_entities();

    for (auto i = entities.begin(); i != entities.end(); i++) {
      Entity a = *i;

      const auto& a_collider = a.get_component<CircleColliderComponent>();
      const auto& a_transform = a.get_component<TransformComponent>();

      // Start 'j' at 'i + 1' to avoid duplicate pair checks (A-B vs B-A):
      for (auto j = std::next(i); j != entities.end(); j++) {
        Entity b = *j;

        const auto& b_collider = b.get_component<CircleColliderComponent>();
        const auto& b_transform = b.get_component<TransformComponent>();

        glm::vec2 a_center = glm::vec2(
          a_transform.position.x + (a_collider.width / 2.0f) * a_transform.scale.x,
          a_transform.position.y + (a_collider.height / 2.0f) * a_transform.scale.y
        );

        glm::vec2 b_center = glm::vec2(
          b_transform.position.x + (b_collider.width / 2.0f) * b_transform.scale.x,
          b_transform.position.y + (b_collider.height / 2.0f) * b_transform.scale.y
        );

        // Use float to prevent precision loss from integer truncation
        float a_radius = a_collider.radius * std::max(a_transform.scale.x, a_transform.scale.y);
        float b_radius = b_collider.radius * std::max(b_transform.scale.x, b_transform.scale.y);

        // Fixed variable names: a_center and b_center
        bool collision = check_circular_collision(a_radius, b_radius, a_center, b_center);

        if (collision) {
          std::cout << "[CollisionSystem] Collision between " << a.get_id() << " and " << b.get_id() << ".\n";
          event_manager.emit_event<CollisionEvent>(a, b);
        }
      }
    }
  }

  bool check_circular_collision(size_t a_radius, size_t b_radius, glm::vec2 a_position, glm::vec2 b_position) {
    glm::vec2 difference = a_position - b_position;
    double length = glm::sqrt((difference.x * difference.x) + (difference.y * difference.y));

    // If the sum of both radius is equal or bigger than the distance between
    // the centers, a collision ocurred.
    return (a_radius + b_radius) >= length;
  }
};

#endif  // COLLISSIONSYSTEM_H
