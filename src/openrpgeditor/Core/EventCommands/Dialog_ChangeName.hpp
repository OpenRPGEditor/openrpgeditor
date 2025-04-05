#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"

#include "Database/Actor.hpp"
#include "Database/EventCommands/ChangeName.hpp"

struct Dialog_ChangeName : IEventDialogController {
  Dialog_ChangeName() = delete;
  explicit Dialog_ChangeName(const std::string& name, const std::shared_ptr<ChangeNameCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeNameCommand());
    }
    m_actor = command->actor;
    m_str = command->name;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_actor;
  std::string m_str;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::shared_ptr<ChangeNameCommand> command;
  std::tuple<bool, bool> result;
};