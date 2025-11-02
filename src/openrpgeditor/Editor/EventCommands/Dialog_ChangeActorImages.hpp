#pragma once
#include "Database/Actors.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/ChangeActorImage.hpp"
#include "Editor/CommonUI/CharacterPicker.hpp"
#include "Editor/CommonUI/FacePicker.hpp"
#include "Editor/CommonUI/ImagePicker.hpp"
#include "Editor/CommonUI/ObjectPicker.hpp"
#include "Editor/CommonUI/TextEditor.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"
#include "Editor/Graphics/CharacterSheet.hpp"
#include "Editor/Graphics/CheckeredCompositeTexture.hpp"
#include "Editor/Graphics/FaceSheet.hpp"
#include "Editor/Graphics/SideViewBattlerSheet.hpp"

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