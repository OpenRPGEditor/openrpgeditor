#pragma once
#include "Editor/EventCommands/IEventDialogController.hpp"
#include "Editor/Log.hpp"
#include "Editor/ResourceManager.hpp"
#include "Editor/Settings.hpp"

#include "Database/Database.hpp"
#include "Database/EventCommands/PlayBGM.hpp"
#include "Editor/CommonUI/AudioEditor.hpp"
#include "Editor/CommonUI/Directory.hpp"
#include "Editor/Sound.hpp"

#include <iostream>

namespace fs = std::filesystem;
struct Dialog_PlayBGM : IEventDialogController {
  Dialog_PlayBGM() = delete;
  explicit Dialog_PlayBGM(const std::string& name, const std::shared_ptr<PlayBGMCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new PlayBGMCommand());
    }
    m_audioRenderer.emplace(m_command->code(), m_command->audio);
  }
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::shared_ptr<IEventCommand> getCommand() override { return m_command; }

  void setOpen(const bool open) override {
    IEventDialogController::setOpen(open);
    m_audioRenderer->setOpen(open);
  }

private:
  bool m_confirmed{false};
  std::optional<AudioEditor> m_audioRenderer;
  std::shared_ptr<PlayBGMCommand> m_command;
};
