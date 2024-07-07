#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/NameInput.hpp"

struct Project;
struct Dialog_NameInputProcessing : IDialogController {
  Dialog_NameInputProcessing() = delete;
  explicit Dialog_NameInputProcessing(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.reset(new NameInputCommand());
    m_actor = command->actorId;
    m_maxChar = command->maxChar;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:

  int m_actor;
  int m_maxChar;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::shared_ptr<NameInputCommand> command;
  std::tuple<bool, bool> result;
};