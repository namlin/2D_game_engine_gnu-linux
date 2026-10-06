#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <iostream>
#include <map>
#include <string>

#include <sol/sol.hpp>

#include "../include/Game.h"
#include "../include/SceneLoader.h"

class SceneManager {
 private:
  std::map<std::string, std::string> scenes;
  std::string next_scene = "";
  bool is_scene_running = false;
  SceneLoader* scene_loader = nullptr;

 public:
  SceneManager(void);
  ~SceneManager(void);

  void load_scene_from_script(const std::string& path, sol::state& lua);
  void load_scene(void);

  std::string get_next_scene(void) const;
  void set_next_scene(const std::string& next_scene);

  bool get_is_scene_running(void) const;

  void start_scene(void);
  void stop_scene(void);
};

#endif  // SCENEMANAGER_H
