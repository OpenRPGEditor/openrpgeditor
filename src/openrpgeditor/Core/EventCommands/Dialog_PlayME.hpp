#pragma once
#include "Core/CommonUI/AudioEditor.hpp"
#include "Core/CommonUI/Directory.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Log.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Settings.hpp"
#include "Core/Sound.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/PlayME.hpp"
#include <iostream>
#include <SFML/Audio.hpp>

namespace fs = std::filesystem;
struct Dialog_PlayME : IEventDialogController {
  Dialog_PlayME() = delete;
  explicit Dialog_PlayME(const std::string& name, const std::shared_ptr<PlayMECommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new PlayMECommand());
    }
    m_audioRenderer.emplace(command->code(), command->audio);
  }
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::shared_ptr<IEventCommand> getCommand() override { return command; }

private:
  bool m_confirmed{false};
  std::tuple<bool, bool> result;
  std::shared_ptr<PlayMECommand> command;
  std::optional<AudioEditor> m_audioRenderer;
};
