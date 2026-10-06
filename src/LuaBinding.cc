#include "../include/LuaBinding.h"
#include "../include/Game.h"

bool is_action_activated(const std::string& action) {
  return Game::get_instance()->controller_manager->is_action_activated(action);
}

void set_velocity(Entity entity, float x, float y) {
  auto& rigid_body = entity.get_component<RigidBodyComponent>();
  rigid_body.velocity.x = x;
  rigid_body.velocity.y = y;
}

void go_to_scene(const std::string name) {
  Game::get_instance()->scene_manager->set_next_scene(name);
  Game::get_instance()->scene_manager->stop_scene();
}
