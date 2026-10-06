#include "../include/SceneManager.h"

SceneManager::SceneManager(void) {
  std::cout << "[SCENEMANAGER] Constructor Executing.\n";

  this->scene_loader = new SceneLoader();

  if (this->scene_loader == nullptr) {
    std::cerr << "[SCENEMANAGER] ERROR: No dynamic memory was allocated for the scene_loader pointer.\n";
    std::exit(EXIT_FAILURE);
  }
}

SceneManager::~SceneManager(void) {
  std::cout << "[SCENEMANAGER] Destructor Executing.\n";
  delete this->scene_loader;
  this->scene_loader = nullptr;
}

void SceneManager::load_scene_from_script(const std::string& path, sol::state& lua) {
  // Run script safely and handle file errors/syntax issues:
  auto result = lua.script_file(path, sol::script_pass_on_error);

  if (!result.valid()) {
    sol::error err = result;
    std::cerr << "[SceneManager Error] Failed to execute script " << path
              << ": " << err.what() << "\n";
    return;
  }

  // 2. Fetch 'scenes' safely using sol::optional
  sol::optional<sol::table> scenes_opt = lua["scenes"];
  if (!scenes_opt || !scenes_opt->valid()) {
    std::cerr << "[SceneManager Error] Global table 'scenes' not found in "
              << path << "\n";
    return;
  }

  sol::table scenes = scenes_opt.value();
  size_t index = 1;

  while (true) {
    // Fetch subtable by integer index rather than string "index":
    sol::optional<sol::table> scene_opt = scenes[index];

    if (!scene_opt || !scene_opt->valid()) {
      break; // Reached end of array
    }

    sol::table scene = scene_opt.value();

    // Safely extract values with fallback checks:
    std::string name = scene.get_or<std::string>("name", "");
    std::string scene_path = scene.get_or<std::string>("path", "");

    if (!name.empty() && !scene_path.empty()) {
      this->scenes.emplace(name, scene_path);

      // 5. Set default initial scene on first entry (index == 1)
      if (index == 1) {
        this->next_scene = name;
      }

    }

    else {
      std::cerr << "[SceneManager] WARNING: Missing 'name' or 'path' at scenes index "
                << index << "\n";
    }

    index++;
  }
}

void SceneManager::load_scene(void) {
  Game& game = *Game::get_instance();
  std::string scene_path = this->scenes[this->next_scene];
  this->scene_loader->load_scene(scene_path, game.lua, *game.asset_manager,
                       *game.controller_manager, *game.registry, game.renderer);
}

std::string SceneManager::get_next_scene(void) const {
  return this->next_scene;
}

void SceneManager::set_next_scene(const std::string& next_scene) {
  this->next_scene = next_scene;
}

bool SceneManager::get_is_scene_running(void) const {
  return this->is_scene_running;
}

void SceneManager::start_scene(void) {
  this->is_scene_running = true;
}

void SceneManager::stop_scene(void) {
  this->is_scene_running = false;
}
