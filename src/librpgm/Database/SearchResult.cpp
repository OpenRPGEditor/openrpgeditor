#include "SearchResult.hpp"
SearchResult::SearchResult(int commonId, int step) {
  m_commonEventId = commonId;
  m_step = step;
  m_resultSource = ResultSource::CommonEvent;
}
SearchResult::SearchResult(const int mapId, const int eventId, const int pageIndex) {
  m_eventId = eventId;
  m_mapId = mapId;
  m_page = pageIndex;
  m_resultSource = ResultSource::Event;
}
SearchResult::SearchResult(const int commonEventId, const std::shared_ptr<IEventCommand>& cmd, const int step)
: m_command(cmd) {
  m_commonEventId = commonEventId;
  m_step = step;
  m_resultSource = ResultSource::CommonEvent;
}
SearchResult::SearchResult(const int mapId, const int eventId, const std::shared_ptr<IEventCommand>& cmd, const int pageIndex, const int step)
: m_command(cmd) {
  m_eventId = eventId;
  m_mapId = mapId;
  m_page = pageIndex;
  m_step = step;
  m_resultSource = ResultSource::Event;
}
SearchResult::SearchResult(SpecialDataParent type, int parentId, int dataId) {
  m_type = type;
  m_parentId = parentId;
  m_dataId = dataId;
  m_resultSource = ResultSource::Special;
}
