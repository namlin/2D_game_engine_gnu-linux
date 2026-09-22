#include "../include/LuaBinding.h"
#include "../include/Game.h"

bool is_action_activated(const std::string& action) {
  return Game::get_instance()->controller_manager->is_action_activated(action);
}
