#pragma once
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Core/Graphics/FaceSheet.hpp"
#include "Core/Graphics/SideViewBattlerSheet.hpp"
#include "DBCommonEventsTab.hpp"

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

  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(int start, int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }
  std::string getName(int index) override { return m_actors.actor(index)->name(); }
  int getCount() override { return m_actors.count(); }

private:
  int m_categoryStart;
  int m_categoryEnd;
  bool checkEquipable(int etypeId, int dataId) const;
  static std::string itemDisplayName(bool isWeapon, int dataId);

  std::vector<int> m_headers;
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
