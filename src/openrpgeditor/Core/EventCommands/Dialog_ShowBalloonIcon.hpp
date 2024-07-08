#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ShowBalloonIcon.hpp"

struct Project;
struct Dialog_ShowBalloonIcon : IEventDialogController {
  Dialog_ShowBalloonIcon() = delete;
  explicit Dialog_ShowBalloonIcon(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new ShowBalloonIconCommand());
    m_character = command->id;
    m_balloonIndex = static_cast<int>(command->index);
    m_waitCompletion = command->waitForCompletion;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_character;
  int m_balloonIndex;
  bool m_waitCompletion;


  bool m_confirmed{false};
  std::shared_ptr<ShowBalloonIconCommand> command;
  std::tuple<bool, bool> result;
};