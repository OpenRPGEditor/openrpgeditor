#include "Editor/CommonUI/TextEditorInternal.hpp"

#include "TextEditor.h"

#include <cctype>
#include <chrono>
#include <string_view>

namespace {

bool isJsPunctuation(ImWchar character) {
  static bool punctuation[128] = {
      false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
      false, true,  false, false, false, true,  true,  false, true,  true,  true,  true,  true,  true,  true,  true,
      false, false, false, false, false, false, false, false, false, false, true,  true,  true,  true,  true,  true,
      false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false, false, true,  false, true,  true,  false,
      false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false, false, false, false, true,  true,  true,  true,  false,
  };

  return character < 127 ? punctuation[character] : false;
}

TextEditor::Iterator getJsIdentifier(TextEditor::Iterator start, TextEditor::Iterator end) {
  if (start >= end) {
    return start;
  }

  if (*start == '$') {
    ++start;
    while (start < end && TextEditor::CodePoint::isXidContinue(*start)) {
      ++start;
    }
    return start;
  }

  if (TextEditor::CodePoint::isXidStart(*start)) {
    ++start;
    while (start < end && TextEditor::CodePoint::isXidContinue(*start)) {
      ++start;
    }
  }

  return start;
}

TextEditor::Iterator getJsNumber(TextEditor::Iterator start, TextEditor::Iterator end) {
  if (start >= end) {
    return start;
  }

  auto i = start;
  const ImWchar first = *i;

  if (first == '0') {
    auto next = i;
    ++next;
    if (next < end && (*next == 'x' || *next == 'X')) {
      i = next;
      ++i;
      const auto hexStart = i;
      while (i < end && std::isxdigit(static_cast<unsigned char>(*i))) {
        ++i;
      }
      return i > hexStart ? i : start;
    }
  }

  if (first == '.') {
    auto next = i;
    ++next;
    if (next >= end || !std::isdigit(static_cast<unsigned char>(*next))) {
      return start;
    }
    ++i;
  } else if (!std::isdigit(static_cast<unsigned char>(first))) {
    return start;
  }

  while (i < end && std::isdigit(static_cast<unsigned char>(*i))) {
    ++i;
  }

  if (i < end && *i == '.') {
    ++i;
    while (i < end && std::isdigit(static_cast<unsigned char>(*i))) {
      ++i;
    }
  }

  if (i < end && (*i == 'e' || *i == 'E')) {
    ++i;
    if (i < end && (*i == '+' || *i == '-')) {
      ++i;
    }
    const auto exponentStart = i;
    while (i < end && std::isdigit(static_cast<unsigned char>(*i))) {
      ++i;
    }
    if (i == exponentStart) {
      return start;
    }
  }

  return i > start ? i : start;
}

const TextEditor::Language* javascriptLanguage() {
  static bool initialized = false;
  static TextEditor::Language language;

  if (!initialized) {
    language.name = "Javascript";
    language.singleLineComment = "//";
    language.commentStart = "/*";
    language.commentEnd = "*/";
    language.hasDoubleQuotedStrings = true;
    language.stringEscape = '\\';

    static constexpr const char* keywords[] = {
        "await",     "break",  "case",    "catch", "class",   "const",   "continue",  "debugger",  "default",    "delete", "do",    "else",
        "enum",      "export", "extends", "false", "finally", "for",     "function",  "if",        "implements", "import", "in",    "instanceof",
        "interface", "let",    "new",     "null",  "package", "private", "protected", "prototype", "public",     "return", "super", "switch",
        "static",    "this",   "throw",   "try",   "true",    "typeof",  "var",       "void",      "while",      "with",   "yield", "console",
    };

    static constexpr const char* identifiers[] = {
        "Math",      "Array",       "string",  "Object", "prototype", "keys",     "values",     "entries", "call",   "apply",   "bind",   "toUpperCase", "toLowerCase", "split",
        "substring", "lastIndexOf", "indexOf", "charAt", "length",    "splice",   "slice",      "join",    "concat", "unshift", "shift",  "pop",         "push",        "abs",
        "ceil",      "floor",       "round",   "max",    "min",       "parseInt", "parseFloat", "Number",  "String", "Boolean", "random", "log",
    };

    for (const auto& keyword : keywords) {
      language.keywords.insert(keyword);
    }
    for (const auto& identifier : identifiers) {
      language.identifiers.insert(identifier);
    }

    language.isPunctuation = isJsPunctuation;
    language.getIdentifier = getJsIdentifier;
    language.getNumber = getJsNumber;
    initialized = true;
  }

  return &language;
}

} // namespace

std::string autocompleteLinePrefix(const TextEditor& editor, const TextEditor::AutoCompleteState& state) {
  return editor.GetSectionText(static_cast<int>(state.line), 0, static_cast<int>(state.line), static_cast<int>(state.searchTermStartColumn));
}

bool autocompleteMemberContext(std::string_view linePrefix) {
  return !linePrefix.empty() && linePrefix.back() == '.';
}

struct TextEditorInternal::Impl {
  TextEditor editor;
  TextEditor::AutoCompleteConfig autocompleteConfig;
  AutoCompleteHandler autoCompleteHandler;

  Impl() {
    autocompleteConfig.triggerOnTyping = true;
    autocompleteConfig.triggerOnShortcut = true;
    autocompleteConfig.triggerInComments = false;
    autocompleteConfig.triggerInStrings = false;
    autocompleteConfig.triggerDelay = std::chrono::milliseconds{150};
    autocompleteConfig.callback = [this](TextEditor::AutoCompleteState& state) {
      state.suggestions.clear();
      if (!autoCompleteHandler) {
        return;
      }
      const auto linePrefix = autocompleteLinePrefix(editor, state);
      if (state.searchTerm.empty() && !autocompleteMemberContext(linePrefix)) {
        return;
      }
      autoCompleteHandler(state.suggestions, state.searchTerm, linePrefix);
    };
  }
};

TextEditorInternal::TextEditorInternal() : m_impl(std::make_unique<Impl>()) {}

TextEditorInternal::~TextEditorInternal() = default;

void TextEditorInternal::setupJavascript() {
  m_impl->editor.SetLanguage(javascriptLanguage());
  m_impl->editor.SetAutoCompleteConfig(&m_impl->autocompleteConfig);
}

void TextEditorInternal::setAutoCompleteHandler(AutoCompleteHandler handler) {
  m_impl->autoCompleteHandler = std::move(handler);
}

void TextEditorInternal::setText(const std::string& text) { m_impl->editor.SetText(text); }

std::string TextEditorInternal::getText() const { return m_impl->editor.GetText(); }

void TextEditorInternal::setReadOnlyEnabled(bool enabled) { m_impl->editor.SetReadOnlyEnabled(enabled); }

void TextEditorInternal::setShowLineNumbersEnabled(bool enabled) { m_impl->editor.SetShowLineNumbersEnabled(enabled); }

void TextEditorInternal::setFocus() { m_impl->editor.SetFocus(); }

void TextEditorInternal::replaceTextAtCursor(const std::string& text) { m_impl->editor.ReplaceTextInCurrentCursor(text); }

bool TextEditorInternal::canUndo() const { return m_impl->editor.CanUndo(); }

bool TextEditorInternal::canRedo() const { return m_impl->editor.CanRedo(); }

void TextEditorInternal::undo() { m_impl->editor.Undo(); }

void TextEditorInternal::redo() { m_impl->editor.Redo(); }

void TextEditorInternal::cut() { m_impl->editor.Cut(); }

void TextEditorInternal::copy() { m_impl->editor.Copy(); }

void TextEditorInternal::paste() { m_impl->editor.Paste(); }

void TextEditorInternal::openFindReplaceWindow() { m_impl->editor.OpenFindReplaceWindow(); }

void TextEditorInternal::toggleComments() { m_impl->editor.ToggleComments(); }

void TextEditorInternal::render(const char* id, ImVec2 size, bool border) { m_impl->editor.Render(id, size, border); }
