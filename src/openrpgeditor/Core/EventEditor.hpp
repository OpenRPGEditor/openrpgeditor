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

  std::tuple<bool, bool> draw() override;

protected:
  friend IEventEditor* IEventEditor::create(Event* ev);
  explicit EventEditor(Event* event) : IEventEditor(event) {}

private:
  std::optional<ObjectPicker<Template>> m_templatePicker;
  int m_selectedPage = 0;
  bool m_open{false};
  bool m_confirmed{false};
};
