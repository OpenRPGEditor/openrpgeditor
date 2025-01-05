#pragma once
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Core/Graphics/FaceSheet.hpp"
#include "Core/Graphics/SideViewBattlerSheet.hpp"

#include "Database/Actors.hpp"

#include <optional>

struct DBActorsTab : IDBEditorTab {
  DBActorsTab() = delete;
  explicit DBActorsTab(Actors& actors, DatabaseEditor* parent);
  void draw() override;

  Actors& actors() { return m_actors; }
  const Actors& actors() const { return m_actors; }

  Actor* actor(int id) { return m_actors.actor(id); }
  const Actor* actor(int id) const { return m_actors.actor(id); }

  bool isModified() const { return m_actors.isModified(); }

private:
  bool checkEquipable(int etypeId, int dataId) const;
  static std::string itemDisplayName(bool isWeapon, int dataId);

  Actors& m_actors;
  Actor* m_selectedActor{};
  int m_editMaxActors{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  bool m_showEquipEdit = false;
  int m_selectedEquip = 0;
  int m_chosenEquip = 0;
  std::optional<FaceSheet> m_faceSheet;
  std::optional<CharacterSheet> m_characterSheet;
  std::optional<SideViewBattlerSheet> m_battlerSheet;
  std::optional<CheckeredCompositeTexture> m_faceButton;
  std::optional<CheckeredCompositeTexture> m_actorButton;
  std::optional<CheckeredCompositeTexture> m_battlerButton;
  TraitsEditor m_traitsEditor;
};
