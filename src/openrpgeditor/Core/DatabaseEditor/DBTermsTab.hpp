#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Database.hpp"

#include <orei18n.hpp>

#include <string>

struct System;
class DBTermsTab final : public IDBCoreEditorTab<DBTermsTab> {
public:
  void draw() override;

  [[nodiscard]] std::string tabName() const override { return tr("Terms"); }

  bool isReady() const override { return !!Database::instance()->system; }
  bool isInitialized() const override { return m_system; }
  void initialize() override {
    if (!isReady()) {
      return;
    }

    m_system = &Database::instance()->system.value();
  }

private:
  static std::tuple<bool, bool> drawAbbreviatedString(float width, std::string_view label, std::string& full, std::string& abbrv);
  static bool drawString(float width, std::string_view label, std::string& full);
  void initializeParameterStrings() const;
  void initializeBasicStrings() const;
  void initializeCommandStrings() const;
  System* m_system{};
};
