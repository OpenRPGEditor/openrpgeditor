#include "SearchResult.hpp"
SearchResult::SearchResult(int mapId, const std::shared_ptr<const IModifiable> event) : m_event(event) { m_mapId = mapId; }
SearchResult::SearchResult(const std::optional<CommonEvent>* event, int step) : m_commonEvent(event) { m_step = step; }
SearchResult::SearchResult(int mapId, int eventId, const IEventCommand* cmd, int step) : m_command(cmd) {
  m_mapId = mapId;
  m_commandEventId = eventId;
  m_step = step;
}
