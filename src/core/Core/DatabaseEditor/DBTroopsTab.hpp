//
// Created by antidote on 6/7/24.
//

#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct Troops;
struct DBTroopsTab : IDBEditorTab {
  DBTroopsTab() = delete;
  explicit DBTroopsTab(Troops& troops, DatabaseEditor* parent) : IDBEditorTab(parent), m_troops(troops) {}
  void draw() override;

private:
  Troops& m_troops;
};
