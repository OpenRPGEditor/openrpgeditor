#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

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
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
};