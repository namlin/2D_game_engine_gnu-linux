#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>

#include <string>

struct SpriteComponent {
  std::string texture_id;
  size_t width;
  size_t height;
  SDL_Rect rectangle;

  SpriteComponent(const std::string& texture_id = "", int width = 0,
            int height = 0, int rectangle_x = 0, int rectangle_y = 0) {
    this->texture_id = texture_id;
    this->width = width;
    this->height = height;
    this->rectangle = {rectangle_x, rectangle_y, width, height};
  }
};

#endif  // SPRITECOMPONENT_H
