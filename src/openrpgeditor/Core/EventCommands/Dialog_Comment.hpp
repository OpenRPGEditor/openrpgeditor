#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/Comment.hpp"

struct Project;
struct Dialog_Comment : IDialogController {
  Dialog_Comment() = delete;
  explicit Dialog_Comment(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<CommentCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  char script[4096] = "";
  bool m_confirmed{false};
  std::optional<CommentCommand> command;
  std::tuple<bool, bool> result;
};