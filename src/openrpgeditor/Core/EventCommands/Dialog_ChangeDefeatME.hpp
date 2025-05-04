#pragma once
#include "Core/CommonUI/AudioEditor.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/ChangeDefeatME.hpp"
#include <iostream>
#include <SFML/Audio.hpp>

namespace fs = std::filesystem;
struct Dialog_ChangeDefeatME : IEventDialogController {
  Dialog_ChangeDefeatME() = delete;
  explicit Dialog_ChangeDefeatME(const std::string& name, const std::shared_ptr<ChangeDefeatMECommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeDefeatMECommand());
    }
    m_audioRenderer.emplace(command->code(), command->me);
  }
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::shared_ptr<IEventCommand> getCommand() override { return command; }

private:
  bool m_confirmed{false};
  std::shared_ptr<ChangeDefeatMECommand> command;
  std::tuple<bool, bool> result;
  std::optional<AudioEditor> m_audioRenderer;
};
