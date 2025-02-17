#pragma once

#include "Core/CommonUI/IDialogController.hpp"
class CreateNewProjectDialog : public IDialogController {
public:
  explicit CreateNewProjectDialog(const std::string_view name) : IDialogController(name) {}
  std::tuple<bool, bool> draw() override;

  void setOpen(bool open) override;
  std::string_view projectName() const { return m_projectName; }
  std::string_view gameTitle() const { return m_gameTitle; }
  bool copyExample() const { return m_copyExample; }

private:
  std::string m_projectName;
  std::string m_gameTitle;
  bool m_copyExample{true};
};
