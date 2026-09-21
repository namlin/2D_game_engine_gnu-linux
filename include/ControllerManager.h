#ifndef CONTROLLERMANAGER_H
#define CONTROLLERMANAGER_H

#include <SDL2/SDL.h>

#include <iostream>
#include <map>
#include <string>

class ControllerManager {
 private:
  std::map<std::string, int> action_key_name;
  std::map<int, bool> keys_down;

public:
  ControllerManager(void);
  ~ControllerManager(void);

  void clear(void);

  // Keyboard:
  void add_action_key(const std::string& action, size_t key_code);
  void key_down(size_t key_code);
  void key_up(size_t key_code);
  bool is_action_activated(const std::string& action);
};

#endif  // CONTROLLER_MANAGER
