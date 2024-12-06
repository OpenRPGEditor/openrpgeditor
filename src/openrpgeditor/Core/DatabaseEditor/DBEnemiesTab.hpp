#pragma once
#include "Core/CommonUI/ActionsEditor.hpp"
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Database/Armors.hpp"
#include "Database/Enemies.hpp"
#include "Database/Items.hpp"
#include "Database/Weapons.hpp"

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
  void drawPopup();
  std::string GetDropString(int dropIndex, int kindId);
  Enemies& m_enemies;
  Enemy* m_selectedEnemy{};
  int m_editMaxEnemies{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  int m_dropSelection{0};
  bool m_isOpen{false};
  bool m_isDropConfirmed{false};
  CharacterSheet* m_currentSheet = nullptr;
  std::optional<CheckeredCompositeTexture> m_characterButtonTexture;
  std::optional<CharacterSheet> m_characterSheet;
  std::optional<CharacterSheet> m_characterPicker;
  TraitsEditor m_traitsEditor;
  ActionsEditor m_actionsEditor;
  std::optional<ObjectPicker<Item>> item_picker;
  std::optional<ObjectPicker<Armor>> armor_picker;
  std::optional<ObjectPicker<Weapon>> weapon_picker;
  std::vector<int> m_item{1, 1, 1};

  int m_dropIndex{0};
  int m_dropDenominator{1};
};