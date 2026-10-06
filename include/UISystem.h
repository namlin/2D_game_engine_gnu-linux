#ifndef UISYSTEM_H
#define UISYSTEM_H

#include <SDL2/SDL.h>

#include <iostream>  // TEST
#include <string>

// ECS:
#include "../include/System.h"

// Components:
#include "../include/ClickableComponent.h"
#include "../include/TextComponent.h"
#include "../include/TransformComponent.h"

// Events:
#include "../include/ClickEvent.h"

// Managers:
#include "../include/EventManager.h"

class UISystem : public System {
 public:
  UISystem(void) {
    this->require_component<ClickableComponent>();
    this->require_component<TextComponent>();
    this->require_component<TransformComponent>();
  }

  void subscribe_to_click_event(EventManager& event_manager) {
    event_manager.subscribe_to_event<ClickEvent, UISystem>(this,
                                                           &UISystem::on_click_event);
  }

  void on_click_event(ClickEvent& click_event) {
    for (auto entity : this->get_entities()) {
      const auto& text = entity.get_component<TextComponent>();
      const auto& transform = entity.get_component<TransformComponent>();

      if (transform.position.x < click_event.x_position
          && click_event.x_position < transform.position.x + text.width
          && transform.position.y < click_event.y_position
          && click_event.y_position < transform.position.y + text.height) {

        if (entity.has_component<ScriptComponent>()) {
          const auto& script = entity.get_component<ScriptComponent>();

          if (script.on_click != sol::nil) {
            script.on_click();
          }
        }
      }
    }
  }
};

#endif  // UISYSTEM_H
