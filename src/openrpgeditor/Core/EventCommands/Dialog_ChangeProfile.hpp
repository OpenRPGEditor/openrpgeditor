#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeGold.hpp"
#include "Database/EventCommands/ChangeNickname.hpp"
#include "Database/EventCommands/ChangeProfile.hpp"
#include "Database/EventCommands/RecoverAll.hpp"

struct Dialog_ChangeProfile : IEventDialogController {
  Dialog_ChangeProfile() = delete;
  explicit Dialog_ChangeProfile(const std::string& name, const std::shared_ptr<ChangeProfileCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeProfileCommand());
    }
    m_actor = command->actor;
    m_profile = command->profile;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_actor;
  std::string m_profile;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::shared_ptr<ChangeProfileCommand> command;
  std::tuple<bool, bool> result;
};