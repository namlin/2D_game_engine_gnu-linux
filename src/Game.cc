#include "../include/Game.h"

Game::Game(void) {
  std::cout << "[GAME] Constructor Executing.\n";

  this->asset_manager = new AssetManager();

  if (this->asset_manager == nullptr) {
    std::cerr << "[GAME] ERROR: No dynamic memory was allocated for the asset_manager pointer.\n";
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
  delete this->registry;
}

Game* Game::get_instance(void) {
  if (!instance) {
    instance = new Game();

    if (instance == nullptr) {
      std::cerr << "[GAME] ERROR: No dynamic memory was allocated for Game instance.\n";
      std::exit(EXIT_FAILURE);
    }
  }

  return instance;
}

void Game::Init(void) {
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

  this->isRunning = true;

  this->rect_1 = {
    (window_width / 2) - 25,  // X position.
    (window_height / 2) - 25,  // Y position.
    50,  // Width.
    50  // Height.
  };
}

void Game::Setup(void) {
  // Add systems:
  this->registry->add_system<CollisionSystem>();
  this->registry->add_system<RenderSystem>();
  this->registry->add_system<MovementSystem>();

  // Register assets:
  this->asset_manager->add_texture(this->renderer, "enemy_1", "./assets/enemy_1.png");
  this->asset_manager->add_texture(this->renderer, "enemy_2", "./assets/enemy_1.png");

  // ---Create entities---

  // Enemy 1:
  Entity enemy_1 = this->registry->create_entity();
  enemy_1.add_component<CircleColliderComponent>(8, 16, 16);
  enemy_1.add_component<RigidBodyComponent>(glm::vec2(50, 0));
  enemy_1.add_component<SpriteComponent>("enemy_1", 16, 16, 0, 0);
  enemy_1.add_component<TransformComponent>(glm::vec2(200.0, 100.0), glm::vec2(2.0, 2.0), 0.0);

  // Enemy 2:
  Entity enemy_2 = this->registry->create_entity();
  enemy_2.add_component<CircleColliderComponent>(8, 16, 16);
  enemy_2.add_component<RigidBodyComponent>(glm::vec2(-50, 0));
  enemy_2.add_component<SpriteComponent>("enemy_2", 16, 16, 0, 0);
  enemy_2.add_component<TransformComponent>(glm::vec2(600.0, 100.0), glm::vec2(2.0, 2.0), 0.0);
}

void Game::ProcessInput(void) {
  SDL_Event SDL_event;

  while (SDL_PollEvent(&SDL_event)) {
    switch (SDL_event.type) {
      case SDL_QUIT:
        this->isRunning = false;
        break;

      case SDL_KEYDOWN:
        if (SDL_event.key.keysym.sym == SDLK_ESCAPE) {
          this->isRunning = false;
        }

        break;

      default:
        break;
    }
  }
}

void Game::Update(void) {
  size_t time_to_wait = MILLISECS_PER_FRAME - (SDL_GetTicks()) - this->millisecs_previous_frame;

  // Delay if going faster:
  if (0 < time_to_wait && time_to_wait <= MILLISECS_PER_FRAME) {
    SDL_Delay(time_to_wait);
  }

  // Convert to seconds:
  double delta_time = (SDL_GetTicks() - this->millisecs_previous_frame) / 1000.0;

  // TODO: Add this variable to the Lua state.

  this->millisecs_previous_frame = SDL_GetTicks();

  this->registry->update();

  this->registry->get_system<MovementSystem>().update(delta_time);
  this->registry->get_system<CollisionSystem>().update();
}

void Game::Render(void) {
  SDL_SetRenderDrawColor(this->renderer, 225, 225, 24, 225);
  SDL_RenderClear(this->renderer);

  this->registry->get_system<RenderSystem>().Update(this->renderer, *this->asset_manager);

  SDL_SetRenderDrawColor(this->renderer, 225, 98, 245, 225);
  SDL_RenderFillRect(this->renderer, &this->rect_1);

  SDL_RenderPresent(this->renderer);  // Swap the drawing matrix.
}

void Game::Run(void) {
  while (this->isRunning) {
    this->ProcessInput();
    this->Update();
    this->Render();
  }
}

void Game::Destroy(void) {
  SDL_DestroyRenderer(this->renderer);
  SDL_DestroyWindow(this->window);

  TTF_Quit();
  SDL_Quit();

  // Safely delete the Game singleton instance at the very end:
  delete instance;
  instance = nullptr;
}
