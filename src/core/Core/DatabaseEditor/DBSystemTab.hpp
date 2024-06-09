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

  [[nodiscard]] std::vector<std::string>& skillTypes() { return m_system.skillTypes; }
  [[nodiscard]] const std::vector<std::string>& skillTypes() const { return m_system.skillTypes; }

  [[nodiscard]] std::string& skillType(int id) { return m_system.skillType(id); }
  [[nodiscard]] const std::string& skillType(int id) const { return m_system.skillType(id); }

  [[nodiscard]] std::vector<std::string>& equipTypes() { return m_system.equipTypes; }
  [[nodiscard]] const std::vector<std::string>& equipTypes() const { return m_system.equipTypes; }

  [[nodiscard]] std::string& equipType(int id) { return m_system.equipType(id); }
  [[nodiscard]] const std::string& equipType(int id) const { return m_system.equipType(id); }

  [[nodiscard]] std::string& armorType(int id) { return m_system.armorTypes[id]; }
  [[nodiscard]] const std::string& armorType(int id) const { return m_system.armorTypes[id]; }

  [[nodiscard]] std::vector<std::string>& armorTypes() { return m_system.armorTypes; }
  [[nodiscard]] const std::vector<std::string>& armorTypes() const { return m_system.armorTypes; }

  [[nodiscard]] std::string& weaponType(int id) { return m_system.weaponTypes[id]; }
  [[nodiscard]] const std::string& weaponType(int id) const { return m_system.weaponTypes[id]; }

  [[nodiscard]] std::vector<std::string>& weaponTypes() { return m_system.weaponTypes; }
  [[nodiscard]] const std::vector<std::string>& weaponTypes() const { return m_system.weaponTypes; }

  [[nodiscard]] const std::optional<std::string>& basic(int id) const { return m_system.terms.basic[id]; }
  [[nodiscard]] const std::optional<std::string>& command(int id) const { return m_system.terms.commands[id]; }

  [[nodiscard]] Messages& messages() { return m_system.terms.messages; }
  [[nodiscard]] const Messages& messages() const { return m_system.terms.messages; }

private:
  System& m_system;
};
