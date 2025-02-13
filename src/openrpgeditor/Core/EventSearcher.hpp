#pragma once

#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/Event.hpp"
#include "ReferenceSearch.hpp"
#include "imgui.h"

#include <vector>

struct MainWindow;
class EventSearcher {
public:
  explicit EventSearcher(MainWindow* parent) : reference(parent), m_parent(parent) {}

  bool getListing();

  void draw();
  void drawTable(std::string label, int mapId, int eventId, std::string eventName, int x, int y, int pageNo);
  void drawTable(int commonId, int tableIndex);

  [[nodiscard]] bool isOpen() const { return m_isOpen; }
  void open() { m_isOpen = true; }

private:
  ReferenceSearch reference;
  // std::vector<std::optional<Event>> m_events;
  std::optional<VariableSwitchPicker> picker;
  MainWindow* m_parent;
  int m_selectedVersion = -1;
  int m_selectedSearchType{0};
  int m_selectedEvent{0};
  int m_selectedSwitch{1};
  int m_selectedVariable{1};
  int m_targetId{0};
  int m_map{1};
  std::string m_searchString;
  bool m_dataType{false};
  bool m_isOpen = false;
};
