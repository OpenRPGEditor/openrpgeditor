#pragma once
#include "Editor/CommonUI/IDialogController.hpp"
#include "TranslationDocument.hpp"

#include <vector>
class TranslatorDialog : public ITypedDialogController<TranslatorDialog> {
public:
  std::tuple<bool, bool> draw() override;

 
  bool closeAllDocuments(bool saveDocuments);
private:
  std::shared_ptr<TranslationDocument> m_currentDocument;
};
