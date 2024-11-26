#pragma once
#include "Core/TemplateEditor/IDBTemplates.hpp"
#include "Database/Event.hpp"
#include "Database/Templates.hpp"

struct TemplatesEvent : IDBTemplates {
  TemplatesEvent() = delete;
  explicit TemplatesEvent(Event* ev, DatabaseEditor* parent) : IDBTemplates(parent), m_event(ev) {
    m_eventName = m_event->name;
    m_characterIndex = m_event->pages.front().image.characterIndex; // We assume that the character index for one page defines the rest.
                                                                    // This is mostly useful for ARPG templates where all the event pages will be facing one direction.
    //for (auto& page : m_event->pages) {
    //  page.image.characterIndex = 1;
    //}
  }
  bool hasChanges() override {
    return m_hasChanges;
  }
  void draw() override;
  DatabaseEditor* m_parent = nullptr;

private:
  Event* m_event;

  std::string m_eventName;
  int m_characterIndex;

  bool m_hasChanges{false};
  bool m_open{true};

};