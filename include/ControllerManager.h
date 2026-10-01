#ifndef CONTROLLERMANAGER_H
#define CONTROLLERMANAGER_H

#include <SDL2/SDL.h>

#include <iostream>
#include <map>
#include <string>
#include <tuple>

class ControllerManager {
 private:
  std::map<std::string, uint8_t> action_key_name;
  std::map<uint8_t, bool> keys_down;

  std::map<std::string, uint8_t> mouse_button_name;
  std::map<uint8_t, bool> mouse_button_down;

  uint16_t mouse_x_position = 0;
  uint16_t mouse_y_position = 0;

public:
  ControllerManager(void);
  ~ControllerManager(void);

  void clear(void);

  // Keyboard:
  void add_action_key(const std::string& action, size_t key_code);
  void key_down(size_t key_code);
  void key_up(size_t key_code);
  bool is_action_activated(const std::string& action);

  // Mouse:
  void add_mouse_button(const std::string& name, uint8_t code);
  void set_mouse_button_down(uint8_t code);
  void set_mouse_button_up(uint8_t code);
  bool is_mouse_button_down(const std::string& name) const;

  void set_mouse_position(uint8_t x, uint8_t y);
  std::tuple<uint8_t, uint8_t> get_mouse_position(void) const;
};

#endif  // CONTROLLER_MANAGER
