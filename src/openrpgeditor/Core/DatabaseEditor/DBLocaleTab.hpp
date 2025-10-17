#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Database.hpp"
#include "Database/Locales.hpp"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

struct DBLocaleTab final : IDBEditorTab {
  DBLocaleTab() = delete;
  explicit DBLocaleTab(DatabaseEditor* parent);
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
  bool isExperimental() const override { return true; }
  
  bool isReady() const override { return !!Database::instance()->locales; }
  bool isInitialized() const override { return m_locales; }
  void initialize() override {
    if (!isReady()) {
      return;
    }

    m_locales = &Database::instance()->locales.value();

    const auto path = Database::instance()->basePath / "locales/"; // Replace with your directory path

    try {
      if (fs::exists(path) && fs::is_directory(path)) {
        for (const auto& entry : fs::directory_iterator(path)) {
          if (is_directory(entry.status())) {
            m_localeList.push_back(entry.path().filename().generic_string());
          }
        }
      }
    } catch (const fs::filesystem_error& e) { std::cout << "Error: " << e.what() << std::endl; }

    m_jpLocales.loadMap(Database::instance()->basePath / std::format("locales/ja/Map0{:03}.json", m_selectedMap));

    for (int i = 0; i < m_locales->locales.size(); i++) {
      if (m_locales->locales.at(i).second.empty() || m_jpLocales.locales.at(i).second.empty()) {
        m_untranslatedIndices.push_back(i);
      }
    }
  }

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
