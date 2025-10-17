#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Core/TroopsEventEditor/TroopsEVEditor.hpp"
#include "Database/Actor.hpp"
#include "Database/Enemies.hpp"
#include "Database/Enemy.hpp"
#include "Database/Troops.hpp"
#include "DBCommonEventsTab.hpp"

class Troops;
struct DBTroopsTab final : IDBCoreEditorTab<DBTroopsTab> {
  void draw() override;

  std::string getName(const int index) const override { return m_troops->troop(index)->name(); }
  int getCount() const override { return m_troops->count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Troops"); }

  bool isReady() const override { return Database::instance()->enemies && Database::instance()->troops; }

  void initialize() override {
    if (!isReady()) {
      return;
    }
    m_troops = &Database::instance()->troops.value();
    m_enemies = &Database::instance()->enemies.value();
    /* FIXME: Currently causes a crash */
    // m_troopsEditor.setTroop(m_troops->troop(1));
    // m_selectedTroop = m_troops->troop(1);
    m_maxTroops = m_troops->count();
  }

  bool isInitialized() const override { return m_troops && m_enemies; }

private:
  Troops* m_troops = nullptr;
  Troop* m_selectedTroop{};
  int m_maxTroops{};
  int m_editMaxTroops{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  bool m_isOpen{false};
  Enemies* m_enemies = nullptr;
  Enemy* m_selectedEnemy{};
  int m_selectedPage = 0;
  std::vector<TroopsEVPage> m_pages;

  CharacterSheet* m_currentSheet = nullptr;
  std::optional<CheckeredCompositeTexture> m_characterButtonTexture;
  std::optional<CharacterSheet> m_characterSheet;
  std::optional<CharacterSheet> m_characterPicker;

  TroopsEVEditor m_troopsEditor{nullptr};
};
