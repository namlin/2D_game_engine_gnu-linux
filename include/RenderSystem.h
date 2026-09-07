#ifndef RENDERSYSTEM_H
#define RENDERSYSTEMT_H

#include <SDL2/SDL.h>

#include "AssetManager.h"
#include "Entity.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

class RenderSystem : public System {
 private:

 public:
  RenderSystem(void) {
    // Define the signature of RenderSystem:
    this->required_component<SpriteComponent>();
    this->required_component<TransformComponent>();
  }

  ~RenderSystem(void);

  void Update(SDL_Renderer* renderer, const AssetManager& asset_manager) {
    for (auto entity : this->get_entities()) {
      const auto sprite = entity.get_component<SpriteComponent>();
      const auto transform = entity.get_component<TransformComponent>();

      SDL_Rect source_rectangle = sprite.rectangle;

      SDL_Rect destination_rectangle = {
        static_cast<int>(transform.position.x),
        static_cast<int>(transform.position.y),
        static_cast<int>(sprite.width * transform.scale.x),
        static_cast<int>(sprite.height * transform.scale.y)
      };

      SDL_RenderCopyEx(
        renderer,
        asset_manager.get_texture(sprite.texture_id),
        &source_rectangle,
        &destination_rectangle,
        transform.rotation,
        NULL,
        SDL_FLIP_NONE
      );
    }
  }
};

#endif  // RENDERSYSTEM_H
