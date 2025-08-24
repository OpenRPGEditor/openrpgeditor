#pragma once

#include "Core/FirstBootWizard/IWizardPage.hpp"
class ExperimentalWarning final : public IWizardPage {
public:
  void draw() override;
  [[nodiscard]] const std::string pageTitle() const override { return tr("Experimental Version!"); }
};
