#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include <sol/sol.hpp>

struct ScriptComponent {
  sol::function update;
  sol::function on_click;

  ScriptComponent(sol::function update = sol::lua_nil,
                  sol::function on_click = sol::lua_nil) {
    this->update = update;
    this->on_click = on_click;
  }
};

#endif  // SCRIPTCOMPONENT_H
