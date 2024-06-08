#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct Actors;
struct Actor;

struct DBActorsTab : IDBEditorTab {
  DBActorsTab() = delete;
  explicit DBActorsTab(Actors& actors);
  void draw() override;

private:
  Actors& m_actors;
  Actor* m_selectedActor{};
  float m_splitterWidth = 300.f;
};