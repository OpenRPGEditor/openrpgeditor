#pragma once

#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/TextEditor.hpp"
#include "Core/CommonUI/CharacterPicker.hpp"
#include "Core/Graphics/CharacterSheet.hpp"

#include "Database/EventCommands/ChangeVehicleImage.hpp"

struct Dialog_ChangeVehicleImage : IEventDialogController {
  Dialog_ChangeVehicleImage() = delete;
  explicit Dialog_ChangeVehicleImage(const std::string& name,
                                     const std::shared_ptr<ChangeVehicleImageCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeVehicleImageCommand());
    }
    m_image = command->picture;
    m_character = command->pictureIndex;
    m_vehicle = static_cast<int>(command->vehicle);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  std::string m_image;
  int m_character;
  int m_vehicle;

  std::optional<CharacterSheet> m_characterSheet;
  CheckerboardTexture m_buttonBack{80, 102, CellSizes::_64, 255, 200};
  CharacterPicker m_characterPicker{CharacterPicker::PickerMode::Character};

  bool m_confirmed{false};
  std::shared_ptr<ChangeVehicleImageCommand> command;
  std::tuple<bool, bool> result;
};