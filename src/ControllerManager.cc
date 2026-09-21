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
