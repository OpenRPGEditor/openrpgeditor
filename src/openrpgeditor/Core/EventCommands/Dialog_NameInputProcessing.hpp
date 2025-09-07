#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"

#include "Database/Actor.hpp"
#include "Database/EventCommands/NameInput.hpp"

struct Dialog_NameInputProcessing : IEventDialogController {
  Dialog_NameInputProcessing() = delete;
  explicit Dialog_NameInputProcessing(const std::string& name, const std::shared_ptr<NameInputCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new NameInputCommand());
    }
    m_actor = m_command->actorId;
    m_maxChar = m_command->maxChar;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();

  int m_actor;
  int m_maxChar;

  bool m_confirmed{false};
  std::optional<ActorPicker> m_actorPicker;
  std::shared_ptr<NameInputCommand> m_command;
  std::tuple<bool, bool> m_result;
};