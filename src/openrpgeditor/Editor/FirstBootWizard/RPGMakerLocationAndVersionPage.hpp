#pragma once
#include "Editor/FirstBootWizard/IWizardPage.hpp"

class RPGMakerLocationAndVersionPage final : public IWizardPage {
public:
  void draw() override;
  [[nodiscard]] const std::string pageTitle() const override { return tr("Specify RPG Maker MV/MZ Location and Version"); }
  [[nodiscard]] bool allRequiredFieldsAreSet() const override { return m_allRequiredFieldsAreSet; }

private:
  bool m_allRequiredFieldsAreSet{false};
};