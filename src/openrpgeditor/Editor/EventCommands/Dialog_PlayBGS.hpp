#pragma once
#include "Database/EventCommands/PlayBGS.hpp"
#include "Editor/CommonUI/AudioEditor.hpp"
#include "Editor/CommonUI/Directory.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

namespace fs = std::filesystem;
struct Dialog_PlayBGS : IEventDialogController {
  Dialog_PlayBGS() = delete;
  explicit Dialog_PlayBGS(const std::string& name, const std::shared_ptr<PlayBGSCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new PlayBGSCommand());
    }
    m_audioRenderer.emplace(AudioEditor::AudioType::BGS, m_command->bgs);
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
  std::shared_ptr<PlayBGSCommand> m_command;
};
