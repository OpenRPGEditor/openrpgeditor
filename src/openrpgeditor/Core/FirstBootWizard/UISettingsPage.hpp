#pragma once
#include "Core/FirstBootWizard/IWizardPage.hpp"

class UISettingsPage final : public IWizardPage {
public:
  void draw() override;
  [[nodiscard]] const std::string pageTitle() const override { return tr("UI Settings"); }

private:
  bool m_uiScaleChanged{false};
};
