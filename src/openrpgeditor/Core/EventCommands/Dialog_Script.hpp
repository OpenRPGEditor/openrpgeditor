#pragma once
#include "Core/CommonUI/TextEditor.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/Script.hpp"

struct Dialog_Script : IEventDialogController {
  Dialog_Script() = delete;
  explicit Dialog_Script(const std::string& name, const std::shared_ptr<ScriptCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ScriptCommand());
    }
    m_Text = command->script;
    for (const auto& next : command->moreScript) {
      m_Text += !m_Text.empty() ? "\n" + next->script : next->script;
    }
    // if (!m_Text.empty()) {
    //   m_textEditor.SetText(m_Text);
    // }
    // m_textEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Javascript());
    // m_textEditor.SetHandleKeyboardInputs(true);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  TextEditor m_textEditor;
  std::string m_Text;
  bool m_confirmed{false};
  std::shared_ptr<ScriptCommand> command;
  std::tuple<bool, bool> result;
};