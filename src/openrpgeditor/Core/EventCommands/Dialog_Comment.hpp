#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/Comment.hpp"

struct Project;
struct Dialog_Comment : IEventDialogController {
  Dialog_Comment() = delete;
  explicit Dialog_Comment(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new CommentCommand());
    // script = command->text;
    //  TODO
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  char script[4096];
  bool m_confirmed{false};
  std::shared_ptr<CommentCommand> command;
  std::tuple<bool, bool> result;
};