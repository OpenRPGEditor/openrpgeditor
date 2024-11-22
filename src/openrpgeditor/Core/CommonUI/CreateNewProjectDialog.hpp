#pragma once

#include "Core/CommonUI/IDialogController.hpp"
class CreateNewProjectDialog : public IDialogController {
public:
  explicit CreateNewProjectDialog(const std::string_view name) : IDialogController(name) {}
  std::tuple<bool, bool> draw() override;

  void SetOpen(bool open) override;
  std::string_view projectName() const { return m_projectName; }
  std::string_view gameTitle() const { return m_gameTitle; }
private:
  std::string m_projectName;
  std::string m_gameTitle;
};
