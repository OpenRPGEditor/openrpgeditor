#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/Comment.hpp"

struct Project;
struct Dialog_Comment : IDialogController {
  Dialog_Comment() = delete;
  explicit Dialog_Comment(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new CommentCommand();
    //script = command->text;
    // TODO
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  char script[4096];
  bool m_confirmed{false};
CommentCommand* command;
  std::tuple<bool, bool> result;
};