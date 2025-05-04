#pragma once
#include "Core/CommonUI/AudioEditor.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/ChangeVictoryME.hpp"
#include <iostream>
#include <SFML/Audio.hpp>

namespace fs = std::filesystem;
struct Dialog_ChangeVictoryME : IEventDialogController {
  Dialog_ChangeVictoryME() = delete;
  explicit Dialog_ChangeVictoryME(const std::string& name, const std::shared_ptr<ChangeVictoryMECommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeVictoryMECommand());
    }
    m_audioRenderer.emplace(command->code(), command->me);
  }
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::shared_ptr<IEventCommand> getCommand() override { return command; }

private:
  bool m_confirmed{false};
  std::optional<AudioEditor> m_audioRenderer;
  std::shared_ptr<ChangeVictoryMECommand> command;
  std::tuple<bool, bool> result;
};
