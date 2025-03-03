#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Core/TroopsEventEditor/TroopsEVEditor.hpp"
#include "DBCommonEventsTab.hpp"
#include "Database/Actor.hpp"
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
  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(int start, int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }

  std::string getName(int index) override { return m_troops.troop(index)->name(); }
  int getCount() override { return m_troops.count(); }

private:
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
  Troops& m_troops;
  Troop* m_selectedTroop{};
  int m_maxTroops{};
  int m_editMaxTroops{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  bool m_isOpen{false};
  Enemies& m_enemies;
  Enemy* m_selectedEnemy{};
  int m_selectedPage = 0;
  std::vector<TroopsEVPage> m_pages;

  CharacterSheet* m_currentSheet = nullptr;
  std::optional<CheckeredCompositeTexture> m_characterButtonTexture;
  std::optional<CharacterSheet> m_characterSheet;
  std::optional<CharacterSheet> m_characterPicker;

  TroopsEVEditor m_troopsEditor;
};
