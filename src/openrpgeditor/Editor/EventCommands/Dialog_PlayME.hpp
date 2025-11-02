#pragma once
#include "Database/Database.hpp"
#include "Database/EventCommands/PlayME.hpp"
#include "Editor/CommonUI/AudioEditor.hpp"
#include "Editor/CommonUI/Directory.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"
#include "Editor/Log.hpp"
#include "Editor/ResourceManager.hpp"
#include "Editor/Settings.hpp"
#include "Editor/Sound.hpp"
#include <iostream>
#include <SFML/Audio.hpp>

namespace fs = std::filesystem;
struct Dialog_PlayME : IEventDialogController {
  Dialog_PlayME() = delete;
  explicit Dialog_PlayME(const std::string& name, const std::shared_ptr<PlayMECommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new PlayMECommand());
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
  std::shared_ptr<PlayMECommand> m_command;
  std::optional<AudioEditor> m_audioRenderer;
};
