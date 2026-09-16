#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <SDL2/SDL.h>

struct AnimationComponent {
  size_t total_frames = 0;
  size_t current_frame = 0;
  size_t frame_speed_rate = 0;
  size_t start_time = 0;
  bool is_loop = false;

  AnimationComponent(size_t total_frames = 1, size_t frame_speed_rate = 1, bool is_loop = true) {
    this->total_frames = total_frames;
    this->current_frame = 1;
    this->frame_speed_rate = frame_speed_rate;
    this->is_loop = is_loop;
    this->start_time = SDL_GetTicks();
  }
};

#endif  // ANIMATIONCOMPONENT_H
