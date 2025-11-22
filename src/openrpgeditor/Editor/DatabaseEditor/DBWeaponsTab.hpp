#pragma once
#include "Database/Animation.hpp"
#include "Database/Weapons.hpp"
#include "DBCommonEventsTab.hpp"
#include "Editor/CommonUI/TraitsEditor.hpp"
#include "Editor/DatabaseEditor/IDBEditorTab.hpp"
#include "Editor/Graphics/CheckeredCompositeTexture.hpp"
#include "Editor/Graphics/IconSheet.hpp"

class Weapons;
struct DBWeaponsTab final : IDBCoreEditorTab<DBWeaponsTab> {
  void draw() override;

  std::string objectName(const int index) const override { return m_weapons->weapon(index)->name(); }

  int getCount() const override { return m_weapons->count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Weapons"); }

  bool isReady() const override { return !!Database::instance()->weapons; }
  bool isInitialized() const override { return m_weapons; }

  void initialize() override {
    if (!isReady()) {
      return;
    }
    m_weapons = &Database::instance()->weapons.value();
    m_selectedWeapon = m_weapons->weapon(1);
    if (m_selectedWeapon) {
      m_traitsEditor.setTraits(&m_selectedWeapon->traits());
    }
    m_maxWeapons = m_weapons->count();
  }

private:
  Weapons* m_weapons{};
  Weapon* m_selectedWeapon{};
  int m_maxWeapons{};
  int m_editMaxWeapons{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  IconSheet* m_currentSheet = nullptr;
  std::optional<CheckeredCompositeTexture> m_itemButtonTexture;
  std::optional<IconSheet> m_itemSheet;
  std::optional<IconSheet> m_itemPicker;
  TraitsEditor m_traitsEditor;
  std::optional<AnimationPicker> m_animationPicker;
};