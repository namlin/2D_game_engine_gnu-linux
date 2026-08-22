#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Game {
 private:
  // --- Attributes ---
  inline static Game* instance;

  SDL_Window* window = nullptr;
  uint16_t window_width = 800;
  uint16_t window_height = 600;
  const char* window_title = "Game Engine";

  SDL_Renderer* renderer = nullptr;
  SDL_Rect rect_1;
  bool isRunning = false;

  // --- Singleton Encapsulation ---
  Game(void);
  ~Game(void);

  // Prevent copying:
  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

  // --- Private Methods ---
  void ProcessInput(void);
  void Update(void);
  void Render(void);

 public:
  static Game* get_instance(void);

  void Init(void);
  void Run(void);
  void Destroy(void);
};

#endif  // GAME_H
