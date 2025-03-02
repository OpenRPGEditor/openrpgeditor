#pragma once

#include "Database/CommonEvent.hpp"

class CommonEvents {
public:
  std::vector<std::optional<CommonEvent>>& events() { return m_events; }
  const std::vector<std::optional<CommonEvent>>& events() const { return m_events; }

  [[nodiscard]] CommonEvent* event(const int id) {
    for (auto& event : m_events) {
      if (event && event->id() == id) {
        return &event.value();
      }
    }
    return nullptr;
  }

  [[nodiscard]] const CommonEvent* event(const int id) const {
    for (const auto& event : m_events) {
      if (event && event->id() == id) {
        return &event.value();
      }
    }

    return nullptr;
  }

  int count() const { return m_events.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    const int oldSize = m_events.size();
    m_events.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_events.size(); ++i) {
        // TODO: CommonEvent constructor
        m_events[i] = CommonEvent();
        m_events[i]->commands().emplace_back(new EventDummy());
        m_events[i]->setId(i);
      }
    }
  }
  void swapReference(int targetId, int newId) {
    if (targetId >= 0 && targetId < m_events.size() && newId >= 0 && newId < m_events.size()) {
      m_events[targetId].value().setId(newId);
      m_events[newId].value().setId(targetId);
      std::swap(m_events[targetId], m_events[newId]);
    }
  }
  [[nodiscard]] bool isUpdating() const { return m_isUpdating; }
  void setUpdate(bool update) { m_isUpdating = update; }

private:
  std::vector<std::optional<CommonEvent>> m_events;
  bool m_isUpdating{false};
};
