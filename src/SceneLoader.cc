#include "../include/SceneLoader.h"

SceneLoader::SceneLoader(void) {
  std::cout << "[SceneLoader] Constructor Executing.\n";
}

SceneLoader::~SceneLoader(void) {
  std::cout << "[SceneLOader] Destructor Executing.\n";
}

void SceneLoader::load_scene(const std::string& scene_path, sol::state& lua,
                  AssetManager& asset_manager,
                  ControllerManager& controller_manager,
                  Registry& registry) {
  // TODO: load_sprites()
  // TODO: load_keys
  // TODO: load_entities
}

void SceneLoader::load_sprites(SDL_Renderer* renderer, const sol::table& sprites, AssetManager& asset_manager) {
  size_t index = 0;

  while (true) {
    sol::optional<sol::table> has_sprite = sprites[index];

    if (has_sprite == sol::nullopt) {
      break;
    }

    sol::table sprite = sprites[index];

    std::string asset_id = sprite["asset_id"];
    std::string file_path = sprite["file_path"];

    asset_manager.add_texture(renderer, asset_id, file_path.c_str());

    index++;
  }
}

void SceneLoader::load_keys(const sol::table& keys, ControllerManager& controller_manager) {}

void SceneLoader::load_entities(sol::state& lua, const sol::table& entities, Registry& registry) {}
