#pragma once
#include "Database/EventCommands/Script.hpp"
#include "Editor/CommonUI/TextEditor.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_Script : IEventDialogController {
  Dialog_Script() = delete;
  explicit Dialog_Script(const std::string& name, const std::shared_ptr<ScriptCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ScriptCommand());
    }
    std::string text = m_command->script;
    for (const auto& next : m_command->moreScript) {
      text += !text.empty() ? "\n" + next->script : next->script;
    }
    if (!text.empty()) {
      m_textEditor.SetText(text);
    }
    m_textEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Javascript());
    m_textEditor.SetHandleKeyboardInputs(true);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  TextEditor m_textEditor;
  bool m_confirmed{false};
  std::shared_ptr<ScriptCommand> m_command;
};