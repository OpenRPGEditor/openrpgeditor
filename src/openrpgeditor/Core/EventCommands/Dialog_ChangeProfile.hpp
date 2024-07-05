#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeGold.hpp"
#include "Database/EventCommands/ChangeNickname.hpp"
#include "Database/EventCommands/ChangeProfile.hpp"
#include "Database/EventCommands/RecoverAll.hpp"

struct Project;
struct Dialog_ChangeProfile : IDialogController {
  Dialog_ChangeProfile() = delete;
  explicit Dialog_ChangeProfile(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<ChangeProfileCommand>(command.value()); };
  Project* m_project = nullptr;

private:

  int m_actor = 1;
  std::string m_profile = "";

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::optional<ChangeProfileCommand> command;
  std::tuple<bool, bool> result;
};