#pragma once
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Core/Graphics/IconSheet.hpp"
#include "DBCommonEventsTab.hpp"
#include "Database/Animation.hpp"
#include "Database/Weapons.hpp"
struct Weapons;
struct DBWeaponsTab : IDBEditorTab {
  DBWeaponsTab() = delete;
  explicit DBWeaponsTab(Weapons& weapons, DatabaseEditor* parent);
  void draw() override;

  [[nodiscard]] std::vector<Weapon>& weapons() { return m_weapons.weapons(); }
  [[nodiscard]] const std::vector<Weapon>& weapons() const { return m_weapons.weapons(); }
  [[nodiscard]] Weapon* weapon(int id) { return m_weapons.weapon(id); }
  [[nodiscard]] const Weapon* weapon(int id) const { return m_weapons.weapon(id); }
  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(int start, int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }
  std::string getName(int index) override { return m_weapons.weapon(index)->name(); }
  int getCount() override { return m_weapons.count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Weapons"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBWeaponsTab"sv; };
private:
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
  Weapons& m_weapons;
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