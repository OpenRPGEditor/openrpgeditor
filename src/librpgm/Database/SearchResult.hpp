#pragma once
#include "Database/CommonEvent.hpp"
#include "Database/Event.hpp"
#include "Database/Globals.hpp"

#include <map>
#include <string>
#include <vector>

class SearchResult {
public:
  explicit SearchResult(int mapId, const Event& event, int pageIndex);
  explicit SearchResult(const std::optional<CommonEvent>* event, int step);
  explicit SearchResult(int mapId, const Event& event, const std::shared_ptr<IEventCommand>& cmd, int step);

  [[nodiscard]] const Event& getEvent() const { return *m_event; }
  [[nodiscard]] int getCommonId() const { return m_commonEvent->value().id(); }
  [[nodiscard]] const std::shared_ptr<IEventCommand>& getCommand() const { return m_command; }
  [[nodiscard]] int getMapId() const { return m_mapId; }
  [[nodiscard]] int getTile() const { return 0; } // TODO
  [[nodiscard]] int getPage() const { return m_page; }
  [[nodiscard]] int getStep() const { return m_step; }

private:
  const Event* m_event;
  std::shared_ptr<IEventCommand> m_command;
  const std::optional<CommonEvent>* m_commonEvent = nullptr;

  int m_mapId{0};
  int m_step{0};
  int m_page{0};
};
