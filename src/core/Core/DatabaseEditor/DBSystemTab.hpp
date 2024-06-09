#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/System.hpp"

struct System;
struct DBSystemTab : IDBEditorTab {
  DBSystemTab() = delete;
  explicit DBSystemTab(System& system, DatabaseEditor* parent) : IDBEditorTab(parent), m_system(system) {}
  void draw() override;

  [[nodiscard]] std::string& element(int id) { return m_system.elements[id]; }
  [[nodiscard]] const std::string& element(int id) const { return m_system.elements[id]; }
  [[nodiscard]] std::string& variable(int id) { return m_system.variables[id]; }
  [[nodiscard]] const std::string& variable(int id) const { return m_system.variables[id]; }
  [[nodiscard]] std::string& switche(int id) { return m_system.switches[id]; }
  [[nodiscard]] const std::string& switche(int id) const { return m_system.switches[id]; }

  [[nodiscard]] std::vector<std::string>& equipTypes() { return m_system.equipTypes; }
  [[nodiscard]] const std::vector<std::string>& equipTypes() const { return m_system.equipTypes; }
  [[nodiscard]] std::string& equipType(int id) { return m_system.equipType(id); }
  [[nodiscard]] const std::string& equipType(int id) const { return m_system.equipType(id); }
  [[nodiscard]] const std::optional<std::string>& basic(int id) const { return m_system.terms.basic[id]; }
  [[nodiscard]] const std::optional<std::string>& command(int id) const { return m_system.terms.commands[id]; }
  [[nodiscard]] Messages& messages() { return m_system.terms.messages; }
  [[nodiscard]] const Messages& messages() const { return m_system.terms.messages; }

private:
  System& m_system;
};
