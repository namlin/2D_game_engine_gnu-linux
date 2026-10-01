#include "../include/ControllerManager.h"

ControllerManager::ControllerManager(void) {
  std::cout << "[ControllerManager] Constructor Executing.\n";
}

ControllerManager::~ControllerManager(void) {
  std::cout << "[ControllerManager] Destructor Executing.\n";
}

void ControllerManager::clear(void) {
  this->action_key_name.clear();
  this->keys_down.clear();
}

// Keyboard:
void ControllerManager::add_action_key(const std::string& action, size_t key_code) {
  this->action_key_name.emplace(action, key_code);
  this->keys_down.emplace(key_code, false);
}

void ControllerManager::key_down(size_t key_code) {
  auto it = this->keys_down.find(key_code);

  if (it != this->keys_down.end()) {
    this->keys_down[key_code] = true;
  }
}

void ControllerManager::key_up(size_t key_code) {
  auto it = this->keys_down.find(key_code);

  if (it != this->keys_down.end()) {
    this->keys_down[key_code] = false;
  }
}

bool ControllerManager::is_action_activated(const std::string& action) {
  auto it = this->action_key_name.find(action);

  if (it != this->action_key_name.end()) {
    size_t key_code = this->action_key_name[action];

    return this->keys_down[key_code];
  }

  return false;
}

// Mouse:
void ControllerManager::add_mouse_button(const std::string& name, uint8_t code) {
  this->mouse_button_name.emplace(name, code);
  this->mouse_button_down.emplace(code, false);
}

void ControllerManager::set_mouse_button_down(uint8_t code) {
  auto it = this->mouse_button_down.find(code);

  if (it != this->mouse_button_down.end()) {
    this->mouse_button_down[code] = true;
  }
}

void ControllerManager::set_mouse_button_up(uint8_t code) {
  auto it = this->mouse_button_down.find(code);

  if (it != this->mouse_button_down.end()) {
    this->mouse_button_down[code] = false;
  }
}

bool ControllerManager::is_mouse_button_down(const std::string& name) const {
  // Lookup the button code from the name string safely:
  auto name_it = this->mouse_button_name.find(name);

  if (name_it == this->mouse_button_name.end()) {
    return false;
  }

  uint8_t code = name_it->second;

  // Lookup whether that code is pressed:
  auto btn_it = this->mouse_button_down.find(code);
  if (btn_it != this->mouse_button_down.end()) {
    return btn_it->second;
  }

  return false;
}

void ControllerManager::set_mouse_position(uint8_t x, uint8_t y) {
  this->mouse_x_position = x;
  this->mouse_x_position = y;
}

std::tuple<uint8_t, uint8_t> ControllerManager::get_mouse_position(void) const {
  return {this->mouse_x_position, this->mouse_y_position};
}
