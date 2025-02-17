#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

#include <map>
#include <string>
#include <vector>

struct MainWindow;
class ReferenceSearch {
public:
  explicit ReferenceSearch(MainWindow* parent);

  [[nodiscard]] std::map<int, std::vector<int>> getEvents() const { return m_event; }
  [[nodiscard]] std::map<int, std::vector<int>> getCommands() const { return m_list; }
  [[nodiscard]] std::vector<int> getCommons() const { return m_common; }
  void findAllReferences(int targetId, SearchType type);
  void findAllReferences(std::string text, SearchType type);
  void searchCommonByVariable(int targetId, SearchType type);
  void searchCommonByVariable(std::string text, SearchType type);
  int getCommonSize() { return m_common.size(); }
  int getListSize() { return m_list.size(); }
  int getEventSize() { return m_event.size(); }
  int totalSize() { return getCommonSize() + getListSize() + getEventSize(); }
  void clear() {
    m_event.clear();
    m_list.clear();
    m_common.clear();
  }

private:
  std::map<int, std::vector<int>> m_event;
  std::map<int, std::vector<int>> m_list;
  std::vector<int> m_common;
  MainWindow* m_parent; // Common Event Results
};
