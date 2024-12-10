#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Database/Enemies.hpp"
#include "Database/Enemy.hpp"
#include "Database/Troops.hpp"

struct Troops;
struct DBTroopsTab : IDBEditorTab {
  DBTroopsTab() = delete;
  explicit DBTroopsTab(Troops& troops, DatabaseEditor* parent);
  void draw() override;

  [[nodiscard]] std::vector<Troop>& troops() { return m_troops.troops(); }
  [[nodiscard]] const std::vector<Troop>& troops() const { return m_troops.troops(); }

  [[nodiscard]] Troop* troop(const int id) { return m_troops.troop(id); }
  [[nodiscard]] const Troop* troop(const int id) const { return m_troops.troop(id); }

private:
  Troops& m_troops;
  Troop* m_selectedTroop{};
  int m_maxTroops{};
  int m_editMaxTroops{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  Enemies& m_enemies;
  Enemy* m_selectedEnemy{};

  CharacterSheet* m_currentSheet = nullptr;
  std::optional<CheckeredCompositeTexture> m_characterButtonTexture;
  std::optional<CharacterSheet> m_characterSheet;
  std::optional<CharacterSheet> m_characterPicker;
};
