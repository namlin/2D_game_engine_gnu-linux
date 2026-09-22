#ifndef LUABINDING_H
#define LUABINDING_H

#include <string>

#include "../include/Entity.h"

#include "../include/RigidBodyComponent.h"

class Game;

bool is_action_activated(const std::string& action);

// Rigid Body Component:
void set_velocity(Entity entity, float x, float y);

#endif  // LUABINDING_H
