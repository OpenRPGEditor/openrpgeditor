#pragma once
#include "Core/CommonUI/CharacterPicker.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/TextEditor.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/FaceSheet.hpp"
#include "Core/Graphics/SideViewBattlerSheet.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeActorImage.hpp"

struct Dialog_ChangeActorImages : IEventDialogController {
  Dialog_ChangeActorImages() = delete;
  explicit Dialog_ChangeActorImages(const std::string& name, const std::shared_ptr<ChangeActorImageCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeActorImageCommand());
    }
    m_actor = command->actor;
    m_charPicture = command->charPicture;
    m_charIndex = command->charIndex;
    m_facePicture = command->facePicture;
    m_faceIndex = command->faceIndex;
    m_battlerPicture = command->battlerPicture;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_actor;
  std::string m_charPicture;
  int m_charIndex;
  std::string m_facePicture;
  int m_faceIndex;
  std::string m_battlerPicture;

  int m_image_selection{0};

  CheckerboardTexture m_buttonBack{80, 102, CellSizes::_64, 255, 200};
  CharacterPicker m_characterPicker{CharacterPicker::PickerMode::Character};

  std::optional<CharacterSheet> m_characterSheet;
  std::optional<FaceSheet> m_faceSheet;
  std::optional<SideViewBattlerSheet> m_battlerSheet;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::shared_ptr<ChangeActorImageCommand> command;
  std::tuple<bool, bool> result;
};