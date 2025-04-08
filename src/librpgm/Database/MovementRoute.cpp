#include "MovementRoute.hpp"
#include "Database/CommandParser.hpp"

MovementRoute::MovementRoute(const MovementRoute& other)
: IModifiable(other)
, m_list(other.m_list)
, m_repeat(other.m_repeat)
, m_skippable(other.m_skippable)
, m_wait(other.m_wait)
, m_oldlist(other.m_oldlist)
, m_oldrepeat(other.m_oldrepeat)
, m_oldskippable(other.m_oldskippable)
, m_oldwait(other.m_oldwait) {}

MovementRoute& MovementRoute::operator=(const MovementRoute& other) {
  IModifiable::operator=(other);
  m_list = other.m_list;
  m_repeat = other.m_repeat;
  m_skippable = other.m_skippable;
  m_wait = other.m_wait;
  m_oldlist = other.m_oldlist;
  m_oldrepeat = other.m_oldrepeat;
  m_oldskippable = other.m_oldskippable;
  m_oldwait = other.m_oldwait;
  return *this;
}
MovementRoute::MovementRoute(MovementRoute&& other) noexcept
: IModifiable(std::move(other))
, m_list(std::move(other.m_list))
, m_repeat(other.m_repeat)
, m_skippable(other.m_skippable)
, m_wait(other.m_wait)
, m_oldlist(std::move(other.m_oldlist))
, m_oldrepeat(other.m_oldrepeat)
, m_oldskippable(other.m_oldskippable)
, m_oldwait(other.m_oldwait) {}

MovementRoute& MovementRoute::operator=(MovementRoute&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_list = std::move(other.m_list);
  m_repeat = other.m_repeat;
  m_skippable = other.m_skippable;
  m_wait = other.m_wait;
  m_oldlist = std::move(other.m_oldlist);
  m_oldrepeat = other.m_oldrepeat;
  m_oldskippable = other.m_oldskippable;
  m_oldwait = other.m_oldwait;
  return *this;
}

std::vector<std::shared_ptr<IEventCommand>>& MovementRoute::list() { return m_list; }
const std::vector<std::shared_ptr<IEventCommand>>& MovementRoute::list() const { return m_list; }
void MovementRoute::setList(const std::vector<std::shared_ptr<IEventCommand>>& list) {
  if (m_list == list) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(list);
  m_list = list;
  if (!signalsDisabled()) {
    listModified().fire(this, list);
  }
  setModified();
}

bool MovementRoute::repeat() const { return m_repeat; }

void MovementRoute::setRepeat(bool repeat) {
  if (repeat == m_repeat) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(repeat);
  m_repeat = repeat;
  if (!signalsDisabled()) {
    repeatModified().fire(this, repeat);
  }
  setModified();
}

bool MovementRoute::skippable() const { return m_skippable; }

void MovementRoute::setSkippable(bool skippable) {
  if (skippable == m_skippable) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(skippable);
  m_skippable = skippable;
  if (!signalsDisabled()) {
    skippableModified().fire(this, skippable);
  }
  setModified();
}
bool MovementRoute::wait() const { return m_wait; }
void MovementRoute::setWait(bool wait) {
  if (wait == m_wait) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(wait);
  m_wait = wait;
  if (!signalsDisabled()) {
    waitModified().fire(this, wait);
  }
  setModified();
}

int MovementRoute::addCommand(const std::shared_ptr<IEventCommand>& command, int position) {
  MODIFIABLE_SET_OLD_VALUE(list);
  if (m_list.size() == 1) {
    position = 0;
  }
  if (position < 0)
    position = 0;
  auto select = m_list.insert(m_list.begin() + position, command);
  listModified().fire(this, m_list);
  setModified(true);
  return select - m_list.begin();
}

void MovementRoute::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(list);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(skippable);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(wait);
  for (const auto& e : m_list) {
    if (e) {
      e->restoreOriginal();
    }
  }
}
void MovementRoute::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(list);
  MODIFIABLE_ACCEPT_VALUE(skippable);
  MODIFIABLE_ACCEPT_VALUE(wait);
  for (const auto& e : m_list) {
    if (e) {
      e->acceptChanges();
    }
  }
}
nlohmann::ordered_json MovementRoute::serializeOldValues() const {
  nlohmann::ordered_json json;
  CommandParser::serialize(json["list"], m_oldlist ? *m_oldlist : m_list, true, true);
  json["repeat"] = m_repeat;
  json["skippable"] = m_skippable;
  json["wait"] = m_wait;
  return json;
}

rpgmutils::signal<void(MovementRoute*, const std::vector<std::shared_ptr<IEventCommand>>&)>& MovementRoute::listModified() {
  if (!m_listModified) {
    m_listModified.emplace();
  }
  return *m_listModified;
}

rpgmutils::signal<void(MovementRoute*, bool)>& MovementRoute::repeatModified() {
  if (!m_repeatModified) {
    m_repeatModified.emplace();
  }
  return *m_repeatModified;
}

rpgmutils::signal<void(MovementRoute*, bool)>& MovementRoute::skippableModified() {
  if (!m_skippableModified) {
    m_skippableModified.emplace();
  }
  return *m_skippableModified;
}

rpgmutils::signal<void(MovementRoute*, bool)>& MovementRoute::waitModified() {
  if (!m_waitModified) {
    m_waitModified.emplace();
  }
  return *m_waitModified;
}

void to_json(nlohmann::ordered_json& json, const MovementRoute& route) {
  CommandParser::serialize(json["list"], route.m_list, true);
  json["repeat"] = route.m_repeat;
  json["skippable"] = route.m_skippable;
  json["wait"] = route.m_wait;
}

void from_json(const nlohmann::ordered_json& json, MovementRoute& route) {
  CommandParser parser;
  route.m_list = parser.parse(json["list"]);
  route.m_repeat = json.value("repeat", route.m_repeat);
  route.m_skippable = json.value("skippable", route.m_skippable);
  route.m_wait = json.value("wait", route.m_wait);
}