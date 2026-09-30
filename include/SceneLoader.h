#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <sol/sol.hpp>

#include <iostream>
#include <string>

#include "../include/Registry.h"

// Managers:
#include "../include/AssetManager.h"
#include "../include/ControllerManager.h"

// Components:
#include "../include/AnimationComponent.h"
#include "../include/CircleColliderComponent.h"
#include "../include/RigidBodyComponent.h"
#include "../include/ScriptComponent.h"
#include "../include/SpriteComponent.h"
#include "../include/TransformComponent.h"

class SceneLoader {
 private:
  void load_sprites(SDL_Renderer* renderer, const sol::table& sprites, AssetManager& asset_manager);
  void load_keys(const sol::table& keys, ControllerManager& controller_manager);
  void load_entities(sol::state& lua, const sol::table& entities, Registry& registry);

 public:
  SceneLoader(void);
  ~SceneLoader(void);

  void load_scene(const std::string& scene_path, sol::state& lua,
                  AssetManager& asset_manager,
                  ControllerManager& controller_manager,
                  Registry& registry);
};

#endif  // SCENELOADER_H
