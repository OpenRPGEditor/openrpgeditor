#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ErasePicture.hpp"

struct Dialog_ErasePicture : IEventDialogController {
  Dialog_ErasePicture() = delete;
  explicit Dialog_ErasePicture(const std::string& name, const std::shared_ptr<ErasePictureCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ErasePictureCommand());
    }
    m_picture = command->picture;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  bool m_confirmed{false};
  int m_picture;
  std::shared_ptr<ErasePictureCommand> command;
  std::tuple<bool, bool> result;
};