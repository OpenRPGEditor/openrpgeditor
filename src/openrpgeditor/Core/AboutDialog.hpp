#pragma once

#include "Core/CommonUI/IDialogController.hpp"
class AboutDialog final : public IDialogController {
public:
  AboutDialog()
  : IDialogController("AboutDialog") {}
  std::tuple<bool, bool> draw() override;

private:
  bool m_showImGuiCredits{false};
};
