#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/FadeoutBGM.hpp"

struct Project;
struct Dialog_FadeoutBGM : IEventDialogController {
  Dialog_FadeoutBGM() = delete;
  explicit Dialog_FadeoutBGM(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new FadeoutBGM());
    m_duration = command->duration;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_duration;
  bool m_confirmed{false};
  std::shared_ptr<FadeoutBGM> command;
  std::tuple<bool, bool> result;
};