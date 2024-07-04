#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/TextEditor.hpp"
#include "Database/EventCommands/Script.hpp"

struct Project;
struct Dialog_Script : IDialogController {
  Dialog_Script() = delete;
  explicit Dialog_Script(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    m_textEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Javascript());
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<ScriptCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  TextEditor m_textEditor;
  bool m_confirmed{false};
  std::optional<ScriptCommand> command;
  std::tuple<bool, bool> result;
};