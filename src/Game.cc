#include "../include/Game.h"

Game::Game(void) {
  std::cout << "[GAME] Constructor Executing.\n";

  this->asset_manager = new AssetManager();

  if (this->asset_manager == nullptr) {
    std::cerr << "[GAME] ERROR: No dynamic memory was allocated for the asset_manager pointer.\n";
    std::exit(EXIT_FAILURE);
  }

  this->controller_manager = new ControllerManager();

  if (this->controller_manager == nullptr) {
    std::cerr << "[GAME] ERROR: No dynamic memory was allocated for the controller_manager pointer.\n";
    std::exit(EXIT_FAILURE);
  }

  this->event_manager = new EventManager();

  if (this->event_manager == nullptr) {
    std::cerr << "[GAME] ERROR: No dynamic memory was allocated for the event_manager pointer.\n";
    std::exit(EXIT_FAILURE);
  }

  this->registry = new Registry();

  if (this->registry == nullptr) {
    std::cerr << "[GAME] ERROR: No dynamic memory was allocated for the registry pointer.\n";
    std::exit(EXIT_FAILURE);
  }

  this->scene_loader = new SceneLoader();

  if (this->scene_loader == nullptr) {
    std::cerr << "[GAME] ERROR: No dynamic memory was allocated for the scene_loader pointer.\n";
    std::exit(EXIT_FAILURE);
  }
}

Game::~Game(void) {
  std::cout << "[GAME] Destructor Executing.\n";

  delete this->asset_manager;
  delete this->controller_manager;
  delete this->event_manager;
  delete this->registry;
  delete this->scene_loader;
}

Game* Game::get_instance(void) {
  if (!instance) {
    instance = new Game();

    if (instance == nullptr) {
      std::cerr << "[GAME] ERROR: No dynamic memory was allocated for the Game instance.\n";
      std::exit(EXIT_FAILURE);
    }
  }

  return instance;
}

void Game::init(void) {
  // Initialize SDL:
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cerr << "[GAME] ERROR: SDL was not initialized.\n";
    std::exit(EXIT_FAILURE);
    return;
  }

  // Initialize SDL TTF:
  if (TTF_Init() != 0) {
    std::cerr << "[GAME] ERROR: SDL TTF was not initialized.\n";
    std::exit(EXIT_FAILURE);
    return;
  }

  this->window = SDL_CreateWindow(
    this->window_title,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    this->window_width,
    this->window_height,
    SDL_WINDOW_SHOWN
  );

  this->renderer = SDL_CreateRenderer(this->window, -1, 0);

  if (!renderer) {
    std::cerr << "[GAME] ERROR: Renderer was not initialized.\n";
    return;
  }

  this->is_running = true;

  this->rect_1 = {
    (window_width / 2) - 25,  // X position.
    (window_height / 2) - 25,  // Y position.
    50,  // Width.
    50  // Height.
  };
}

void Game::setup(void) {
  // Add systems:
  this->registry->add_system<AnimationSystem>();
  this->registry->add_system<CollisionSystem>();
  this->registry->add_system<DamageSystem>();
  this->registry->add_system<MovementSystem>();
  this->registry->add_system<RenderSystem>();
  this->registry->add_system<ScriptSystem>();

  this->lua.open_libraries(sol::lib::base, sol::lib::math);
  this->registry->get_system<ScriptSystem>().create_lua_binding(this->lua);

  this->scene_loader->load_scene("./assets/lua_scripts/scene_01.lua", this->lua,
                                 *this->asset_manager, *this->controller_manager,
                                 *this->registry, this->renderer);

  /*
  // Map keys:

  // Use the SDL keycodes.
  this->controller_manager->add_action_key("Move Up", SDLK_w);
  this->controller_manager->add_action_key("Move Down", SDLK_s);
  this->controller_manager->add_action_key("Move Left", SDLK_a);
  this->controller_manager->add_action_key("Move Right", SDLK_d);

  // Register assets:
  this->asset_manager->add_texture(this->renderer, "Player", "./assets/Player.png");
  this->asset_manager->add_texture(this->renderer, "enemy_1", "./assets/enemy_1.png");
  this->asset_manager->add_texture(this->renderer, "enemy_2", "./assets/enemy_1.png");

  // ---Create entities---

  // Player:
  Entity player = this->registry->create_entity();
  this->lua.script_file("./assets/lua_scripts/player.lua");

  // player.add_component<AnimationComponent>(1, 10, true);
  player.add_component<CircleColliderComponent>(8, 16, 16);
  player.add_component<RigidBodyComponent>(glm::vec2(0, 0));
  // player.add_component<ScriptComponent>(lua["update"]);
  player.add_component<ScriptComponent>(sol::function(lua["update"]));
  player.add_component<SpriteComponent>("Player", 16, 16, 0, 0);
  player.add_component<TransformComponent>(glm::vec2(400.0, 300.0), glm::vec2(2.0, 2.0), 0.0);

  // Enemy 1:
  Entity enemy_1 = this->registry->create_entity();
  enemy_1.add_component<AnimationComponent>(1, 10, true);
  enemy_1.add_component<CircleColliderComponent>(8, 16, 16);
  enemy_1.add_component<RigidBodyComponent>(glm::vec2(50, 0));
  enemy_1.add_component<SpriteComponent>("enemy_1", 16, 16, 0, 0);
  enemy_1.add_component<TransformComponent>(glm::vec2(200.0, 100.0), glm::vec2(2.0, 2.0), 0.0);

  // Enemy 2:
  Entity enemy_2 = this->registry->create_entity();
  enemy_2.add_component<AnimationComponent>(1, 10, true);
  enemy_2.add_component<CircleColliderComponent>(8, 16, 16);
  enemy_2.add_component<RigidBodyComponent>(glm::vec2(-50, 0));
  enemy_2.add_component<SpriteComponent>("enemy_2", 16, 16, 0, 0);
  enemy_2.add_component<TransformComponent>(glm::vec2(600.0, 100.0), glm::vec2(2.0, 2.0), 0.0);
  */
}

void Game::process_input(void) {
  SDL_Event SDL_event;

  while (SDL_PollEvent(&SDL_event)) {
    switch (SDL_event.type) {
      case SDL_QUIT:
        this->is_running = false;
        break;

      case SDL_KEYDOWN:
        if (SDL_event.key.keysym.sym == SDLK_ESCAPE) {
          this->is_running = false;
          break;
        }

        this->controller_manager->key_down(SDL_event.key.keysym.sym);
        break;

      case SDL_KEYUP:
        this->controller_manager->key_up(SDL_event.key.keysym.sym);
        break;

      case SDL_MOUSEMOTION: {
        int x = 0;
        int y = 0;
        SDL_GetMouseState(&x, &y);
        this->controller_manager->set_mouse_position(x, y);
        break;
      }

      case SDL_MOUSEBUTTONDOWN:
        this->controller_manager->set_mouse_position(SDL_event.button.x, SDL_event.button.y);
        this->controller_manager->set_mouse_button_down(SDL_event.button.button);
            std::cout << "Mouse button: "
              << static_cast<int>(SDL_event.button.button)
              << "\n";  // TEST
        break;

      default:
        break;
    }
  }
}

void Game::update(void) {
  size_t time_to_wait = MILLISECS_PER_FRAME - (SDL_GetTicks()) - this->millisecs_previous_frame;

  // Delay if going faster:
  if (0 < time_to_wait && time_to_wait <= MILLISECS_PER_FRAME) {
    SDL_Delay(time_to_wait);
  }

  // Convert to seconds:
  double delta_time = (SDL_GetTicks() - this->millisecs_previous_frame) / 1000.0;

  // TODO: Add this variable to the Lua state.

  this->millisecs_previous_frame = SDL_GetTicks();

  // Reset subscriptions.
  this->event_manager->reset();
  this->registry->get_system<DamageSystem>().subscribe_to_collision_event(*this->event_manager);

  this->registry->update();

  this->registry->get_system<ScriptSystem>().update(this->lua);
  this->registry->get_system<AnimationSystem>().update();
  this->registry->get_system<CollisionSystem>().update(*this->event_manager);
  this->registry->get_system<MovementSystem>().update(delta_time);
}

void Game::render(void) {
  SDL_SetRenderDrawColor(this->renderer, 225, 225, 24, 225);
  SDL_RenderClear(this->renderer);

  this->registry->get_system<RenderSystem>().Update(this->renderer, *this->asset_manager);

  SDL_SetRenderDrawColor(this->renderer, 225, 98, 245, 225);
  SDL_RenderFillRect(this->renderer, &this->rect_1);

  SDL_RenderPresent(this->renderer);  // Swap the drawing matrix.
}

void Game::run(void) {
  while (this->is_running) {
    this->process_input();
    this->update();
    this->render();
  }
}

void Game::destroy(void) {
  SDL_DestroyRenderer(this->renderer);
  SDL_DestroyWindow(this->window);

  TTF_Quit();
  SDL_Quit();

  // Safely delete the Game singleton instance at the very end:
  delete instance;
  instance = nullptr;
}
