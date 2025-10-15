#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Database.hpp"

#include <orei18n.hpp>

#include <string>

struct System;
class DBTermsTab final : public IDBEditorTab {
public:
  explicit DBTermsTab(DatabaseEditor* parent)
  : IDBEditorTab(parent) {}
  void draw() override;
  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(int start, int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }

  std::string getName(int index) override { return ""; }
  int getCount() override { return 0; }

  [[nodiscard]] std::string tabName() const override { return tr("Terms"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBTermsTab"sv; };

  bool isReady() const override { return !!Database::instance()->system; }
  bool isInitialized() const override { return m_system; }
  void initialize() override {
    if (!isReady()) {
      return;
    }

    m_system = &Database::instance()->system.value();
  }

private:
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
  static std::tuple<bool, bool> drawAbbreviatedString(float width, std::string_view label, std::string& full, std::string& abbrv);
  static bool drawString(float width, std::string_view label, std::string& full);
  void initializeParameterStrings() const;
  void initializeBasicStrings() const;
  void initializeCommandStrings() const;
  System* m_system{};
};
