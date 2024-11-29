#pragma once
#include "Core/TemplateEditor/IDBTemplates.hpp"
#include "Database/Event.hpp"
#include "Database/Map.hpp"

struct TemplatesEvent final : IDBTemplates {
  TemplatesEvent() = delete;
  explicit TemplatesEvent(Event* ev, Map* map, DatabaseEditor* parent, const int x, const int y) : IDBTemplates(parent), m_event(ev), m_map(map), m_tileX(x), m_tileY(y) {
    m_id = m_map->findOrMakeFreeId();
    m_eventName = m_event->name;
    m_eventNote = m_event->note;
    m_direction = static_cast<int>(m_event->pages.front().image.direction); // We assume that the character index for one page defines the rest.
                                                                            // This is mostly useful for ARPG templates where all the event pages will be facing one direction.
    // for (auto& page : m_event->pages) {
    //   page.image.characterIndex = 1;
    // }
  }
  bool hasChanges() override { return m_hasChanges; }
  void draw() override;
  DatabaseEditor* m_parent = nullptr;

private:
  Event* m_event;
  Map* m_map;

  std::string m_eventName;
  std::string m_eventNote;
  int m_direction;
  int m_id;

  int m_tileX;
  int m_tileY;

  bool m_hasChanges{false};
  bool m_open{true};
};