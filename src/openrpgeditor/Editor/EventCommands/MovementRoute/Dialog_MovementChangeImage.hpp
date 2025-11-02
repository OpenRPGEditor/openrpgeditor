#pragma once
#include "Database/EventCommands/MovementRoute/ChangeImage.hpp"
#include "Editor/CommonUI/CharacterPicker.hpp"
#include "Editor/CommonUI/TextEditor.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"
#include "Editor/Graphics/CharacterSheet.hpp"
#include "Editor/Graphics/CheckeredCompositeTexture.hpp"

struct MainWindow;
struct Dialog_MovementChangeImage : IEventDialogController {
  Dialog_MovementChangeImage() = delete;
  explicit Dialog_MovementChangeImage(const std::string& name, const std::shared_ptr<MovementChangeImageCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (m_command == nullptr) {
      m_command.reset(new MovementChangeImageCommand());
    }
    m_image = m_command->image;
    m_character = m_command->character;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();
  std::string m_image;
  int m_character{0};

  std::optional<CharacterSheet> m_characterSheet;
  CharacterPicker m_characterPicker;
  CheckeredCompositeTexture m_actorButton;

  std::shared_ptr<MovementChangeImageCommand> m_command;
};