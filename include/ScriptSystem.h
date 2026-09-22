#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include <sol/sol.hpp>

#include "../include/LuaBinding.h"

#include "../include/ScriptComponent.h"
#include "../include/Entity.h"
#include "../include/System.h"

class ScriptSystem : public System {
 public:
  ScriptSystem(void) {
    this->require_component<ScriptComponent>();
  }

  void create_lua_binding(sol::state& lua) {
    // Classes:
    lua.new_usertype<Entity>("Entity");

    // Functions:
    lua.set_function("is_action_activated", is_action_activated);
    lua.set_function("set_velocity", set_velocity);
  }

  void update(sol::state& lua) {
    for (auto entity : this->get_entities()) {
      const auto& script = entity.get_component<ScriptComponent>();

      if (script.update != sol::nil) {
        lua["this"] = entity;
        script.update();
      }
    }
  }
};

#endif  // SCRIPTSYSTEM_H
