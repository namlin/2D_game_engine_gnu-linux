#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include <iostream>  // TEST

#include "../include/CollisionEvent.h"
#include "../include/CircleColliderComponent.h"
#include "../include/EventManager.h"
#include "../include/System.h"

class DamageSystem : public System {
 public:
  DamageSystem(void) {
    this->require_component<CircleColliderComponent>();
  }

  void subscribe_to_collision_event(EventManager& event_manager) {
    event_manager->subscribe_to_event<CollisionEvent, DamageSystem>(this, &DamageSystem::on_collision);
  }

  void on_collision(CollisionEvent& e) {
    std::cout << "[DamageSystem] Collision of the entity " << e.a.get_id() << " and " << e.b.get_id() << ".\n";  // TEST
    e.a.delete_entity();
    e.b.delete_entity();
  }
};

#endif  // DAMAGESYSTEM_H
