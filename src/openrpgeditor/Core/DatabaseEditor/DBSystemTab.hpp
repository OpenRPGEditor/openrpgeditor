#pragma once
#include "Core/CommonUI/CharacterPicker.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Database/Actors.hpp"
#include "Database/System.hpp"

/* TODO: Move this somewhere else */
enum class AudioType {
  BackgroundMusic,
  Melody,
  SoundEffect,
};

struct System;
struct DBSystemTab : IDBEditorTab {
  DBSystemTab() = delete;
  explicit DBSystemTab(System& system, DatabaseEditor* parent);
  void draw() override;

  [[nodiscard]] std::string* element(const int id) { return m_system.element(id); }
  [[nodiscard]] const std::string* element(const int id) const { return m_system.element(id); }

  [[nodiscard]] std::vector<std::string>& elements() { return m_system.elements; }
  [[nodiscard]] const std::vector<std::string>& elements() const { return m_system.elements; }

  [[nodiscard]] size_t elementsCount() const { return m_system.elements.size(); }

  [[nodiscard]] std::string& variable(const int id) { return m_system.variables[id]; }
  [[nodiscard]] const std::string& variable(const int id) const { return m_system.variables[id]; }

  [[nodiscard]] std::string& switche(const int id) { return m_system.switches[id]; }
  [[nodiscard]] const std::string& switche(const int id) const { return m_system.switches[id]; }

  [[nodiscard]] std::vector<std::string>& skillTypes() { return m_system.skillTypes; }
  [[nodiscard]] const std::vector<std::string>& skillTypes() const { return m_system.skillTypes; }

  [[nodiscard]] std::string* skillType(const int id) { return m_system.skillType(id); }
  [[nodiscard]] const std::string* skillType(const int id) const { return m_system.skillType(id); }

  [[nodiscard]] std::vector<std::string>& equipTypes() { return m_system.equipTypes; }
  [[nodiscard]] const std::vector<std::string>& equipTypes() const { return m_system.equipTypes; }

  [[nodiscard]] std::string* equipType(const int id) { return m_system.equipType(id); }
  [[nodiscard]] const std::string* equipType(const int id) const { return m_system.equipType(id); }

  [[nodiscard]] std::string* armorType(const int id) { return m_system.armorType(id); }
  [[nodiscard]] const std::string* armorType(const int id) const { return m_system.armorType(id); }

  [[nodiscard]] std::vector<std::string>& armorTypes() { return m_system.armorTypes; }
  [[nodiscard]] const std::vector<std::string>& armorTypes() const { return m_system.armorTypes; }

  [[nodiscard]] std::string* weaponType(const int id) { return m_system.weaponType(id); }
  [[nodiscard]] const std::string* weaponType(const int id) const { return m_system.weaponType(id); }

  [[nodiscard]] std::vector<std::string>& weaponTypes() { return m_system.weaponTypes; }
  [[nodiscard]] const std::vector<std::string>& weaponTypes() const { return m_system.weaponTypes; }

  [[nodiscard]] const std::optional<std::string>& basic(const int id) const { return m_system.terms.basic[id]; }
  [[nodiscard]] const std::optional<std::string>& command(const int id) const { return m_system.terms.commands[id]; }

  [[nodiscard]] Messages& messages() { return m_system.terms.messages; }
  [[nodiscard]] const Messages& messages() const { return m_system.terms.messages; }

  void addAudioRow(const Audio& audio, const std::string& type, AudioType audioType);

private:
  System& m_system;
  CharacterSheet m_boatSheet;
  CharacterSheet m_airshipSheet;
  CharacterSheet m_shipSheet;
  CharacterSheet* m_currentSheet = nullptr;
  CharacterPicker m_characterPicker;
  CheckeredCompositeTexture m_boatButtonTexture;
  CheckeredCompositeTexture m_shipButtonTexture;
  CheckeredCompositeTexture m_airshipButtonTexture;
  std::optional<ObjectPicker<Actor>> m_actorsPicker;
};
