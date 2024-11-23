#pragma once
#include "Core/FirstBootWizard/IWizardPage.hpp"

class WelcomePage : public IWizardPage {
public:
  void draw() override;
  [[nodiscard]] constexpr std::string_view pageTitle() const override { return "Welcome to Open RPG Editor"sv; }
};
