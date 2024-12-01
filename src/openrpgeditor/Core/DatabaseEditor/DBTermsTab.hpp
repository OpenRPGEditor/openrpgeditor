#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct System;
class DBTermsTab final : public IDBEditorTab {
public:
  explicit DBTermsTab(System& system, DatabaseEditor* parent) : IDBEditorTab(parent), m_system(system) {}
  void draw() override;

private:
  System& m_system;
};
