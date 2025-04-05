#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/RotatePicture.hpp"

struct Dialog_RotatePicture : IEventDialogController {
  Dialog_RotatePicture() = delete;
  explicit Dialog_RotatePicture(const std::string& name, const std::shared_ptr<RotatePictureCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new RotatePictureCommand());
    }
    m_picture = command->picture;
    m_rotation = command->rotation;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  bool m_confirmed{false};
  int m_picture;
  int m_rotation;
  std::shared_ptr<RotatePictureCommand> command;
  std::tuple<bool, bool> result;
};