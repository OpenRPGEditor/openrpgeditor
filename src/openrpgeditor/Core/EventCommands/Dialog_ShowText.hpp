#pragma once
#include "Core/CommonUI/CharacterPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Graphics/CheckerboardTexture.hpp"
#include "Core/Graphics/FaceSheet.hpp"
#include "Core/Log.hpp"
#include "Database/EventCommands/Plugin.hpp"
#include "Database/EventCommands/ShowText.hpp"

struct Dialog_ShowText : IEventDialogController {
  Dialog_ShowText() = delete;
  explicit Dialog_ShowText(const std::string& name, const std::shared_ptr<ShowTextCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ShowTextCommand());
    }
    m_faceImage = command->faceImage;
    m_faceIndex = command->faceIndex;
    m_background = static_cast<int>(command->background);
    m_position = static_cast<int>(command->position);
    std::string textStr;
    for (auto& str : command->text) {
      textStr += str->text + "\n";
    }
    APP_INFO("(loading): " + textStr);
    std::strcpy(m_textLine, textStr.c_str());
    // strncpy(m_textLine, command->textLine.c_str(), 4096);
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

private:
  std::string m_faceImage;
  int m_faceIndex;
  int m_background;
  int m_position;
  char m_textLine[4096];
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