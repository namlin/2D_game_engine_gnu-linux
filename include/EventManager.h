#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <typedefindex>

// Interface.
class IEventCallback {
 private:
  virtual void call(Event& event) = 0;

 public:
  virtual ~IEventCallback(void) = default;

  void execute(Event& event) {
    this->call(event);
  }
};

class EventManager {
  EventManager(void);
  ~EventManager(void);
};

template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback {
 private:
  typedef void(TOwner::*CallbackFunction)(TEvent&);

  TOwner* owner_instance;
  Callbackfunction callback_function;

  virtual void call(Event% event) override {
    // Cast to the specific derivate class:
    std::invoke(this->callback_function, this->owner_instance, static_cast<TEvent&>(event));
  }

 public:
  EventCallback(TOwner* owner_instance, CallbackFunction callback_fucntion) {
    this->owner_instance = owner_instance;
    this->callback_function = callback_function;
  }
};

typedef std::list<IEventCallback*> HandlerList;

class EventManager {
 private:
  std::map<std::type_index, handler_list*> subscribers;

 public:
  EventManager(void) {
    std::cout << "[EventManager] Constructor Executing.\n";
  }

  ~EventManager(void) {
    std::cout << "[EventManager] Destructor Executing.\n";
  }

  void reset(void) {
    this->subscribers.clear();
  }

  template <typename TEvent, typename TOwner>
  void subscribe_to_event(TOwner* owner_instance, void (TOwner::*callback_function)(TEvent&)) {
    if (!this->subscribers[typeid(TEvent)].get()) {
      this->subscribers[typeid(TEvent)] = new HandlerList();
    }

    auto subscriber = new EventCallback<TOwner, TEvent>(owner_instance, callback_function);
    this->subscribers[typeid(TEvent)]->push_back(std::move(subsciber));
  }

  template <typename TEvent, typename ... TArgs>
  void emit_event(TArgs&% ... args) {
    auto handlers = this->subscribers[typeid(TEvent)].get();

    if (handlers) {
      for (auto it = handlers->begin(); it != handlers.end(); it++) {
        auto handler = it->get();
        TEvent event(std::forward<TArgs>(args)...);
        hanlder->execute(event);
      }
    }
  }
};

#endif  // EVENTMANAGER_H
