#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include <SDL2/SDL.h>

// ECS:
#include "../include/System.h"

// Components:
#include "../include/AnimationComponent.h"
#include "../include/SpriteComponent.h"

class AnimationSystem : public System {
 public:
  AnimationSystem(void) {
    this->require_component<AnimationComponent>();
    this->require_component<SpriteComponent>();
  }

  void update(void) {
    for (auto entity : this->get_entities()) {
      auto& animation = entity.get_component<AnimationComponent>();
      auto& sprite = entity.get_component<SpriteComponent>();

      // Convert to seconds dividing by 1000.
      animation.current_frame = ((SDL_GetTicks() - animation.start_time)
      * (animation.frame_speed_rate / 1000)) % animation.total_frames;

      sprite.rectangle.x = animation.current_frame * sprite.width;
    }
  }
};

#endif  // ANIMATIONSYSTEM_H
