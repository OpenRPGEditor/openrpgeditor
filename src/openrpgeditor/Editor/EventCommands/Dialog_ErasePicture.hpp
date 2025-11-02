#pragma once
#include "Database/EventCommands/ErasePicture.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_ErasePicture : IEventDialogController {
  Dialog_ErasePicture() = delete;
  explicit Dialog_ErasePicture(const std::string& name, const std::shared_ptr<ErasePictureCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ErasePictureCommand());
    }
    m_picture = m_command->picture;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  bool m_confirmed{false};
  int m_picture;
  std::shared_ptr<ErasePictureCommand> m_command;
  std::tuple<bool, bool> result;
};