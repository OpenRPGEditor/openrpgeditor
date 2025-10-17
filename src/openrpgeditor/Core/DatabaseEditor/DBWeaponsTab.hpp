#pragma once
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Core/Graphics/IconSheet.hpp"
#include "Database/Animation.hpp"
#include "Database/Weapons.hpp"
#include "DBCommonEventsTab.hpp"

class Weapons;
struct DBWeaponsTab final : IDBEditorTab {
  void draw() override;

  std::string getName(const int index) const override { return m_weapons->weapon(index)->name(); }

  int getCount() const override { return m_weapons->count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Weapons"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBWeaponsTab"sv; };

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