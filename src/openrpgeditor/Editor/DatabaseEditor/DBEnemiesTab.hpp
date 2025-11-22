#pragma once
#include "Database/Armors.hpp"
#include "Database/Enemies.hpp"
#include "Database/Items.hpp"
#include "Database/Weapons.hpp"
#include "DBCommonEventsTab.hpp"
#include "Editor/CommonUI/ActionsEditor.hpp"
#include "Editor/CommonUI/TraitsEditor.hpp"
#include "Editor/DatabaseEditor/IDBEditorTab.hpp"
#include "Editor/Graphics/CharacterSheet.hpp"
#include "Editor/Graphics/CheckeredCompositeTexture.hpp"

class Enemies;
class DBEnemiesTab final : public IDBCoreEditorTab<DBEnemiesTab> {
public:
  void draw() override;

  std::string objectName(const int index) const override { return m_enemies->enemy(index)->name(); }
  int getCount() const override { return m_enemies->count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Enemies"); }

  bool isReady() const override { return Database::instance()->system && Database::instance()->enemies && Database::instance()->skills && Database::instance()->states; }

  void initialize() override {
    if (!isReady()) {
      return;
    }
    m_enemies = &Database::instance()->enemies.value();
    m_selectedEnemy = m_enemies->enemy(1);
    if (m_selectedEnemy) {
      m_traitsEditor.setTraits(&m_selectedEnemy->traits());
      m_actionsEditor.setActions(&m_selectedEnemy->actions());
    }
  }

  bool isInitialized() const override { return m_enemies; }

private:
  void drawPopup();
  std::string getDropString(int dropIndex, int kindId) const;
  Enemies* m_enemies = nullptr;
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