#include "SearchResult.hpp"
SearchResult::SearchResult(const int mapId, const Event& event, const int pageIndex) : m_event(&event) {
  m_mapId = mapId;
  m_page = pageIndex;
}
SearchResult::SearchResult(const std::optional<CommonEvent>* event, const int step) : m_event(nullptr), m_commonEvent(event) { m_step = step; }
SearchResult::SearchResult(int mapId, const Event& event, const std::shared_ptr<IEventCommand>& cmd, const int pageIndex, const int step) : m_event(&event), m_command(cmd) {

  m_mapId = mapId;
  m_page = pageIndex;
  m_step = step;
}
