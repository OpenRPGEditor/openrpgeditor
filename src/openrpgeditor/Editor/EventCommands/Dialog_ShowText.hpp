#pragma once
#include "Database/EventCommands/Plugin.hpp"
#include "Database/EventCommands/ShowText.hpp"
#include "Editor/CommonUI/CharacterPicker.hpp"
#include "Editor/CommonUI/FacePicker.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"
#include "Editor/Graphics/CheckerboardTexture.hpp"
#include "Editor/Graphics/CheckeredCompositeTexture.hpp"
#include "Editor/Graphics/FaceSheet.hpp"
#include "Editor/Log.hpp"

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
  void drawPickers();
  std::string m_faceImage;
  int m_faceIndex;
  TextBackground m_background;
  TextWindowPosition m_position;
  std::string m_textLine;

  bool m_batchEntry{false};
  CharacterPicker m_characterPicker{CharacterPicker::PickerMode::Character};
  CheckeredCompositeTexture m_faceButtonTexture;

  std::optional<FaceSheet> m_faceSheet;
  std::optional<FacePicker> m_facePicker;
  std::vector<std::shared_ptr<ShowTextCommand>> m_moreCommands;
  std::vector<std::shared_ptr<IEventCommand>> m_eventCommands;

  bool m_confirmed{false};
  std::shared_ptr<ShowTextCommand> m_command;
};