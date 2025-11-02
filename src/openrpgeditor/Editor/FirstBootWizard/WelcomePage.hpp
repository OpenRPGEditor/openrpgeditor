#pragma once
#include "Editor/FirstBootWizard/IWizardPage.hpp"

class WelcomePage final : public IWizardPage {
public:
  void draw() override;
  [[nodiscard]] const std::string pageTitle() const override { return tr("Welcome to Open RPG Editor"); }
};
