#include "SearchResult.hpp"

#include <utility>
SearchResult::SearchResult(int mapId, const std::shared_ptr<const IModifiable>& event, int pageIndex) {
  m_event = event;
  m_mapId = mapId;
  m_page = pageIndex;
}
SearchResult::SearchResult(const std::optional<CommonEvent>* event, int step) : m_commonEvent(event) { m_step = step; }
SearchResult::SearchResult(int mapId, int eventId, std::shared_ptr<IEventCommand>& cmd, const int step) : m_command(cmd) {
  m_mapId = mapId;
  m_commandEventId = eventId;
  m_step = step;
}
