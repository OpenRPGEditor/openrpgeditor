#pragma once
#include "Core/CommonUI/AudioEditor.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/ChangeVehicleBGM.hpp"
#include <iostream>
#include <SFML/Audio.hpp>

namespace fs = std::filesystem;
struct Dialog_ChangeVehicleBGM : IEventDialogController {
  Dialog_ChangeVehicleBGM() = delete;
  explicit Dialog_ChangeVehicleBGM(const std::string& name, const std::shared_ptr<ChangeVehicleBGMCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeVehicleBGMCommand());
    }
    m_audioRenderer.emplace(m_command->code(), m_command->bgm);
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
  std::shared_ptr<ChangeVehicleBGMCommand> m_command;
};
