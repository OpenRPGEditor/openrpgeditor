#pragma once
#include "Core/CommonUI/TextEditor.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/MovementRoute/Script.hpp"

struct MainWindow;
struct Dialog_MovementScript : IEventDialogController {
  Dialog_MovementScript() = delete;
  explicit Dialog_MovementScript(const std::string& name)
  : IEventDialogController(name) {
    command.reset(new MovementScriptCommand());
    m_Text = command->script;
    m_textEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Javascript());
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  TextEditor m_textEditor;
  std::string m_Text;
  bool m_confirmed{false};
  std::shared_ptr<MovementScriptCommand> command;
  std::tuple<bool, bool> result;
};