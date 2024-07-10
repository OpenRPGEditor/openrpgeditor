#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/TextEditor.hpp"
#include "Database/EventCommands/MovementRoute/Frequency.hpp"

struct Project;
struct Dialog_MovementChangeFrequency : IEventDialogController {
  Dialog_MovementChangeFrequency() = delete;
  explicit Dialog_MovementChangeFrequency(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new MovementFrequencyCommand());
    m_frequency = command->frequency;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_frequency;
  bool m_confirmed{false};
  std::shared_ptr<MovementFrequencyCommand> command;
  std::tuple<bool, bool> result;
};