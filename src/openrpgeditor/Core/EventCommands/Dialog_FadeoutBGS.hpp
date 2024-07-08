#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/FadeoutBGS.hpp"

struct Project;
struct Dialog_FadeoutBGS : IEventDialogController {
  Dialog_FadeoutBGS() = delete;
  explicit Dialog_FadeoutBGS(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new FadeoutBGS());
    m_duration = command->duration;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_duration;
  bool m_confirmed{false};
  std::shared_ptr<FadeoutBGS> command;
  std::tuple<bool, bool> result;
};