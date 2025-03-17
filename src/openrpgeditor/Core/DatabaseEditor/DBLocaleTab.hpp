#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Locales.hpp"

struct DBLocaleTab : IDBEditorTab {
  DBLocaleTab() = delete;
  explicit DBLocaleTab(Locales& locales, DatabaseEditor* parent);
  void draw() override;
  int getHeader(int index) override { return 0; }
  std::vector<int>& getHeaders() override {
    std::vector<int> none;
    return none;
  }
  bool hasHeader() override { return false; }
  void setHeaderRange(int start, int end) override {}
  std::string getName(int index) override { return ""; }
  int getCount() override { return 0; }

private:
  Locales* m_locales = nullptr;
  Locales m_jpLocales;

  std::vector<std::string> m_localeList;

  int m_selectedMap{1};
  int m_selectedIndex{-1};
  int m_editingIndex{-1};
  int m_selectedLanguage{1};

  bool m_mainLanguageDirty{false};
  bool m_compareLanguageDirty{false};
  bool m_mainLanguageEdit{false};
};
