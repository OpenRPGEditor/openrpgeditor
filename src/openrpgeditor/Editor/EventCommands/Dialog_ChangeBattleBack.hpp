#pragma once
#include "Database/EventCommands/ChangeBattleback.hpp"
#include "Editor/CommonUI/ImagePicker.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_ChangeBattleBack : IEventDialogController {
  Dialog_ChangeBattleBack() = delete;
  explicit Dialog_ChangeBattleBack(const std::string& name, const std::shared_ptr<ChangeBattlebackCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeBattlebackCommand());
    }
    m_image = command->battleBack1Name;
    m_image_2 = command->battleBack2Name;
    m_imagePicker.emplace(ImagePicker::PickerMode::Battleback, m_image, m_image_2);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  std::string m_image;
  std::string m_image_2;

  bool m_confirmed{false};
  std::shared_ptr<ChangeBattlebackCommand> command;
  std::tuple<bool, bool> result;
  std::optional<ImagePicker> m_imagePicker;
};