#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/NameInput.hpp"

struct Dialog_NameInputProcessing : IEventDialogController {
  Dialog_NameInputProcessing() = delete;
  explicit Dialog_NameInputProcessing(const std::string& name, const std::shared_ptr<NameInputCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new NameInputCommand());
    }
    m_actor = command->actorId;
    m_maxChar = command->maxChar;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_actor;
  int m_maxChar;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::shared_ptr<NameInputCommand> command;
  std::tuple<bool, bool> result;
};