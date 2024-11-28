#pragma once
#include "Core/TemplateEditor/IDBTemplates.hpp"
#include "Database/Event.hpp"
#include "Database/Templates.hpp"
#include "Database/Map.hpp"

struct TemplatesEventProperties : IDBTemplates {
  TemplatesEventProperties() = delete;
  explicit TemplatesEventProperties(Template* templ, DatabaseEditor* parent) : IDBTemplates(parent), m_currentTemplate(templ) {
    //m_id = m_event->id;
    //m_templateName = m_currentTemplate->name;
    //_templateName = m_currentTemplate->note;
    //m_eventNote = m_event->note;
    //m_characterIndex = m_event->pages.front().image.characterIndex; // We assume that the character index for one page defines the rest.
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
  Map* m_map;

  std::string m_eventName;
  std::string m_eventNote;
  int m_characterIndex;
  int m_id;
  Template* m_currentTemplate;

  bool m_hasChanges{false};
  bool m_open{true};

};