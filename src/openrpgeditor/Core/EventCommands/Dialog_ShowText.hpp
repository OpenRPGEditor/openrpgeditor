#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/Plugin.hpp"
#include "Database/EventCommands/ShowText.hpp"
#include "Core/CheckerboardTexture.hpp"
#include "Core/CommonUI/CharacterPicker.hpp"
#include "Core/FaceSheet.hpp"
#include "Core/Log.hpp"

struct Project;
struct Dialog_ShowText : IEventDialogController {
  Dialog_ShowText() = delete;
  explicit Dialog_ShowText(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new ShowTextCommand());
    m_faceImage = command->faceImage;
    m_faceIndex = command->faceIndex;
    m_background = static_cast<int>(command->background);
    m_position = static_cast<int>(command->position);
    m_textLine = command->textLine;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  std::vector<std::shared_ptr<IEventCommand>> getBatchCommands() override {
    for (auto& commands : moreCommands) {
      std::shared_ptr<IEventCommand> sharedCommand = commands;
      eventCommands.push_back(sharedCommand);
    }
    return eventCommands;
  };
  Project* m_project = nullptr;

private:
  std::string m_faceImage;
  int m_faceIndex;
  int m_background;
  int m_position;
  std::string m_textLine;
  int textIndex{0};

  bool m_batchEntry{false};
  CheckerboardTexture m_buttonBack{80, 102, CellSizes::_64, 255, 200};
  CharacterPicker m_characterPicker{CharacterPicker::PickerMode::Character};

  std::optional<FaceSheet> m_faceSheet;
  std::vector<std::shared_ptr<ShowTextCommand>> moreCommands;
  std::vector<std::shared_ptr<IEventCommand>> eventCommands;

  bool m_confirmed{false};
  std::shared_ptr<ShowTextCommand> command;
  std::tuple<bool, bool> result;
};