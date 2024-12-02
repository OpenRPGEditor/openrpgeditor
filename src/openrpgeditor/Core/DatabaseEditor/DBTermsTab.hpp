#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

#include <string>

struct System;
class DBTermsTab final : public IDBEditorTab {
public:
  explicit DBTermsTab(System& system, DatabaseEditor* parent) : IDBEditorTab(parent), m_system(system) {}
  void draw() override;

private:
  static void drawAbbreviatedString(float width, std::string_view label, std::string& full, std::string& abbrv);
  static void drawString(float width, std::string_view label, std::string& full);
  void initializeParameterStrings() const;
  void initializeBasicStrings() const;
  void initializeCommandStrings() const;
  System& m_system;
};
