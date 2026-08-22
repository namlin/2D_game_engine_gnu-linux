#include <cstdint>
#include <iostream>

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <sol/sol.hpp>

#include "../include/Game.h"

int main(int argc, char* argv[]) {
  std::cout << "---Game Engine---\n\n";

  Game* game = Game::get_instance();
  game->Init();
  game->Run();
  game->Destroy();

  std::cout << "Ciao!\n\n";

  return 0;
}
