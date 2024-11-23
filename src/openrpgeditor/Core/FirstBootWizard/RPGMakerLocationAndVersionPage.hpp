#pragma once
#include "Core/FirstBootWizard/IWizardPage.hpp"

class RPGMakerLocationAndVersionPage : public IWizardPage {
public:
  void draw() override;
  [[nodiscard]] constexpr std::string_view pageTitle() const override {
    return "Specify RPG Maker MV/MZ Location and Version"sv;
  }
  [[nodiscard]] bool allRequiredFieldsAreSet() const override { return m_allRequiredFieldsAreSet; }

private:
  bool m_allRequiredFieldsAreSet{false};
};