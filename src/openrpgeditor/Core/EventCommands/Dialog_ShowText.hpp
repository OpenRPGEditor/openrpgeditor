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
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ShowTextCommand());
    }
    m_faceImage = m_command->faceImage;
    m_faceIndex = m_command->faceIndex;
    m_background = m_command->background;
    m_position = m_command->position;
    for (const auto& str : m_command->text) {
      m_textLine += (m_textLine.empty() ? "" : "\n") + str->text;
    };
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };
  std::vector<std::shared_ptr<IEventCommand>> getBatchCommands() override {
    for (auto& commands : m_moreCommands) {
      std::shared_ptr<IEventCommand> sharedCommand = commands;
      m_eventCommands.push_back(sharedCommand);
    }
    return m_eventCommands;
  };

private:
  std::string m_faceImage;
  int m_faceIndex;
  TextBackground m_background;
  TextWindowPosition m_position;
  std::string m_textLine;

  bool m_batchEntry{false};
  CheckerboardTexture m_buttonBack{80, 102, CellSizes::_64, 255, 200};
  CharacterPicker m_characterPicker{CharacterPicker::PickerMode::Character};

  std::optional<FaceSheet> m_faceSheet;
  std::vector<std::shared_ptr<ShowTextCommand>> m_moreCommands;
  std::vector<std::shared_ptr<IEventCommand>> m_eventCommands;

  bool m_confirmed{false};
  std::shared_ptr<ShowTextCommand> m_command;
  std::tuple<bool, bool> m_result;
};