#pragma once
#include "Core/CommonUI/ActionsEditor.hpp"
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "DBCommonEventsTab.hpp"
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
  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(int start, int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }
  std::string getName(int index) override { return m_enemies.enemy(index)->name(); }
  int getCount() override { return m_enemies.count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Enemies"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBEnemiesTab"sv; };

private:
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
  void drawPopup();
  std::string getDropString(int dropIndex, int kindId) const;
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
  std::optional<ItemPicker> item_picker;
  std::optional<ArmorPicker> armor_picker;
  std::optional<WeaponPicker> weapon_picker;
  std::vector<int> m_item{1, 1, 1};

  int m_dropIndex{0};
  int m_dropDenominator{1};
};