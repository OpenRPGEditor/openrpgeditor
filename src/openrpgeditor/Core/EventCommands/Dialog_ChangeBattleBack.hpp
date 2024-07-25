#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ChangeBattleback.hpp"
#include "Core/CommonUI/ImagePicker.hpp"

struct Project;
struct Dialog_ChangeBattleBack : IEventDialogController {
  Dialog_ChangeBattleBack() = delete;
  explicit Dialog_ChangeBattleBack(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new ChangeBattlebackCommand());
    m_image = command->battleBack1Name;
    m_image_2 = command->battleBack2Name;
    m_imagePicker.emplace(ImagePicker::PickerMode::Battleback, m_image, m_image_2);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  std::string m_image;
  std::string m_image_2;

  bool m_confirmed{false};
  std::shared_ptr<ChangeBattlebackCommand> command;
  std::tuple<bool, bool> result;
  std::optional<ImagePicker> m_imagePicker;
};