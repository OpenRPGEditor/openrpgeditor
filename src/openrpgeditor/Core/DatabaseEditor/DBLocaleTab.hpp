#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/Image.hpp"
#include "Database/Locales.hpp"

struct DBLocaleTab : IDBEditorTab {
  DBLocaleTab() = delete;
  explicit DBLocaleTab(Locales& locales, DatabaseEditor* parent);
  void draw() override;
  void drawTableOne(std::string& text, int index);
  void drawTableTwo(int index);
  int getHeader(int index) override { return 0; }

  std::vector<int>& getHeaders() override { return m_untranslatedIndices; }
  bool hasHeader() override { return false; }
  void setHeaderRange(int start, int end) override {}
  std::string getName(int index) override { return ""; }
  int getCount() override { return 0; }

  // TODO: Fuzzy searching

  [[nodiscard]] std::string tabName() const override { return tr("Locales"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBLocaleTab"sv; };

private:
  Locales* m_locales = nullptr;
  Locales m_jpLocales;

  std::string m_searchString;

  std::vector<std::string> m_localeList;
  std::vector<int> m_untranslatedIndices;

  int m_selectedMap{1};
  int m_selectedIndex{-1};
  int m_editingIndex{-1};
  int m_selectedLanguage{1};
  int m_selectedUntranslated{-1};

  bool m_mainLanguageDirty{false};
  bool m_compareLanguageDirty{false};
  bool m_mainLanguageEdit{false};
  bool m_filterUntranslated{false};
  bool m_navigator{false};
};
