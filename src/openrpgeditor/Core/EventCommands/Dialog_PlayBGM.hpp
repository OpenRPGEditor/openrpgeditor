#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Log.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Settings.hpp"

#include "Core/CommonUI/AudioEditor.hpp"
#include "Core/CommonUI/Directory.hpp"
#include "Core/Sound.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/PlayBGM.hpp"

#include <iostream>

namespace fs = std::filesystem;
struct Dialog_PlayBGM : IEventDialogController {
  Dialog_PlayBGM() = delete;
  explicit Dialog_PlayBGM(const std::string& name, const std::shared_ptr<PlayBGMCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new PlayBGMCommand());
    }
    m_audioRenderer.emplace(command->code(), command->audio);
  }
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::shared_ptr<IEventCommand> getCommand() override { return command; }

private:
  bool m_confirmed{false};
  std::optional<AudioEditor> m_audioRenderer;
  std::shared_ptr<PlayBGMCommand> command;
  std::tuple<bool, bool> result;
};
