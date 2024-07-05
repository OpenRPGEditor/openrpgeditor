#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeNickname.hpp"

struct Project;
struct Dialog_ChangeNickname : IDialogController {
  Dialog_ChangeNickname() = delete;
  explicit Dialog_ChangeNickname(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command = new ChangeNicknameCommand();
    m_actor = command->actor;
    m_nickname = command->nick;
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:

  int m_actor = 1;
  std::string m_nickname = "";

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
ChangeNicknameCommand* command;
  std::tuple<bool, bool> result;
};