#pragma once
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Database/Enemies.hpp"

struct Enemies;
struct DBEnemiesTab : IDBEditorTab {
  DBEnemiesTab() = delete;
  explicit DBEnemiesTab(Enemies& enemies, DatabaseEditor* parent);
  void draw() override;

  [[nodiscard]] std::vector<Enemy>& enemies() { return m_enemies.enemies(); }
  [[nodiscard]] const std::vector<Enemy>& enemies() const { return m_enemies.enemies(); }

  [[nodiscard]] Enemy* enemy(int id) { return m_enemies.enemy(id); }
  [[nodiscard]] const Enemy* enemy(int id) const { return m_enemies.enemy(id); }

private:
  Enemies& m_enemies;
  Enemy* m_selectedEnemy{};
  int m_editMaxEnemies{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  CharacterSheet* m_currentSheet = nullptr;
  std::optional<CheckeredCompositeTexture> m_characterButtonTexture;
  std::optional<CharacterSheet> m_characterSheet;
  std::optional<CharacterSheet> m_characterPicker;
  TraitsEditor m_traitsEditor;
};