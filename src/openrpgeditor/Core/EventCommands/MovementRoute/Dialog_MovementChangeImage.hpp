#pragma once
#include "Core/CommonUI/CharacterPicker.hpp"
#include "Core/CommonUI/TextEditor.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Database/EventCommands/MovementRoute/ChangeImage.hpp"

struct MainWindow;
struct Dialog_MovementChangeImage : IEventDialogController {
  Dialog_MovementChangeImage() = delete;
  explicit Dialog_MovementChangeImage(const std::string& name, const std::shared_ptr<MovementChangeImageCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (command == nullptr) {
      command.reset(new MovementChangeImageCommand());
    }
    m_image = command->image;
    m_character = command->character;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  std::string m_image;
  int m_character{0};

  std::optional<CharacterSheet> m_characterSheet;
  CheckerboardTexture m_buttonBack{80, 102, CellSizes::_64, 255, 200};
  CharacterPicker m_characterPicker;
  std::optional<CheckeredCompositeTexture> m_actorButton;

  bool m_confirmed{false};
  std::shared_ptr<MovementChangeImageCommand> command;
  std::tuple<bool, bool> result;
};