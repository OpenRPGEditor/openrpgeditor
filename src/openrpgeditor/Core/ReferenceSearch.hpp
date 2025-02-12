#pragma once
#include "Database/IModifiable.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

struct MainWindow;
class ReferenceSearch {
public:
  explicit ReferenceSearch(MainWindow* parent);

  [[nodiscard]] std::map<int, std::vector<std::shared_ptr<const IModifiable>>> getEvents() const { return m_events; }
  [[nodiscard]] std::map<int, std::vector<std::shared_ptr<const IModifiable>>> getCommands() const { return m_listEvents; }
  [[nodiscard]] std::vector<int> getCommons() const { return m_common; }
  void findAllReferences(int targetId, SearchType type);
  void findAllReferences(std::string text, SearchType type);
  void searchCommonByVariable(int targetId, SearchType type);

private:
  std::map<int, std::vector<std::shared_ptr<const IModifiable>>> m_events;     // Event Condition Results
  std::map<int, std::vector<std::shared_ptr<const IModifiable>>> m_listEvents; // Command Results
  std::vector<int> m_common;
  MainWindow* m_parent; // Common Event Results
};
