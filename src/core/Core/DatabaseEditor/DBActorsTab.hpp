#pragma once
#include "Core/CharacterSheet.hpp"
#include "Core/CheckerboardTexture.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include <optional>

struct Actors;
struct Actor;

struct DBActorsTab : IDBEditorTab {
  DBActorsTab() = delete;
  explicit DBActorsTab(Actors& actors, DatabaseEditor* parent);
  void draw() override;

private:
  Actors& m_actors;
  Actor* m_selectedActor{};
  int m_maxActors{};
  int m_editMaxActors;
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  bool m_showEquipEdit = false;
  int m_selectedEquip = 0;
  int m_chosenEquip = 0;
  CheckerboardTexture m_buttonBack{128, 128, CellSizes::_64, 255, 200};
  std::optional<CharacterSheet> m_charaterSheet;
};