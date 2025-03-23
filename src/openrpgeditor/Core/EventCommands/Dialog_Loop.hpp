#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/EventDummy.hpp"
#include "Database/EventCommands/Loop.hpp"
#include "Database/EventCommands/RepeatAbove.hpp"

struct Dialog_Loop : IEventDialogController {
  Dialog_Loop() = delete;
  explicit Dialog_Loop(const std::string& name, const std::shared_ptr<LoopCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new LoopCommand());
    }
    m_open = false;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; }

  std::vector<std::shared_ptr<IEventCommand>> getBatchCommands() override {
    std::vector<std::shared_ptr<IEventCommand>> eventCommands;
    std::shared_ptr<IEventCommand> sharedCommand = getCommand();
    eventCommands.push_back(sharedCommand);
    eventCommands.back()->setIndent(getParentIndent().value());
    eventCommands.push_back(std::make_shared<EventDummy>());
    eventCommands.back()->setIndent(getParentIndent().value() + 1);
    eventCommands.push_back(std::make_shared<RepeatAboveCommand>());
    eventCommands.back()->setIndent(getParentIndent().value());
    return eventCommands;
  }

private:
  bool m_confirmed{true};
  std::shared_ptr<LoopCommand> command;
  std::tuple<bool, bool> result;
};