#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <iostream>

// SDL:
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <sol/sol.hpp>

// Managers:
#include "../include/AssetManager.h"
#include "../include/ControllerManager.h"
#include "../include/EventManager.h"
#include "../include/SceneManager.h"

// ECS:
#include "../include/Component.h"
#include "../include/Entity.h"
#include "../include/Registry.h"
#include "../include/System.h"

/*
// Components:
#include "../include/AnimationComponent.h"
#include "../include/CircleColliderComponent.h"
#include "../include/RigidBodyComponent.h"
#include "../include/ScriptComponent.h"
#include "../include/SpriteComponent.h"
#include "../include/TextComponent.h"
#include "../include/TransformComponent.h"
*/

// Systems:
#include "../include/AnimationSystem.h"
#include "../include/CollisionSystem.h"
#include "../include/DamageSystem.h"
#include "../include/MovementSystem.h"
#include "../include/RenderSystem.h"
#include "../include/RenderTextSystem.h"
#include "../include/ScriptSystem.h"
#include "../include/UISystem.h"

// Events:
#include "../include/ClickEvent.h"

const uint8_t FPS = 30;
const uint16_t MILLISECS_PER_FRAME = 1000 / FPS;

class SceneManager;  // Forward declaration.

class Game {
 private:
  // --- Attributes ---
  inline static Game* instance;

  SDL_Window* window = nullptr;
  uint16_t window_width = 800;  // 256
  uint16_t window_height = 800;  // 192
  const char* window_title = "2D Game Engine";

  uint16_t millisecs_previous_frame = 0;

  SDL_Rect rect_1;
  bool is_running = false;

  // --- Singleton Encapsulation ---
  Game(void);
  ~Game(void);

  // Prevent copying:
  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

  // --- Private Methods ---
  void process_input(void);
  void update(void);
  void render(void);
  void run_scene(void);

 public:
  Registry* registry = nullptr;

  AssetManager* asset_manager = nullptr;
  EventManager* event_manager = nullptr;
  ControllerManager* controller_manager = nullptr;
  SceneManager* scene_manager = nullptr;

  SDL_Renderer* renderer = nullptr;

  sol::state lua;

  static Game* get_instance(void);

  void init(void);
  void setup(void);
  void run(void);
  void destroy(void);
};

#endif  // GAME_H
