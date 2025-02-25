#include "SearchResult.hpp"

#include <utility>
SearchResult::SearchResult(int mapId, const Event& event, int pageIndex) : m_event(&event), m_command(nullptr), m_commonEvent(nullptr) {
  m_mapId = mapId;
  m_page = pageIndex;
}
SearchResult::SearchResult(const std::optional<CommonEvent>* event, int step) : m_event(nullptr), m_command(nullptr), m_commonEvent(event) { m_step = step; }
SearchResult::SearchResult(int mapId, const Event& event, const std::shared_ptr<IEventCommand>& cmd, const int step) : m_event(&event), m_command(cmd), m_commonEvent(nullptr) {
  m_mapId = mapId;
  m_step = step;
}
