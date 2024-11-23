#pragma once
#include "Core/FirstBootWizard/IWizardPage.hpp"

class UISettingsPage : public IWizardPage {
public:
  void draw() override;
  [[nodiscard]] constexpr std::string_view pageTitle() const override { return "UI Settings"sv; }

private:
  bool m_fontSizeChanged{false};
};
