#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

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

#endif  // EVENTMANAGER_H
