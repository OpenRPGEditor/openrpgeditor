#pragma once

#include "Core/CommonUI/CharacterPicker.hpp"
#include "Core/CommonUI/TextEditor.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Database/Database.hpp"

#include "Database/EventCommands/ChangeVehicleImage.hpp"

struct Dialog_ChangeVehicleImage : IEventDialogController {
  Dialog_ChangeVehicleImage() = delete;
  explicit Dialog_ChangeVehicleImage(const std::string& name, const std::shared_ptr<ChangeVehicleImageCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeVehicleImageCommand());
      const Vehicle* vehicle = nullptr;
      switch (m_command->vehicle) {
      case VehicleType::Boat:
        vehicle = &Database::instance()->system.boat();
        break;
      case VehicleType::Airship:
        vehicle = &Database::instance()->system.airship();
        break;
      case VehicleType::Ship:
        vehicle = &Database::instance()->system.ship();
        break;
      }
      m_command->picture = vehicle->characterName();
      m_command->pictureIndex = vehicle->characterIndex();
    }
    m_image = m_command->picture;
    m_character = m_command->pictureIndex;
    m_vehicle = static_cast<int>(m_command->vehicle);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();
  std::string m_image;
  int m_character;
  int m_vehicle;

  std::optional<CharacterSheet> m_characterSheet;
  CheckeredCompositeTexture m_characterButton;
  CharacterPicker m_characterPicker{CharacterPicker::PickerMode::Character};

  bool m_confirmed{false};
  std::shared_ptr<ChangeVehicleImageCommand> m_command;
  std::tuple<bool, bool> result;
};