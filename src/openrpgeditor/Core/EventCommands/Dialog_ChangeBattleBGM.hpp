#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Log.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Settings.hpp"

#include "Core/CommonUI/AudioEditor.hpp"
#include "Core/Sound.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/ChangeBattleBGM.hpp"

#include <iostream>

namespace fs = std::filesystem;
struct Dialog_ChangeBattleBGM : IEventDialogController {
  Dialog_ChangeBattleBGM() = delete;
  explicit Dialog_ChangeBattleBGM(const std::string& name, const std::shared_ptr<ChangeBattleBGMCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeBattleBGMCommand());
    }
    m_audioRenderer.emplace(command->code(), command->bgm);
  }
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::shared_ptr<IEventCommand> getCommand() override { return command; }

private:
  bool m_confirmed{false};
  std::optional<AudioEditor> m_audioRenderer;
  std::shared_ptr<ChangeBattleBGMCommand> command;
  std::tuple<bool, bool> result;
};
