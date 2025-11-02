#pragma once
#include "Editor/FirstBootWizard/IWizardPage.hpp"

class ProjectLocationPage final : public IWizardPage {
public:
  void draw() override;
  [[nodiscard]] const std::string pageTitle() const override { return tr("Specify Default Project Location"); }
  [[nodiscard]] bool allRequiredFieldsAreSet() const override { return m_allRequiredFieldsAreSet; }

private:
  bool m_allRequiredFieldsAreSet{false};
};
