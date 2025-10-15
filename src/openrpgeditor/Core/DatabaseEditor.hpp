#pragma once

#include "Utils/SignalSlot.hpp"

#include "Core/DatabaseEditor/IDBEditorTab.hpp"

#include "Core/Graphics/IconSheet.hpp"

struct MainWindow;
class DatabaseEditor {
public:
  DatabaseEditor();
  void drawCategoryHeaders();
  void draw();

  void open() { m_isOpen = true; }

  bool isReady() const {
    return std::ranges::all_of(m_editorTabs, [](const auto& v) { return v->isReady(); });
  }

  std::string& getFilterString() { return m_searchString; } // TODO: Search by string
  bool isFilteredByCategory() const { return m_filterByHeader && m_selectedHeaderIndex != -1; }

  bool isOpen() const { return m_isOpen; }
  [[nodiscard]] IDBEditorTab* getCurrentTab() const { return m_currentTab; }

  rpgmutils::signal<void()> onReady;

  void addTab(const std::shared_ptr<IDBEditorTab>& tab) { m_editorTabs.emplace_back(tab); }

private:
  MainWindow* m_parent;
  std::vector<std::shared_ptr<IDBEditorTab>> m_editorTabs;

  std::string m_searchString;
  bool m_filterByHeader{false};
  int m_selectedHeaderIndex{-1};
  std::vector<std::string> m_headers;

  IDBEditorTab* m_currentTab = nullptr;
  bool m_isOpen{};
};