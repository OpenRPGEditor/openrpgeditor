#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/Comment.hpp"

struct Dialog_Comment : IEventDialogController {
  Dialog_Comment() = delete;
  explicit Dialog_Comment(const std::string& name, const std::shared_ptr<CommentCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new CommentCommand());
    }
    // script = command->text;
    //  TODO
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  char script[4096];
  bool m_confirmed{false};
  std::shared_ptr<CommentCommand> command;
  std::tuple<bool, bool> result;
};