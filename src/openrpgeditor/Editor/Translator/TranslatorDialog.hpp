#pragma once
#include "Editor/CommonUI/IDialogController.hpp"
#include "Editor/Translator/TranslationDocument.hpp"

class ConfirmationDialog : protected ITypedDialogController<ConfirmationDialog> {
public:
  enum class Buttons {
    YesNo,
    YesNoCancel,
    Ok,
    OkCancel,
  };

  enum class Result {
    Invalid = -1,
    Confirmed,
    Rejected,
    Cancelled,
  };

  ConfirmationDialog(const std::string_view title, const std::string_view message, const Buttons buttons = Buttons::YesNo)
  : m_title(title)
  , m_message(message)
  , m_buttons(buttons) {}

  void setTitle(const std::string_view title) { m_title = title; }
  void setMessage(const std::string_view message) { m_message = message; }
  void setButtons(const Buttons buttons) { m_buttons = buttons; }
  [[nodiscard]] Result result() const { return m_result; }

  bool show() {
    const auto [closed, _] = ITypedDialogController::draw();
    return closed;
  }

  std::tuple<bool, bool> draw() override;

  std::string m_title;
  std::string m_message;
  Buttons m_buttons;
  Result m_result{Result::Invalid};
};

class TranslatorDialog : public ITypedDialogController<TranslatorDialog> {
public:
  std::tuple<bool, bool> draw() override;

  bool closeAllDocuments(bool saveDocuments);

private:
  std::string m_currentLocale;
  std::shared_ptr<TranslationDocument> m_currentDocument;
  std::shared_ptr<TranslationDocument> m_highlightedDocument;
  std::optional<ConfirmationDialog> m_confirmationDialog;
  size_t m_currentTranslation = -1;
  size_t m_currentCheckFirst = -1;
  std::optional<ConfirmationDialog> m_removeCheckFirstConfirmationDialog;
};
