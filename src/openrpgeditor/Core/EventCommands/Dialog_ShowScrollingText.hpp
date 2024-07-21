#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ScrollingText.hpp"
#include "Core/Log.hpp"

struct Project;
struct Dialog_ShowScrollingText : IEventDialogController {
  Dialog_ShowScrollingText() = delete;
  explicit Dialog_ShowScrollingText(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new ShowScrollTextCommand());
    m_speed = command->speed;
    m_noFast = command->noFast;
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
  Project* m_project = nullptr;

private:
  int m_speed;
  bool m_noFast;
  std::string m_textLine;

  std::vector<std::shared_ptr<ShowScrollTextCommand>> moreCommands;
  std::vector<std::shared_ptr<IEventCommand>> eventCommands;

  bool m_confirmed{false};
  std::shared_ptr<ShowScrollTextCommand> command;
  std::tuple<bool, bool> result;
};