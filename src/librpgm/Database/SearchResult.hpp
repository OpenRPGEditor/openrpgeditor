#pragma once
#include "Database/CommonEvent.hpp"
#include "Database/Database.hpp"
#include "Database/Event.hpp"
#include "Database/Globals.hpp"

#include <map>
#include <string>
#include <vector>

class SearchResult {
public:
  explicit SearchResult(int mapId, int eventId, int pageIndex);
  explicit SearchResult(int commonEventId, int step);
  explicit SearchResult(int mapId, int eventId, const std::shared_ptr<IEventCommand>& cmd, int pageIndex, int step);

  [[nodiscard]] Event& getEvent() const { return *Database::instance()->mapInfos.map(m_mapId)->event(m_eventId); }
  [[nodiscard]] CommonEvent& getCommonEvent() const { return *Database::instance()->commonEvents.event(m_commonEventId); }
  [[nodiscard]] int getEventId() const { return m_eventId; }
  [[nodiscard]] int getCommonEventId() const { return m_commonEventId; }
  [[nodiscard]] const std::shared_ptr<IEventCommand>& getCommand() const { return m_command; }
  [[nodiscard]] int getMapId() const { return m_mapId; }
  [[nodiscard]] int getTile() const { return 0; } // TODO
  [[nodiscard]] int getPage() const { return m_page; }
  [[nodiscard]] int getStep() const { return m_step; }

private:
  int m_eventId;
  std::shared_ptr<IEventCommand> m_command;
  int m_commonEventId;
  int m_mapId{0};
  int m_step{-1};
  int m_page{0};
};
