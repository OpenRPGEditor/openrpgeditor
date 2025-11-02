#pragma once
#include "Database/EventCommands/Comment.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"
#include "imgui.h"

struct Dialog_Comment : IEventDialogController {
  Dialog_Comment() = delete;
  explicit Dialog_Comment(const std::string& name, const std::shared_ptr<CommentCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new CommentCommand());
    }
    m_comment = m_command->text.c_str();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  std::string m_comment;
  bool m_confirmed{false};
  std::shared_ptr<CommentCommand> m_command;
  std::tuple<bool, bool> result;
};