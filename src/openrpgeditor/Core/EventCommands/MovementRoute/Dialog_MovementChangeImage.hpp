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
  : IEventDialogController(name)
  , command(cmd) {
    if (command == nullptr) {
      command.reset(new MovementChangeImageCommand());
    }
    m_image = command->image;
    m_character = command->character;
    m_characterSheet = CharacterSheet(m_image);
    if (!m_actorButton->hasCompositeTextures() && m_characterSheet) {
      auto [min, max] = m_characterSheet.value().getRectForCharacter(m_character, 1);

      m_actorButton->setTexturesToComposite({{m_characterSheet->texture(),
                                              {m_characterSheet->characterWidth(), m_characterSheet->characterHeight()},
                                              {static_cast<int>(min.x() * m_characterSheet->texture().width()), static_cast<int>(min.y() * m_characterSheet->texture().height())}}});
    }
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