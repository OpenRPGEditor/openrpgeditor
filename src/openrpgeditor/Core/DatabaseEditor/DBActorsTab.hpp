#pragma once
#include "Core/CharacterSheet.hpp"
#include "Core/CheckerboardTexture.hpp"
#include "Core/FaceSheet.hpp"
#include "Core/SideViewBattlerSheet.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Database/Actors.hpp"
#include "Database/Trait.hpp"

#include <optional>

struct DBActorsTab : IDBEditorTab {
  DBActorsTab() = delete;
  explicit DBActorsTab(Actors& actors, DatabaseEditor* parent);
  void draw() override;


  Actors& actors() { return m_actors; }
  const Actors& actors() const { return m_actors; }

  Actor* actor(int id) { return m_actors.actor(id); }
  const Actor* actor(int id) const { return m_actors.actor(id); }

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
  CheckerboardTexture m_buttonBack{144, 144, CellSizes::_32, 220, 255};
  std::optional<FaceSheet> m_faceSheet;
  std::optional<CharacterSheet> m_characterSheet;
  std::optional<SideViewBattlerSheet> m_battlerSheet;
  TraitsEditor m_traitsEditor;

};
