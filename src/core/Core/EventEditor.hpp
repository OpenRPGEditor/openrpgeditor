#pragma once

#include "Core/EventEditor/EVPage.hpp"

#include <vector>

struct Project;

struct Event;
struct EventEditor {
  EventEditor(Project* parent, Event* event);


  bool draw();

  Event* event() { return m_event; }
  const Event* event() const { return m_event; }

  Project* project() { return m_parent; }
  const Project* project() const { return m_parent; }

private:
  Project* m_parent = nullptr;
  Event* m_event = nullptr;
  std::vector<EVPage> m_pages;
  EVPage* m_selectedPage = nullptr;
  bool m_isOpen = true;
};
