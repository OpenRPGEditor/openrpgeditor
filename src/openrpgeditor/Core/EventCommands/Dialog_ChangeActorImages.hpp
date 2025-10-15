#pragma once
#include "Core/CommonUI/CharacterPicker.hpp"
#include "Core/CommonUI/FacePicker.hpp"
#include "Core/CommonUI/ImagePicker.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/TextEditor.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Core/Graphics/FaceSheet.hpp"
#include "Core/Graphics/SideViewBattlerSheet.hpp"
#include "Database/Actors.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/ChangeActorImage.hpp"

struct Dialog_ChangeActorImages : IEventDialogController {
  Dialog_ChangeActorImages() = delete;
  explicit Dialog_ChangeActorImages(const std::string& name, const std::shared_ptr<ChangeActorImageCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeActorImageCommand());
      const auto actor = Database::instance()->actors->actor(m_command->actor);
      m_command->charPicture = actor->characterName();
      m_command->charIndex = actor->characterIndex();
      m_command->facePicture = actor->faceName();
      m_command->faceIndex = actor->faceIndex();
      m_command->battlerPicture = actor->battlerName();
    }
    m_actor = m_command->actor;
    m_charPicture = m_command->charPicture;
    m_charIndex = m_command->charIndex;
    m_facePicture = m_command->facePicture;
    m_faceIndex = m_command->faceIndex;
    m_battlerPicture = m_command->battlerPicture;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();
  int m_actor;
  std::string m_charPicture;
  int m_charIndex;
  std::string m_facePicture;
  int m_faceIndex;
  std::string m_battlerPicture;

  int m_imageSelection{0};

  CheckeredCompositeTexture m_faceButton;
  CheckeredCompositeTexture m_characterButton;
  CheckeredCompositeTexture m_battlerButton;

  FacePicker m_facePicker;
  CharacterPicker m_characterPicker{CharacterPicker::PickerMode::Character};
  ImagePicker m_battlerPicker{ImagePicker::PickerMode::SVBattler};

  std::optional<CharacterSheet> m_characterSheet;
  std::optional<FaceSheet> m_faceSheet;
  std::optional<SideViewBattlerSheet> m_battlerSheet;

  bool m_confirmed{false};
  std::optional<ActorPicker> m_actorPicker;
  std::shared_ptr<ChangeActorImageCommand> m_command;
};