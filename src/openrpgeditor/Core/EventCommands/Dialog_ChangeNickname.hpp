#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeNickname.hpp"

struct Project;
struct Dialog_ChangeNickname : IEventDialogController {
  Dialog_ChangeNickname() = delete;
  explicit Dialog_ChangeNickname(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new ChangeNicknameCommand());
    m_actor = command->actor;
    m_nickname = command->nick;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_actor = 1;
  std::string m_nickname = "";

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::shared_ptr<ChangeNicknameCommand> command;
  std::tuple<bool, bool> result;
};