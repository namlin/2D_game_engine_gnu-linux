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

  this->scene_manager = new SceneManager();

  if (this->scene_manager == nullptr) {
    std::cerr << "[GAME] ERROR: No dynamic memory was allocated for the scene_manager pointer.\n";
    std::exit(EXIT_FAILURE);
  }

  this->registry = new Registry();

  if (this->registry == nullptr) {
    std::cerr << "[GAME] ERROR: No dynamic memory was allocated for the registry pointer.\n";
    std::exit(EXIT_FAILURE);
  }
}

Game::~Game(void) {
  std::cout << "[GAME] Destructor Executing.\n";

  delete this->asset_manager;
  delete this->controller_manager;
  delete this->event_manager;
  delete this->scene_manager;

  delete this->registry;
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
  this->registry->add_system<RenderTextSystem>();
  this->registry->add_system<ScriptSystem>();
  this->registry->add_system<UISystem>();

  this->scene_manager->load_scene_from_script("./assets/lua_scripts/scenes.lua", this->lua);

  this->lua.open_libraries(sol::lib::base, sol::lib::math);
  this->registry->get_system<ScriptSystem>().create_lua_binding(this->lua);
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
          this->scene_manager->stop_scene();
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
        this->event_manager->emit_event<ClickEvent>(SDL_event.button.button, SDL_event.button.x, SDL_event.button.y);
            std::cout << "Mouse button: "
              << static_cast<int>(SDL_event.button.button)
              << "\n";  // TEST
        break;

      case SDL_MOUSEBUTTONUP:
        this->controller_manager->set_mouse_position(SDL_event.button.x, SDL_event.button.y);
        this->controller_manager->set_mouse_button_up(SDL_event.button.button);
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
  this->registry->get_system<UISystem>().subscribe_to_click_event(*this->event_manager);

  this->registry->update();

  this->registry->get_system<ScriptSystem>().update(this->lua);
  this->registry->get_system<AnimationSystem>().update();
  this->registry->get_system<CollisionSystem>().update(*this->event_manager);
  this->registry->get_system<MovementSystem>().update(delta_time);
}

void Game::render(void) {
  SDL_SetRenderDrawColor(this->renderer, 225, 225, 24, 225);
  SDL_RenderClear(this->renderer);

  this->registry->get_system<RenderSystem>().update(this->renderer, *this->asset_manager);
  this->registry->get_system<RenderTextSystem>().update(this->renderer, this->asset_manager);

  SDL_SetRenderDrawColor(this->renderer, 225, 98, 245, 225);
  SDL_RenderFillRect(this->renderer, &this->rect_1);

  SDL_RenderPresent(this->renderer);  // Swap the drawing matrix.
}

void Game::run_scene(void) {
  this->scene_manager->load_scene();

  while (scene_manager->get_is_scene_running()) {
    this->process_input();
    this->update();
    this->render();
  }

  this->asset_manager->clear_assets();
  this->registry->clear_entities();
}

void Game::run(void) {
  while (this->is_running) {
    this->scene_manager->start_scene();
    this->run_scene();
  }
}

void Game::destroy(void) {
  if (this->asset_manager != nullptr) {
    this->asset_manager->clear_assets();
  }

  if (this->renderer != nullptr) {
    SDL_DestroyRenderer(this->renderer);
    this->renderer = nullptr;
  }

  if (this->window != nullptr) {
    SDL_DestroyWindow(this->window);
    this->window = nullptr;
  }

  TTF_Quit();
  SDL_Quit();

  // Safely delete the Game singleton instance at the very end:
  delete instance;
  instance = nullptr;
}
