#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeGold.hpp"
#include "Database/EventCommands/ChangeNickname.hpp"

struct Project;
struct Dialog_ChangeNickname : IDialogController {
  Dialog_ChangeNickname() = delete;
  explicit Dialog_ChangeNickname(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<ChangeNicknameCommand>(command.value()); };
  Project* m_project = nullptr;

private:

  int m_actor = 1;
  std::string m_nickname = "";

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::optional<ChangeNicknameCommand> command;
  std::tuple<bool, bool> result;
};