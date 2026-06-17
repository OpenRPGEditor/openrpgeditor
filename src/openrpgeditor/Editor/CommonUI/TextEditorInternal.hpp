#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <imgui.h>

class TextEditorInternal {
public:
  using AutoCompleteHandler = std::function<void(std::vector<std::string>& suggestions, const std::string& searchTerm, std::string_view linePrefix)>;

  TextEditorInternal();
  ~TextEditorInternal();

  void setupJavascript();
  void setAutoCompleteHandler(AutoCompleteHandler handler);
  void setText(const std::string& text);
  std::string getText() const;
  void setReadOnlyEnabled(bool enabled);
  void setShowLineNumbersEnabled(bool enabled);

  void setFocus();
  void replaceTextAtCursor(const std::string& text);
  bool canUndo() const;
  bool canRedo() const;
  void undo();
  void redo();
  void cut();
  void copy();
  void paste();
  void openFindReplaceWindow();
  void toggleComments();
  void render(const char* id, ImVec2 size, bool border);

private:
  struct Impl;
  std::unique_ptr<Impl> m_impl;
};
