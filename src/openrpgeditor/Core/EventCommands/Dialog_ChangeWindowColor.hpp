#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ChangeWindowColor.hpp"

struct Dialog_ChangeWindowColor : IEventDialogController {
  Dialog_ChangeWindowColor() = delete;
  explicit Dialog_ChangeWindowColor(const std::string& name, const std::shared_ptr<ChangeWindowColorCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeWindowColorCommand());
    }
    red = command->r;
    green = command->g;
    blue = command->b;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int red;
  int green;
  int blue;

  bool m_confirmed{false};
  std::shared_ptr<ChangeWindowColorCommand> command;
  std::tuple<bool, bool> result;
};