#pragma once
#include "Core/CommonUI/CharacterPicker.hpp"
#include "Core/CommonUI/TextEditor.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Database/EventCommands/MovementRoute/ChangeImage.hpp"

struct MainWindow;
struct Dialog_MovementChangeImage : IEventDialogController {
  Dialog_MovementChangeImage() = delete;
  explicit Dialog_MovementChangeImage(const std::string& name) : IEventDialogController(name) {
    command.reset(new MovementChangeImageCommand());
    m_image = command->image;
    m_character = command->character;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  std::string m_image;
  int m_character;

  std::optional<CharacterSheet> m_characterSheet;
  CheckerboardTexture m_buttonBack{80, 102, CellSizes::_64, 255, 200};
  CharacterPicker m_characterPicker{CharacterPicker::PickerMode::Character};

  bool m_confirmed{false};
  std::shared_ptr<MovementChangeImageCommand> command;
  std::tuple<bool, bool> result;
};