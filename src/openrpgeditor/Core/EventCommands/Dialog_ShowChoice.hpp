#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ShowChoice.hpp"

struct Project;
struct Dialog_ShowChoice : IEventDialogController {
  Dialog_ShowChoice() = delete;
  explicit Dialog_ShowChoice(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new ShowChoiceCommand());
    m_choices = command->choices;
    m_background = static_cast<int>(command->background);
    m_position = static_cast<int>(command->positionType);
    m_cancelType = command->cancelType;
    m_defaultType = command->defaultType;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  std::vector<std::shared_ptr<IEventCommand>> getBatchCommands() override {
    return eventCommands;
  };
  Project* m_project = nullptr;

private:
  std::vector<std::string> m_choices;
  int m_background;
  int m_position;
  int m_defaultType;
  int m_cancelType;
  std::string m_choice_1;
  std::string m_choice_2;
  std::string m_choice_3;
  std::string m_choice_4;
  std::string m_choice_5;
  std::string m_choice_6;

  bool m_batchEntry{false};
  std::vector<std::shared_ptr<IEventCommand>> eventCommands;

  bool m_confirmed{false};
  std::shared_ptr<ShowChoiceCommand> command;
  std::tuple<bool, bool> result;
};