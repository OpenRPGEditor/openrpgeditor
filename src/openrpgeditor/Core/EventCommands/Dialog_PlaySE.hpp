#pragma once
#include "Core/CommonUI/AudioEditor.hpp"
#include "Core/CommonUI/Directory.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Log.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Settings.hpp"
#include "Core/Sound.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/PlaySE.hpp"
#include <iostream>
#include <SFML/Audio.hpp>

namespace fs = std::filesystem;
struct Dialog_PlaySE : IEventDialogController {
  Dialog_PlaySE() = delete;
  explicit Dialog_PlaySE(const std::string& name, const std::shared_ptr<PlaySECommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new PlaySECommand());
    }
    m_audioRenderer.emplace(command->code(), command->audio);
  }
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::shared_ptr<IEventCommand> getCommand() override { return command; }

private:
  bool m_confirmed{false};
  std::optional<AudioEditor> m_audioRenderer;
  std::shared_ptr<PlaySECommand> command;
  std::tuple<bool, bool> result;
};
