#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include <sol/sol.hpp>

#include "../ScriptComponent.hpp"
#include "../include/ECS"  // TEST

class ScriptSystem {
 public:
  ScriptSystem(void) {
    this->require_component<ScriptComponent>();
  }

  void update(sol::state& lua) {
    for (auto entity : this->get_entities()) {
      const auto& script = entity.get_component<ScriptComponent>();

      if (script.update != sol::nil) {
        script.update();
      }
    }
  }
};

#endif  // SCRIPTSYSTEM_H
