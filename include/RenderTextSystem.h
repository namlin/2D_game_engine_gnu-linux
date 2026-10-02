#ifndef RENDERTEXTSYSTEM_H
#define RENDERTEXTSYSTEM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../include/System.h"

#include "../include/AssetManager.h"

// Components:
#include "../include/TextComponent.h"
#include "../include/TransformComponent.h"

class RenderTextSystem : public System {
 public:
  RenderTextSystem(void) {
    this->require_component<TextComponent>();
    this->require_component<TransformComponent>();
  }

  ~RenderTextSystem(void) {}

  void update(SDL_Renderer* renderer, AssetManager* asset_manager) {
    if (asset_manager == nullptr) {
      return;
    }

    for (auto entity : this->get_entities()) {
      auto& text = entity.get_component<TextComponent>();
      auto& transform = entity.get_component<TransformComponent>();

      TTF_Font* font = asset_manager->get_font(text.font_id);
      if (font == nullptr) {
        continue;
      }

      SDL_Surface* surface = TTF_RenderText_Blended(
        font,
        text.text.c_str(),
        text.color
      );

      if (surface == nullptr) {
        continue;
      }

      text.width = surface->w;
      text.height = surface->h;

      SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_FreeSurface(surface);

      if (texture != nullptr) {
        SDL_Rect destiny_rectangle = {
          static_cast<int>(transform.position.x),
          static_cast<int>(transform.position.y),
          static_cast<int>(text.width * transform.scale.x),
          static_cast<int>(text.height * transform.scale.y),
        };

        SDL_RenderCopy(renderer, texture, NULL, &destiny_rectangle);
        SDL_DestroyTexture(texture);
      }
    }
  }
};

#endif  // RENDERTEXTSYSTEM_H
