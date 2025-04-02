#pragma once

#include "Core/EventEditor/EVPage.hpp"
#include "Core/TemplateEditor/Dialog/TemplatesEvent.hpp"
#include "Database/Event.hpp"
#include "Database/IEventEditor.hpp"

#include <vector>

struct MainWindow;

class EventEditor final : public IEventEditor {
public:
  bool isOpen() const override { return m_open; }
  void open() override { m_open = true; };
  bool confirmed() const override { return m_confirmed; }
  void accept() const override {
    if (m_event) {
      m_event->acceptChanges();
    }
  }
  int getSelectedPage() const override { return m_selectedPage; }
  std::tuple<bool, bool> draw() override;
  void drawLocalization();

protected:
  friend IEventEditor* IEventEditor::create(Event* ev);
  explicit EventEditor(Event* event) : IEventEditor(event) {}
  void eventPointerInvalidated() override {};

private:
  std::optional<ObjectPicker<Template>> m_templatePicker;

  std::string m_localizationInput;
  int m_localeLinesRequired{0};
  int m_maxLocaleLines{10};
  bool m_localeConfirm{false};
  bool m_choiceParsing{true};

  int m_selectedPage = 0;
  bool m_open{false};
  bool m_confirmed{false};
  bool m_doSwap{false};
  int m_swapTarget{-1};
  int m_swapDestination{-1};

};
