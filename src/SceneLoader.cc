#include "../include/SceneLoader.h"

SceneLoader::SceneLoader(void) {
  std::cout << "[SceneLoader] Constructor Executing.\n";
}

SceneLoader::~SceneLoader(void) {
  std::cout << "[SceneLoader] Destructor Executing.\n";
}

void SceneLoader::load_scene(const std::string& scene_path, sol::state& lua,
                          AssetManager& asset_manager,
                          ControllerManager& controller_manager,
                          Registry& registry) {
}

void SceneLoader::load_sprites(SDL_Renderer* renderer, const sol::table& sprites, AssetManager& asset_manager) {
  size_t index = 0;

  while (true) {
    sol::optional has_sprite = sprites[index];

    if (has_sprite == sol::nullopt) {
      break;
    }

    sol::table sprite = sprites[index];

    std::string asset_id = sprite["id"];
    std::string file_path = sprite["file_path"];

    asset_manager.add_texture(renderer, asset_id, file_path.c_str());

    index++;
  }
}

void SceneLoader::load_keys(const sol::table& keys, ControllerManager& controller_manager) {
  size_t index = 0;

  while (true) {
    sol::optional has_key = keys[index];

    if (has_key == sol::nullopt) {
      break;
    }

    sol::table key_entry = keys[index];
    std::string key_name = key_entry["name"];
    size_t key_code = key_entry["key"];

    controller_manager.add_action_key(key_name, key_code);

    index++;
  }
}

void SceneLoader::load_entities(sol::state& lua, const sol::table& entities, Registry& registry) {
  size_t index = 0;

  while (true) {
    sol::optional has_entity = entities[index];

    if (has_entity == sol::nullopt) {
      break;
    }

    sol::table entity = entities[index];

    Entity new_entity = registry.create_entity();

    sol::optional has_components = entity["components"];

    if (has_components != sol::nullopt) {
      sol::table components = entity["components"];

      // AnimationComponent
      // TODO: Implementation

      // Circle Collider:
      sol::optional has_circle_collider_component = components["circle_collider"];

      if (has_circle_collider_component != sol::nullopt) {
        new_entity.add_component<CircleColliderComponent>(
          components["circle_collider"]["radius"],
          components["circle_collider"]["width"],
          components["circle_collider"]["height"]
        );
      }

      // RigidBodyComponent:
      sol::optional has_rigid_body_component = components["rigid_body"];

      if (has_rigid_body_component != sol::nullopt) {
        new_entity.add_component<RigidBodyComponent>(
          glm::vec2(
            components["rigid_body"]["velocity"]["x"],
            components["rigid_body"]["velocity"]["y"]
          )
        );
      }

      // ScriptComponent:
      sol::optional has_script_component = components["script"];

      if (has_script_component != sol::nullopt) {
        lua["update"] = sol::nil;

        std::string path = components["script"]["path"];

        // Safely attempt to run the script file:
        sol::load_result script_res = lua.load_file(path);

        if (script_res.valid()) {
          script_res();
          sol::optional has_update = lua["update"];
          sol::function update = sol::nil;

          if (has_update != sol::nullopt) {
            update = lua["update"];
          }

          new_entity.add_component<ScriptComponent>(update);
        }

        else {
          sol::error err = script_res;
          std::cerr << "[SceneLoader] Could not load script " << path << ": " << err.what() << "\n";
        }
      }

      // SpriteComponent:
      sol::optional has_sprite_component = components["sprite"];

      if (has_sprite_component != sol::nullopt) {
        new_entity.add_component<SpriteComponent>(
          components["sprite"]["asset_id"],
          components["sprite"]["width"],
          components["sprite"]["height"],
          components["sprite"]["source_rectangle"]["x"],
          components["sprite"]["source_rectangle"]["y"]
        );
      }

      // TransformComponent:
      sol::optional has_transform_component = components["transform"];

      if (has_transform_component != sol::nullopt) {
        new_entity.add_component<TransformComponent>(
          glm::vec2(
            components["transform"]["position"]["x"],
            components["transform"]["position"]["y"]
          ),

          glm::vec2(
            components["transform"]["scale"]["x"],
            components["transform"]["scale"]["y"]
          ),

          components["transform"]["rotation"]
        );
      }
    }

    index++;
  }
}
