#pragma once
#include "Core/FirstBootWizard/IWizardPage.hpp"

class ProjectLocationPage : public IWizardPage {
public:
  void draw() override;
  [[nodiscard]] constexpr std::string_view pageTitle() const override { return "Specify Default Project Location"sv; }
  [[nodiscard]] bool allRequiredFieldsAreSet() const override { return m_allRequiredFieldsAreSet; }

private:
  bool m_allRequiredFieldsAreSet{false};
};
