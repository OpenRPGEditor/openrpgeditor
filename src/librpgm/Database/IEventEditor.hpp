#pragma once

#include <tuple>

class Event;
class IEventEditor {
public:
  IEventEditor() = delete;
  virtual ~IEventEditor() = default;

  virtual std::tuple<bool, bool> draw() = 0;

  virtual bool isOpen() const = 0;
  virtual void open() = 0;
  virtual bool confirmed() const = 0;
  virtual void accept() const = 0;

  int id() const;

  Event* event() const { return m_event; }
  /* Client code must implement this to create their own event editors */
  static IEventEditor* create(Event* ev);

protected:
  friend class Event;
  virtual void eventPointerInvalidated() = 0;
  explicit IEventEditor(Event* event) : m_event(event) {};
  void setEventPtr(Event* ev) {
    m_event = ev;
    eventPointerInvalidated();
  }
  Event* m_event{nullptr};
};