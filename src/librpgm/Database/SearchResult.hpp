#pragma once
#include "Database/CommonEvent.hpp"
#include "Database/Event.hpp"
#include "Database/Globals.hpp"

#include <map>
#include <string>
#include <vector>

class SearchResult {
public:
  explicit SearchResult(int mapId, const std::shared_ptr<const IModifiable>& event, int pageIndex);
  explicit SearchResult(const std::optional<CommonEvent>* event, int step);
  explicit SearchResult(int mapId, int eventId, std::shared_ptr<IEventCommand>& cmd, int step);

  [[nodiscard]] std::shared_ptr<const Event> getEvent() const { return std::dynamic_pointer_cast<const Event>(m_event); }
  [[nodiscard]] int getCommonId() const { return m_commonEvent->value().id(); }
  [[nodiscard]] int getMapId() const { return m_mapId; }
  [[nodiscard]] int getTile() const { return 0; } // TODO
  [[nodiscard]] int getPage() const { return m_page; }
  [[nodiscard]] int getStep() const { return m_step; }

private:
  std::shared_ptr<const IModifiable> m_event;
  std::shared_ptr<const IEventCommand> m_command;
  const std::optional<CommonEvent>* m_commonEvent;

  int m_mapId{0};
  int m_commandEventId{0};
  int m_step{0};
  int m_page{0};
};
