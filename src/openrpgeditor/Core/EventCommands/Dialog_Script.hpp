#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/TextEditor.hpp"
#include "Database/EventCommands/Script.hpp"

struct Project;
struct Dialog_Script : IDialogController {
  Dialog_Script() = delete;
  explicit Dialog_Script(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new ScriptCommand();
    m_Text = command->script;
    m_textEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Javascript());
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  TextEditor m_textEditor;
  std::string m_Text;
  bool m_confirmed{false};
ScriptCommand* command;
  std::tuple<bool, bool> result;
};