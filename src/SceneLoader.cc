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
                             Registry& registry, SDL_Renderer* renderer) {
  sol::load_result script_result = lua.load_file(scene_path);

  if (!script_result.valid()) {
    sol::error error = script_result;
    std::string error_message = error.what();
    std::cerr << "[SceneLoader] ERROR loading scene file " << scene_path << ": " << error_message << "\n";
    return;
  }

  // Execute the loaded script to populate global variables in Lua:
  script_result();

  sol::optional<sol::table> has_scene = lua["scene"];
  if (!has_scene.has_value()) {
    std::cerr << "[SceneLoader] ERROR: 'scene' table is missing in " << scene_path << "\n";
    return;
  }

  sol::table scene = has_scene.value();

  // Load Sprites:
  sol::optional<sol::table> has_sprites = scene["sprites"];

  if (has_sprites.has_value()) {
    this->load_sprites(renderer, has_sprites.value(), asset_manager);
  }

  // Load Fonts:
  sol::optional<sol::table> has_fonts = scene["fonts"];

  if (has_fonts.has_value()) {
    this->load_fonts(has_fonts.value(), asset_manager);
  }

  // Load Keys:
  sol::optional<sol::table> has_keys = scene["keys"];

  if (has_keys.has_value()) {
    this->load_keys(has_keys.value(), controller_manager);
  }

  // Load Buttons:
  sol::optional<sol::table> has_buttons = scene["buttons"];

  if (has_buttons.has_value()) {
    this->load_buttons(has_buttons.value(), controller_manager);
  }

  // Load Entities:
  sol::optional<sol::table> has_entities = scene["entities"];

  if (has_entities.has_value()) {
    this->load_entities(lua, has_entities.value(), registry);
  }
}

void SceneLoader::load_sprites(SDL_Renderer* renderer, const sol::table& sprites, AssetManager& asset_manager) {
  for (const auto& pair : sprites) {
    if (pair.second.is<sol::table>()) {
      sol::table sprite = pair.second.as<sol::table>();

      std::string asset_id = sprite["id"].get_or(std::string(""));
      std::string file_path = sprite["file_path"].get_or(std::string(""));

      if (!asset_id.empty() && !file_path.empty()) {
        asset_manager.add_texture(renderer, asset_id, file_path.c_str());
      }
    }
  }
}

void SceneLoader::load_keys(const sol::table& keys, ControllerManager& controller_manager) {
  for (const auto& pair : keys) {
    if (pair.second.is<sol::table>()) {
      sol::table key_entry = pair.second.as<sol::table>();

      std::string key_name = key_entry["name"].get_or(std::string(""));
      size_t key_code = key_entry["key"].get_or(0);

      if (!key_name.empty()) {
        controller_manager.add_action_key(key_name, key_code);
      }
    }
  }
}

void SceneLoader::load_entities(sol::state& lua, const sol::table& entities, Registry& registry) {
  for (const auto& pair : entities) {
    if (!pair.second.is<sol::table>()) {
      continue;
    }

    sol::table entity = pair.second.as<sol::table>();
    Entity new_entity = registry.create_entity();

    sol::optional<sol::table> has_components = entity["components"];

    if (!has_components.has_value()) {
      continue;
    }

    sol::table components = has_components.value();

    // CircleColliderComponent:
    sol::optional<sol::table> has_circle = components["circle_collider"];

    if (has_circle.has_value()) {
      sol::table circle = has_circle.value();
      new_entity.add_component<CircleColliderComponent>(
        circle["radius"].get_or(0.0f),
        circle["width"].get_or(0.0f),
        circle["height"].get_or(0.0f)
      );
    }

    // ClickableComponent:
    sol::optional<sol::table> has_clickable = components["clickable"];

    if (has_clickable.has_value()) {
      sol::table clickable = has_clickable.value();
      new_entity.add_component<ClickableComponent>(
      );
    }

    // RigidBodyComponent:
    sol::optional<sol::table> has_rigid = components["rigid_body"];

    if (has_rigid.has_value()) {
      sol::table rigid = has_rigid.value();
      sol::optional<sol::table> has_vel = rigid["velocity"];

      float vx = 0.0f, vy = 0.0f;

      if (has_vel.has_value()) {
        sol::table vel = has_vel.value();
        vx = vel["x"].get_or(0.0f);
        vy = vel["y"].get_or(0.0f);
      }

      new_entity.add_component<RigidBodyComponent>(glm::vec2(vx, vy));
    }

    // ScriptComponent:
    sol::optional<sol::table> has_script = components["script"];

    if (has_script.has_value()) {
      sol::table script_tbl = has_script.value();
      lua["on_click"] = sol::nil;
      lua["update"] = sol::nil;

      std::string path = script_tbl["path"].get_or(std::string(""));

      if (!path.empty()) {
        sol::load_result script_res = lua.load_file(path);

        if (script_res.valid()) {
          script_res();
          sol::optional<sol::function> has_update = lua["update"];
          sol::function update = has_update.value_or(sol::nil);

          sol::optional<sol::function> has_on_click = lua["on_click"];
          sol::function on_click = has_on_click.value_or(sol::nil);

          new_entity.add_component<ScriptComponent>(update, on_click);
        }

        else {
          sol::error err = script_res;
          std::cerr << "[SceneLoader] ERROR: Could not load script " << path << ": " << err.what() << "\n";
        }
      }
    }

    // SpriteComponent
    sol::optional<sol::table> has_sprite = components["sprite"];

    if (has_sprite.has_value()) {
      sol::table sprite_comp = has_sprite.value();
      sol::optional<sol::table> has_src = sprite_comp["source_rectangle"];

      int src_x = 0, src_y = 0;

      if (has_src.has_value()) {
        sol::table src_rect = has_src.value();
        src_x = src_rect["x"].get_or(0);
        src_y = src_rect["y"].get_or(0);
      }

      new_entity.add_component<SpriteComponent>(
        sprite_comp["asset_id"].get_or(std::string("")),
        sprite_comp["width"].get_or(0),
        sprite_comp["height"].get_or(0),
        src_x,
        src_y
      );
    }

    // TextComponent:
    sol::optional<sol::table> has_text = components["text"];

    if (has_text.has_value()) {
      sol::table text_tbl = has_text.value();
      new_entity.add_component<TextComponent>(
        text_tbl["text"].get_or(std::string("")),
        text_tbl["id"].get_or(std::string("")),
        text_tbl["r"].get_or(uint8_t{0}),
        text_tbl["g"].get_or(uint8_t{0}),
        text_tbl["b"].get_or(uint8_t{0}),
        text_tbl["a"].get_or(uint8_t{255})
      );
    }

    // TransformComponent:
    sol::optional<sol::table> has_transform = components["transform"];

    if (has_transform.has_value()) {
      sol::table transform = has_transform.value();

      sol::optional<sol::table> has_pos = transform["position"];
      sol::optional<sol::table> has_scale = transform["scale"];

      float px = 0.0f, py = 0.0f;
      float sx = 1.0f, sy = 1.0f;

      if (has_pos.has_value()) {
        sol::table pos = has_pos.value();
        px = pos["x"].get_or(0.0f);
        py = pos["y"].get_or(0.0f);
      }

      if (has_scale.has_value()) {
        sol::table scale = has_scale.value();
        sx = scale["x"].get_or(1.0f);
        sy = scale["y"].get_or(1.0f);
      }

      new_entity.add_component<TransformComponent>(
        glm::vec2(px, py),
        glm::vec2(sx, sy),
        transform["rotation"].get_or(0.0f)
      );
    }
  }
}

void SceneLoader::load_fonts(const sol::table& fonts, AssetManager& asset_manager) {
  for (const auto& pair : fonts) {
    if (pair.second.is<sol::table>()) {
      sol::table font = pair.second.as<sol::table>();

      std::string id = font["id"].get_or(std::string(""));
      std::string file_path = font["file_path"].get_or(std::string(""));
      size_t size = font["size"].get_or(size_t{0});

      if (!id.empty() && !file_path.empty() && size > 0) {
        asset_manager.add_font(id, file_path, size);
      }
    }
  }
}

void SceneLoader::load_buttons(const sol::table& buttons,
                               ControllerManager& controller_manager) {
  for (const auto& pair : buttons) {
    if (pair.second.is<sol::table>()) {
      sol::table button = pair.second.as<sol::table>();

      std::string name = button["name"].get_or(std::string(""));
      uint8_t code = button["code"].get_or(uint8_t{0});

      if (!name.empty()) {
        controller_manager.add_mouse_button(name, code);
      }
    }
  }
}
