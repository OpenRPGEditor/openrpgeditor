#pragma once
#include "Core/CommonUI/AudioEditor.hpp"
#include "Core/CommonUI/Directory.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Log.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Settings.hpp"
#include "Core/Sound.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/PlayBGS.hpp"
#include <iostream>
#include <SFML/Audio.hpp>

namespace fs = std::filesystem;
struct Dialog_PlayBGS : IEventDialogController {
  Dialog_PlayBGS() = delete;
  explicit Dialog_PlayBGS(const std::string& name, const std::shared_ptr<PlayBGSCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new PlayBGSCommand());
    }
    m_audioRenderer.emplace(command->code(), command->audio);
  }
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::shared_ptr<IEventCommand> getCommand() override { return command; }

private:
  bool m_confirmed{false};
  std::optional<AudioEditor> m_audioRenderer;
  std::shared_ptr<PlayBGSCommand> command;
  std::tuple<bool, bool> result;
};
