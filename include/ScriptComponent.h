#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include <sol/sol.hpp>

struct ScriptComponent {
  sol::function update;

  ScriptComponent(sol::function update = sol::lua_nil) {
    this->update = update;
  }
};

#endif  // SCRIPTCOMPONENT_H
