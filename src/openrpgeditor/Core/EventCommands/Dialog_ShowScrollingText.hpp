#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ScrollingText.hpp"
#include "Core/Log.hpp"

struct Dialog_ShowScrollingText : IEventDialogController {
  Dialog_ShowScrollingText() = delete;
  explicit Dialog_ShowScrollingText(const std::string& name,
                                    const std::shared_ptr<ShowScrollTextCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ShowScrollTextCommand());
    }
    m_speed = command->speed;
    m_noFast = command->noFast;
    //strncpy(m_textLine, command->command.c_str(), 4096);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  std::vector<std::shared_ptr<IEventCommand>> getBatchCommands() override {
    for (auto& commands : moreCommands) {
      std::shared_ptr<IEventCommand> sharedCommand = commands;
      eventCommands.push_back(sharedCommand);
    }
    return eventCommands;
  };

private:
  int m_speed;
  bool m_noFast;
  char m_textLine[4096];

  std::vector<std::shared_ptr<ShowScrollTextCommand>> moreCommands;
  std::vector<std::shared_ptr<IEventCommand>> eventCommands;

  bool m_confirmed{false};
  std::shared_ptr<ShowScrollTextCommand> command;
  std::tuple<bool, bool> result;
};