#include "SearchResult.hpp"
SearchResult::SearchResult(const int mapId, const int eventId, const int pageIndex) {
  m_eventId = eventId;
  m_mapId = mapId;
  m_page = pageIndex;
}
SearchResult::SearchResult(const int commonEventId, const int step) {
  m_commonEventId = commonEventId;
  m_step = step;
}
SearchResult::SearchResult(const int mapId, const int eventId, const std::shared_ptr<IEventCommand>& cmd, const int pageIndex, const int step) : m_command(cmd) {
  m_eventId = eventId;
  m_mapId = mapId;
  m_page = pageIndex;
  m_step = step;
}
