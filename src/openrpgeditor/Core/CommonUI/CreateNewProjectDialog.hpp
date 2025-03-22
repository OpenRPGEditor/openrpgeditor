#pragma once

#include "Core/CommonUI/IDialogController.hpp"
class CreateNewProjectDialog : public IDialogController {
public:
  enum class ProjectType {
    RPGMV,
    RPGMZ,
  };
  explicit CreateNewProjectDialog(const std::string_view name) : IDialogController(name) {}
  std::tuple<bool, bool> draw() override;

  void setOpen(bool open) override;
  std::string_view projectName() const { return m_projectName; }
  std::string_view gameTitle() const { return m_gameTitle; }
  bool copyExample() const { return m_copyExample; }

  ProjectType projectType() const { return m_projectType; }

private:
  std::string m_projectName;
  std::string m_gameTitle;
  ProjectType m_projectType{ProjectType::RPGMV};
  bool m_copyExample{true};
};
