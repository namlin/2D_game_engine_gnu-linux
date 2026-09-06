#include "../include/Game.h"

Game::Game(void) {
  std::cout << "[GAME] Constructor Executing.\n";

  this->registry = new Registry();

  if (this->registry == nullptr) {
    std::cerr << "[GAME] ERROR: No dynamic memory was allocated for the registry pointer.\n";
    std::exit(EXIT_FAILURE);
  }
}

Game::~Game(void) {
  std::cout << "[GAME] Destructor Executing.\n";
  // this->registry.reset();
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

  // Initialize SDL TFT:
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
  Entity entity = this->registry->create_entity();
  entity.add_component<TransformComponent>(glm::vec2(100.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
}

void Game::ProcessInput(void) {
  SDL_Event SDL_event;

  while (SDL_PollEvent(&SDL_event)) {
    switch (SDL_event.type) {
      case SDL_QUIT:
        isRunning = false;
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

void Game::Update(void) {}

void Game::Render(void) {
  SDL_SetRenderDrawColor(this->renderer, 225, 225, 24, 225);
  SDL_RenderClear(this->renderer);
  SDL_SetRenderDrawColor(this->renderer, 225, 98, 245, 225);
  SDL_RenderFillRect(this->renderer, &this->rect_1);
  SDL_RenderPresent(this->renderer);  // Swap the drawing matrix.
}

void Game::Run(void) {
  while (this->isRunning) {
    this->ProcessInput();
    // this->Update();
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
