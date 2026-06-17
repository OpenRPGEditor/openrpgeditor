#include "Editor/CommonUI/TextEditor.hpp"
#include "Editor/CommonUI/TextEditorInternal.hpp"

#include "Database/Database.hpp"
#include "Database/GameConstants.hpp"
#include "Editor/Application.hpp"
#include "Editor/CommonUI/VariableSwitchPicker.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "Editor/Settings.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <map>
#include <optional>

namespace {

struct PickerContext {
  ScriptTextEditor* editor = nullptr;
  std::optional<VariableSwitchPicker> picker;
  VariableSwitchPicker::Type type{VariableSwitchPicker::Type::Variable};
};

PickerContext g_picker;

std::optional<std::string> constantAlias(const std::map<int, std::string>& values, int id) {
  if (const auto it = values.find(id); it != values.end() && !it->second.empty()) {
    return it->second;
  }
  return std::nullopt;
}

std::string formatVariableInsert(int id) {
  if (const auto* database = Database::instance(); database && database->gameConstants) {
    if (const auto alias = constantAlias(database->gameConstants->variables, id)) {
      return std::format("VAR_{}", *alias);
    }
  }
  return std::format("$gameVariables.value({})", id);
}

std::string formatSwitchInsert(int id) {
  if (const auto* database = Database::instance(); database && database->gameConstants) {
    if (const auto alias = constantAlias(database->gameConstants->switches, id)) {
      return std::format("SW_{}", *alias);
    }
  }
  return std::format("$gameSwitches.value({})", id);
}

bool canOpenPicker() {
  const auto* database = Database::instance();
  return database && database->system;
}

void openPicker(ScriptTextEditor& editor, VariableSwitchPicker::Type type) {
  const auto* database = Database::instance();
  if (!database || !database->system) {
    return;
  }

  g_picker.editor = &editor;
  g_picker.type = type;
  if (type == VariableSwitchPicker::Type::Variable) {
    g_picker.picker.emplace(type, database->system->variables(), 1);
  } else {
    g_picker.picker.emplace(type, database->system->switches(), 1);
  }
  g_picker.picker->setOpen(true);
}

} // namespace

struct ScriptTextEditor::Impl {
  TextEditorInternal editor;

  void insertTextAtCursor(const std::string& text) {
    editor.setFocus();
    editor.replaceTextAtCursor(text);
  }

  void drawToolbar(ScriptTextEditor& owner, bool readOnly) {
    ImGui::BeginHorizontal("##script_text_editor_toolbar", {-1, 0}, 0);
    {
      ImGui::BeginDisabled(readOnly || !editor.canUndo());
      if (ImGui::Button(trNOOP("Undo"))) {
        editor.undo();
      }
      ImGui::EndDisabled();

      ImGui::BeginDisabled(readOnly || !editor.canRedo());
      if (ImGui::Button(trNOOP("Redo"))) {
        editor.redo();
      }
      ImGui::EndDisabled();

      ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

      ImGui::BeginDisabled(readOnly);
      if (ImGui::Button(trNOOP("Cut"))) {
        editor.cut();
      }
      ImGui::EndDisabled();

      if (ImGui::Button(trNOOP("Copy"))) {
        editor.copy();
      }

      ImGui::BeginDisabled(readOnly);
      if (ImGui::Button(trNOOP("Paste"))) {
        editor.paste();
      }
      ImGui::EndDisabled();

      ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

      if (ImGui::Button(trNOOP("Find..."))) {
        editor.openFindReplaceWindow();
      }

      ImGui::BeginDisabled(readOnly);
      if (ImGui::Button(trNOOP("Comment"))) {
        editor.toggleComments();
      }
      ImGui::EndDisabled();

      ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

      ImGui::BeginDisabled(readOnly || !canOpenPicker());
      if (ImGui::Button(trNOOP("Insert..."))) {
        ImGui::OpenPopup("##script_insert_popup");
      }
      if (ImGui::BeginPopup("##script_insert_popup")) {
        if (ImGui::MenuItem(trNOOP("Variable"))) {
          openPicker(owner, VariableSwitchPicker::Type::Variable);
        }
        if (ImGui::MenuItem(trNOOP("Switch"))) {
          openPicker(owner, VariableSwitchPicker::Type::Switch);
        }
        ImGui::EndPopup();
      }
      ImGui::EndDisabled();

      ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

      bool showLineNumbers = Settings::instance()->scriptEditorShowLineNumbers;
      if (ImGui::Checkbox(trNOOP("Line #"), &showLineNumbers)) {
        Settings::instance()->scriptEditorShowLineNumbers = showLineNumbers;
        editor.setShowLineNumbersEnabled(showLineNumbers);
        App::APP->serializeSettings();
      }
    }
    ImGui::EndHorizontal();
  }

  void applyDefaults() { editor.setShowLineNumbersEnabled(Settings::instance()->scriptEditorShowLineNumbers); }
};

ScriptTextEditor::ScriptTextEditor() : m_impl(std::make_unique<Impl>()) {}

ScriptTextEditor::~ScriptTextEditor() = default;

ScriptTextEditor::ScriptTextEditor(ScriptTextEditor&& other) noexcept : m_impl(std::move(other.m_impl)) {}

ScriptTextEditor& ScriptTextEditor::operator=(ScriptTextEditor&& other) noexcept {
  m_impl = std::move(other.m_impl);
  return *this;
}

void ScriptTextEditor::SetupJavascript() {
  m_impl->editor.setupJavascript();
  m_impl->applyDefaults();
}

void ScriptTextEditor::SetText(const std::string& text) { m_impl->editor.setText(text); }

std::string ScriptTextEditor::GetText() const { return m_impl->editor.getText(); }

void ScriptTextEditor::SetReadOnlyEnabled(bool enabled) { m_impl->editor.setReadOnlyEnabled(enabled); }

void ScriptTextEditor::insertAtCursor(const std::string& text) { m_impl->insertTextAtCursor(text); }

void ScriptTextEditor::DrawPanel(const char* editorId, bool readOnly, bool border, float bottomReserved) {
  m_impl->drawToolbar(*this, readOnly);
  const float bodyHeight = std::max(0.f, ImGui::GetContentRegionAvail().y - bottomReserved);
  ImGui::PushFont(App::APP->getMonoFont());
  m_impl->editor.render(editorId, {0, bodyHeight}, border);
  ImGui::PopFont();
}

void ScriptTextEditor::DrawPickers() {
  if (!g_picker.picker) {
    return;
  }

  const auto [closed, confirmed] = g_picker.picker->draw();
  if (!closed) {
    return;
  }

  if (confirmed && g_picker.editor) {
    const int id = g_picker.picker->selection();
    const std::string insert = g_picker.type == VariableSwitchPicker::Type::Variable ? formatVariableInsert(id) : formatSwitchInsert(id);
    g_picker.editor->insertAtCursor(insert);
  }

  g_picker.picker.reset();
  g_picker.editor = nullptr;
}
