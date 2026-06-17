#pragma once

#include <memory>
#include <string>

class ScriptTextEditor {
public:
  ScriptTextEditor();
  ~ScriptTextEditor();

  ScriptTextEditor(ScriptTextEditor&& other) noexcept;
  ScriptTextEditor& operator=(ScriptTextEditor&& other) noexcept;

  ScriptTextEditor(const ScriptTextEditor&) = delete;
  ScriptTextEditor& operator=(const ScriptTextEditor&) = delete;

  void SetupJavascript();
  void SetText(const std::string& text);
  std::string GetText() const;
  void SetReadOnlyEnabled(bool enabled);

  void DrawPanel(const char* editorId, bool readOnly = false, bool border = true, float bottomReserved = 0.f);
  static void DrawPickers();

private:
  void insertAtCursor(const std::string& text);

  struct Impl;
  std::unique_ptr<Impl> m_impl;
};

using TextEditor = ScriptTextEditor;
