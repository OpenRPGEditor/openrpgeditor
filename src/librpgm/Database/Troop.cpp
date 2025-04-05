#include "Database/Troop.hpp"
#include "Database/CommandParser.hpp"

Troop::Member::Member(const Member& other)
: IModifiable(other)
, m_enemyId(other.m_enemyId)
, m_x(other.m_x)
, m_y(other.m_y)
, m_hidden(other.m_hidden)
, m_oldenemyId(other.m_oldenemyId)
, m_oldx(other.m_oldx)
, m_oldy(other.m_oldy)
, m_oldhidden(other.m_oldhidden) {}

Troop::Member& Troop::Member::operator=(const Member& other) {
  IModifiable::operator=(other);
  m_enemyId = other.m_enemyId;
  m_x = other.m_x;
  m_y = other.m_y;
  m_hidden = other.m_hidden;
  m_oldenemyId = other.m_oldenemyId;
  m_oldx = other.m_oldx;
  m_oldy = other.m_oldy;
  m_oldhidden = other.m_oldhidden;
  return *this;
}

Troop::Member::Member(Member&& other) noexcept
: IModifiable(std::move(other))
, m_enemyId(other.m_enemyId)
, m_x(other.m_x)
, m_y(other.m_y)
, m_hidden(other.m_hidden)
, m_oldenemyId(other.m_oldenemyId)
, m_oldx(other.m_oldx)
, m_oldy(other.m_oldy)
, m_oldhidden(other.m_oldhidden) {}

Troop::Member& Troop::Member::operator=(Member&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_enemyId = other.m_enemyId;
  m_x = other.m_x;
  m_y = other.m_y;
  m_hidden = other.m_hidden;
  m_oldenemyId = other.m_oldenemyId;
  m_oldx = other.m_oldx;
  m_oldy = other.m_oldy;
  m_oldhidden = other.m_oldhidden;
  return *this;
}

int Troop::Member::enemyId() const { return m_enemyId; }
void Troop::Member::setEnemyId(int value) {
  if (m_enemyId == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(enemyId);
  m_enemyId = value;
  if (!signalsDisabled()) {
    enemyIdModified().fire(this, value);
  }
  setHasChanges();
}

int Troop::Member::x() const { return m_x; }
void Troop::Member::setX(int value) {
  if (m_x == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(x);
  m_x = value;
  if (!signalsDisabled()) {
    xModified().fire(this, value);
  }
  setHasChanges();
}

int Troop::Member::y() const { return m_y; }
void Troop::Member::setY(int value) {
  if (m_y == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(y);
  m_y = value;
  if (!signalsDisabled()) {
    yModified().fire(this, value);
  }
  setHasChanges();
}

bool Troop::Member::hidden() const { return m_hidden; }
void Troop::Member::setHidden(bool value) {
  if (m_hidden == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(hidden);
  m_hidden = value;
  if (!signalsDisabled()) {
    hiddenModified().fire(this, value);
  }
  setHasChanges();
}

void Troop::Member::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(enemyId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(x);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(y);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(hidden);
}
void Troop::Member::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(enemyId);
  MODIFIABLE_ACCEPT_VALUE(x);
  MODIFIABLE_ACCEPT_VALUE(y);
  MODIFIABLE_ACCEPT_VALUE(hidden);
}
nlohmann::ordered_json Troop::Member::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(enemyId),
      MODIFIABLE_SERIALIZE_VALUE(x),
      MODIFIABLE_SERIALIZE_VALUE(y),
      MODIFIABLE_SERIALIZE_VALUE(hidden),
  };
}

rpgmutils::signal<void(Troop::Member*, int)>& Troop::Member::enemyIdModified() {
  if (!m_enemyIdModified) {
    m_enemyIdModified.emplace();
  }
  return *m_enemyIdModified;
}
rpgmutils::signal<void(Troop::Member*, int)>& Troop::Member::xModified() {
  if (!m_xModified) {
    m_xModified.emplace();
  }
  return *m_xModified;
}
rpgmutils::signal<void(Troop::Member*, int)>& Troop::Member::yModified() {
  if (!m_yModified) {
    m_yModified.emplace();
  }
  return *m_yModified;
}
rpgmutils::signal<void(Troop::Member*, bool)>& Troop::Member::hiddenModified() {
  if (!m_hiddenModified) {
    m_hiddenModified.emplace();
  }
  return *m_hiddenModified;
}

Troop::Conditions::Conditions(const Conditions& other)
: IModifiable(other)
, m_actorHp(other.m_actorHp)
, m_actorId(other.m_actorId)
, m_actorValid(other.m_actorValid)
, m_enemyIndex(other.m_enemyIndex)
, m_enemyHp(other.m_enemyHp)
, m_enemyValid(other.m_enemyValid)
, m_switchId(other.m_switchId)
, m_switchValid(other.m_switchValid)
, m_turnA(other.m_turnA)
, m_turnB(other.m_turnB)
, m_turnEnding(other.m_turnEnding)
, m_turnValid(other.m_turnValid)
, m_oldactorHp(other.m_oldactorHp)
, m_oldactorId(other.m_oldactorId)
, m_oldactorValid(other.m_oldactorValid)
, m_oldenemyIndex(other.m_oldenemyIndex)
, m_oldenemyHp(other.m_oldenemyHp)
, m_oldenemyValid(other.m_oldenemyValid)
, m_oldswitchId(other.m_oldswitchId)
, m_oldswitchValid(other.m_oldswitchValid)
, m_oldturnA(other.m_oldturnA)
, m_oldturnB(other.m_oldturnB)
, m_oldturnEnding(other.m_oldturnEnding)
, m_oldturnValid(other.m_oldturnValid) {}

Troop::Conditions& Troop::Conditions::operator=(const Conditions& other) {
  IModifiable::operator=(other);
  m_actorHp = other.m_actorHp;
  m_actorId = other.m_actorId;
  m_actorValid = other.m_actorValid;
  m_enemyIndex = other.m_enemyIndex;
  m_enemyHp = other.m_enemyHp;
  m_enemyValid = other.m_enemyValid;
  m_switchId = other.m_switchId;
  m_switchValid = other.m_switchValid;
  m_turnA = other.m_turnA;
  m_turnB = other.m_turnB;
  m_turnEnding = other.m_turnEnding;
  m_turnValid = other.m_turnValid;
  m_oldactorHp = other.m_oldactorHp;
  m_oldactorId = other.m_oldactorId;
  m_oldactorValid = other.m_oldactorValid;
  m_oldenemyIndex = other.m_oldenemyIndex;
  m_oldenemyHp = other.m_oldenemyHp;
  m_oldenemyValid = other.m_oldenemyValid;
  m_oldswitchId = other.m_oldswitchId;
  m_oldswitchValid = other.m_oldswitchValid;
  m_oldturnA = other.m_oldturnA;
  m_oldturnB = other.m_oldturnB;
  m_oldturnEnding = other.m_oldturnEnding;
  m_oldturnValid = other.m_oldturnValid;
  return *this;
}

Troop::Conditions::Conditions(Conditions&& other) noexcept
: IModifiable(std::move(other))
, m_actorHp(other.m_actorHp)
, m_actorId(other.m_actorId)
, m_actorValid(other.m_actorValid)
, m_enemyIndex(other.m_enemyIndex)
, m_enemyHp(other.m_enemyHp)
, m_enemyValid(other.m_enemyValid)
, m_switchId(other.m_switchId)
, m_switchValid(other.m_switchValid)
, m_turnA(other.m_turnA)
, m_turnB(other.m_turnB)
, m_turnEnding(other.m_turnEnding)
, m_turnValid(other.m_turnValid)
, m_oldactorHp(other.m_oldactorHp)
, m_oldactorId(other.m_oldactorId)
, m_oldactorValid(other.m_oldactorValid)
, m_oldenemyIndex(other.m_oldenemyIndex)
, m_oldenemyHp(other.m_oldenemyHp)
, m_oldenemyValid(other.m_oldenemyValid)
, m_oldswitchId(other.m_oldswitchId)
, m_oldswitchValid(other.m_oldswitchValid)
, m_oldturnA(other.m_oldturnA)
, m_oldturnB(other.m_oldturnB)
, m_oldturnEnding(other.m_oldturnEnding)
, m_oldturnValid(other.m_oldturnValid) {}
Troop::Conditions& Troop::Conditions::operator=(Conditions&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_actorHp = other.m_actorHp;
  m_actorId = other.m_actorId;
  m_actorValid = other.m_actorValid;
  m_enemyIndex = other.m_enemyIndex;
  m_enemyHp = other.m_enemyHp;
  m_enemyValid = other.m_enemyValid;
  m_switchId = other.m_switchId;
  m_switchValid = other.m_switchValid;
  m_turnA = other.m_turnA;
  m_turnB = other.m_turnB;
  m_turnEnding = other.m_turnEnding;
  m_turnValid = other.m_turnValid;
  m_oldactorHp = other.m_oldactorHp;
  m_oldactorId = other.m_oldactorId;
  m_oldactorValid = other.m_oldactorValid;
  m_oldenemyIndex = other.m_oldenemyIndex;
  m_oldenemyHp = other.m_oldenemyHp;
  m_oldenemyValid = other.m_oldenemyValid;
  m_oldswitchId = other.m_oldswitchId;
  m_oldswitchValid = other.m_oldswitchValid;
  m_oldturnA = other.m_oldturnA;
  m_oldturnB = other.m_oldturnB;
  m_oldturnEnding = other.m_oldturnEnding;
  m_oldturnValid = other.m_oldturnValid;
  return *this;
}

int Troop::Conditions::actorHp() const { return m_actorHp; }
void Troop::Conditions::setActorHp(int value) {
  if (m_actorHp == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(actorHp);
  m_actorHp = value;
  if (!signalsDisabled()) {
    actorHpModified().fire(this, value);
  }
  setHasChanges();
}

int Troop::Conditions::actorId() const { return m_actorId; }
void Troop::Conditions::setActorId(int value) {
  if (m_actorId == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(actorId);
  m_actorId = value;
  if (!signalsDisabled()) {
    actorIdModified().fire(this, value);
  }
  setHasChanges();
}

bool Troop::Conditions::actorValid() const { return m_actorValid; }
void Troop::Conditions::setActorValid(const bool value) {
  if (m_actorValid == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(actorValid);
}

int Troop::Conditions::enemyIndex() const { return m_enemyIndex; }
void Troop::Conditions::setEnemyIndex(const int value) {
  if (m_enemyIndex == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(enemyIndex);
  m_enemyIndex = value;
  if (!signalsDisabled()) {
    enemyIndexModified().fire(this, value);
  }
  setHasChanges();
}

int Troop::Conditions::enemyHp() const { return m_enemyHp; }
void Troop::Conditions::setEnemyHp(const int value) {
  if (m_enemyHp == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(enemyHp);
  m_enemyHp = value;
  if (!signalsDisabled()) {
    enemyHpModified().fire(this, value);
  }
  setHasChanges();
}

bool Troop::Conditions::enemyValid() const { return m_enemyValid; }
void Troop::Conditions::setEnemyValid(const bool value) {
  if (m_enemyValid == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(enemyValid);
  m_enemyValid = value;
  if (!signalsDisabled()) {
    enemyValidModified().fire(this, value);
  }
  setHasChanges();
}

int Troop::Conditions::switchId() const { return m_switchId; }
void Troop::Conditions::setSwitchId(const int value) {
  if (m_switchId == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(switchId);
  m_switchId = value;
  if (!signalsDisabled()) {
    switchIdModified().fire(this, value);
  }
  setHasChanges();
}

bool Troop::Conditions::switchValid() const { return m_switchValid; }
void Troop::Conditions::setSwitchValid(const bool value) {
  if (m_switchValid == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(switchValid);
  m_switchValid = value;
  if (!signalsDisabled()) {
    switchValidModified().fire(this, value);
  }
  setHasChanges();
}

int Troop::Conditions::turnA() const { return m_turnA; }
void Troop::Conditions::setTurnA(const int value) {
  if (m_turnA == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(turnA);
  m_turnA = value;
  if (!signalsDisabled()) {
    turnAModified().fire(this, value);
  }
  setHasChanges();
}

int Troop::Conditions::turnB() const { return m_turnB; }
void Troop::Conditions::setTurnB(const int value) {
  if (m_turnB == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(turnB);
  m_turnB = value;
  if (!signalsDisabled()) {
    turnBModified().fire(this, value);
  }
  setHasChanges();
}

bool Troop::Conditions::turnEnding() const { return m_turnEnding; }
void Troop::Conditions::setTurnEnding(const bool value) {
  if (m_turnEnding == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(turnEnding);
  m_turnEnding = value;
  if (!signalsDisabled()) {
    turnEndingModified().fire(this, value);
  }
  setHasChanges();
}

bool Troop::Conditions::turnValid() const { return m_turnValid; }
void Troop::Conditions::setTurnValid(const bool value) {
  if (m_turnValid == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(turnValid);
  m_turnValid = value;
  if (!signalsDisabled()) {
    turnValidModified().fire(this, value);
  }
  setHasChanges();
}

void Troop::Conditions::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(actorHp);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(actorId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(actorValid);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(enemyIndex);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(enemyHp);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(enemyValid);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(switchId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(switchValid);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(turnA);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(turnB);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(turnEnding);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(turnValid);
}
void Troop::Conditions::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(actorHp);
  MODIFIABLE_ACCEPT_VALUE(actorId);
  MODIFIABLE_ACCEPT_VALUE(actorValid);
  MODIFIABLE_ACCEPT_VALUE(enemyIndex);
  MODIFIABLE_ACCEPT_VALUE(enemyHp);
  MODIFIABLE_ACCEPT_VALUE(enemyValid);
  MODIFIABLE_ACCEPT_VALUE(switchId);
  MODIFIABLE_ACCEPT_VALUE(switchValid);
  MODIFIABLE_ACCEPT_VALUE(turnA);
  MODIFIABLE_ACCEPT_VALUE(turnB);
  MODIFIABLE_ACCEPT_VALUE(turnEnding);
  MODIFIABLE_ACCEPT_VALUE(turnValid);
}
nlohmann::ordered_json Troop::Conditions::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(actorHp), MODIFIABLE_SERIALIZE_VALUE(actorId),    MODIFIABLE_SERIALIZE_VALUE(actorValid), MODIFIABLE_SERIALIZE_VALUE(enemyIndex),
      MODIFIABLE_SERIALIZE_VALUE(enemyHp), MODIFIABLE_SERIALIZE_VALUE(enemyValid), MODIFIABLE_SERIALIZE_VALUE(switchId),   MODIFIABLE_SERIALIZE_VALUE(switchValid),
      MODIFIABLE_SERIALIZE_VALUE(turnA),   MODIFIABLE_SERIALIZE_VALUE(turnB),      MODIFIABLE_SERIALIZE_VALUE(turnEnding), MODIFIABLE_SERIALIZE_VALUE(turnValid),
  };
}

rpgmutils::signal<void(Troop::Conditions*, int)>& Troop::Conditions::actorHpModified() {
  if (!m_actorHpModified) {
    m_actorHpModified.emplace();
  }
  return *m_actorHpModified;
}
rpgmutils::signal<void(Troop::Conditions*, int)>& Troop::Conditions::actorIdModified() {
  if (!m_actorIdModified) {
    m_actorIdModified.emplace();
  }
  return *m_actorIdModified;
}
rpgmutils::signal<void(Troop::Conditions*, bool)>& Troop::Conditions::actorValidModified() {
  if (!m_actorValidModified) {
    m_actorValidModified.emplace();
  }
  return *m_actorValidModified;
}
rpgmutils::signal<void(Troop::Conditions*, int)>& Troop::Conditions::enemyIndexModified() {
  if (!m_enemyIndexModified) {
    m_enemyIndexModified.emplace();
  }
  return *m_enemyIndexModified;
}
rpgmutils::signal<void(Troop::Conditions*, int)>& Troop::Conditions::enemyHpModified() {
  if (!m_enemyHpModified) {
    m_enemyHpModified.emplace();
  }
  return *m_enemyHpModified;
}
rpgmutils::signal<void(Troop::Conditions*, bool)>& Troop::Conditions::enemyValidModified() {
  if (!m_enemyValidModified) {
    m_enemyValidModified.emplace();
  }
  return *m_enemyValidModified;
}
rpgmutils::signal<void(Troop::Conditions*, int)>& Troop::Conditions::switchIdModified() {
  if (!m_switchIdModified) {
    m_switchIdModified.emplace();
  }
  return *m_switchIdModified;
}
rpgmutils::signal<void(Troop::Conditions*, bool)>& Troop::Conditions::switchValidModified() {
  if (!m_switchValidModified) {
    m_switchValidModified.emplace();
  }
  return *m_switchValidModified;
}
rpgmutils::signal<void(Troop::Conditions*, int)>& Troop::Conditions::turnAModified() {
  if (!m_turnAModified) {
    m_turnAModified.emplace();
  }
  return *m_turnAModified;
}
rpgmutils::signal<void(Troop::Conditions*, int)>& Troop::Conditions::turnBModified() {
  if (!m_turnBModified) {
    m_turnBModified.emplace();
  }
  return *m_turnBModified;
}
rpgmutils::signal<void(Troop::Conditions*, bool)>& Troop::Conditions::turnEndingModified() {
  if (!m_turnEndingModified) {
    m_turnEndingModified.emplace();
  }
  return *m_turnEndingModified;
}
rpgmutils::signal<void(Troop::Conditions*, bool)>& Troop::Conditions::turnValidModified() {
  if (!m_turnValidModified) {
    m_turnValidModified.emplace();
  }
  return *m_turnValidModified;
}

Troop::Page::Page(const Page& other)
: IModifiable(other)
, m_conditions(other.m_conditions)
, m_list(other.m_list)
, m_span(other.m_span)
, m_oldconditions(other.m_oldconditions)
, m_oldlist(other.m_oldlist)
, m_oldspan(other.m_oldspan) {}
Troop::Page& Troop::Page::operator=(const Page& other) {
  IModifiable::operator=(other);
  m_conditions = other.m_conditions;
  m_list = other.m_list;
  m_span = other.m_span;
  m_oldconditions = other.m_oldconditions;
  m_oldlist = other.m_oldlist;
  m_oldspan = other.m_oldspan;
  return *this;
}
Troop::Page::Page(Page&& other) noexcept
: IModifiable(std::move(other))
, m_conditions(std::move(other.m_conditions))
, m_list(std::move(other.m_list))
, m_span(other.m_span)
, m_oldconditions(std::move(other.m_oldconditions))
, m_oldlist(std::move(other.m_oldlist))
, m_oldspan(other.m_oldspan) {}

Troop::Page& Troop::Page::operator=(Page&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_conditions = std::move(other.m_conditions);
  m_list = std::move(other.m_list);
  m_span = other.m_span;
  m_oldconditions = std::move(other.m_oldconditions);
  m_oldlist = std::move(other.m_oldlist);
  m_oldspan = other.m_oldspan;
  return *this;
}

Troop::Conditions& Troop::Page::conditions() { return m_conditions; }
const Troop::Conditions& Troop::Page::conditions() const { return m_conditions; }
void Troop::Page::setConditions(const Conditions& value) {
  if (m_conditions == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(conditions);
  m_conditions = value;
  if (!signalsDisabled()) {
    conditionsModified().fire(this, value);
  }
  setHasChanges();
}

std::vector<std::shared_ptr<IEventCommand>>& Troop::Page::list() { return m_list; }
const std::vector<std::shared_ptr<IEventCommand>>& Troop::Page::list() const { return m_list; }
void Troop::Page::setList(const std::vector<std::shared_ptr<IEventCommand>>& value) {
  if (m_list == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(list);
  m_list = value;
  if (!signalsDisabled()) {
    listModified().fire(this, value);
  }
  setHasChanges();
}

int Troop::Page::span() const { return m_span; }
void Troop::Page::setSpan(int value) {
  if (m_span == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(span);
  m_span = value;
  if (!signalsDisabled()) {
    spanModified().fire(this, value);
  }
  setHasChanges();
}

void Troop::Page::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(conditions);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(list);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(span);
  m_conditions.restoreOriginal();
  std::ranges::for_each(m_list, [&](const std::shared_ptr<IEventCommand>& command) { command->restoreOriginal(); });
}
void Troop::Page::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(conditions);
  MODIFIABLE_ACCEPT_VALUE(list);
  MODIFIABLE_ACCEPT_VALUE(span);
  m_conditions.acceptChanges();
  std::ranges::for_each(m_list, [&](const std::shared_ptr<IEventCommand>& command) { command->acceptChanges(); });
}

nlohmann::ordered_json Troop::Page::serializeOldValues() const {
  nlohmann::ordered_json j;
  j["conditions"] = m_oldconditions ? *m_oldconditions : m_conditions;
  nlohmann::ordered_json list;
  CommandParser::serialize(list, m_oldlist ? *m_oldlist : m_list, false, true);
  j["list"] = list;
  j["span"] = m_oldspan ? *m_oldspan : m_span;

  return j;
}

rpgmutils::signal<void(Troop::Page*, const Troop::Conditions&)>& Troop::Page::conditionsModified() {
  if (!m_conditionsModified) {
    m_conditionsModified.emplace();
  }
  return *m_conditionsModified;
}
rpgmutils::signal<void(Troop::Page*, const std::vector<std::shared_ptr<IEventCommand>>&)>& Troop::Page::listModified() {
  if (!m_listModified) {
    m_listModified.emplace();
  }
  return *m_listModified;
}
rpgmutils::signal<void(Troop::Page*, int)>& Troop::Page::spanModified() {
  if (!m_spanModified) {
    m_spanModified.emplace();
  }
  return *m_spanModified;
}

Troop::Troop(const Troop& other)
: IModifiable(other)
, m_id(other.m_id)
, m_members(other.m_members)
, m_name(other.m_name)
, m_pages(other.m_pages)
, m_oldid(other.m_oldid)
, m_oldmembers(other.m_oldmembers)
, m_oldname(other.m_oldname)
, m_oldpages(other.m_oldpages)
, m_isValid(other.m_isValid) {}

Troop& Troop::operator=(const Troop& other) {
  IModifiable::operator=(other);
  m_id = other.m_id;
  m_members = other.m_members;
  m_name = other.m_name;
  m_pages = other.m_pages;
  m_oldid = other.m_oldid;
  m_oldmembers = other.m_oldmembers;
  m_oldname = other.m_oldname;
  m_oldpages = other.m_oldpages;
  m_isValid = other.m_isValid;
  return *this;
}

Troop::Troop(Troop&& other) noexcept
: IModifiable(std::move(other))
, m_id(other.m_id)
, m_members(std::move(other.m_members))
, m_name(std::move(other.m_name))
, m_pages(std::move(other.m_pages))
, m_oldid(other.m_oldid)
, m_oldmembers(std::move(other.m_oldmembers))
, m_oldname(std::move(other.m_oldname))
, m_oldpages(std::move(other.m_oldpages))
, m_isValid(other.m_isValid) {}

Troop& Troop::operator=(Troop&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_id = other.m_id;
  m_members = std::move(other.m_members);
  m_name = std::move(other.m_name);
  m_pages = std::move(other.m_pages);
  m_oldid = other.m_oldid;
  m_oldmembers = std::move(other.m_oldmembers);
  m_oldname = std::move(other.m_oldname);
  m_oldpages = std::move(other.m_oldpages);
  m_isValid = other.m_isValid;
  return *this;
}

Troop::Member* Troop::member(const int index) {
  if (index < 0 || index > m_members.size()) {
    return nullptr;
  }

  return &m_members[index];
}

const Troop::Member* Troop::member(const int index) const {
  if (index < 0 || index > m_members.size()) {
    return nullptr;
  }

  return &m_members[index];
}

int Troop::id() const { return m_id; }
void Troop::setId(const int id) {
  if (m_id == id) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(id);
  m_id = id;
  if (!signalsDisabled()) {
    idModified().fire(this, id);
  }
  setHasChanges();
}
std::vector<Troop::Member>& Troop::members() { return m_members; }
const std::vector<Troop::Member>& Troop::members() const { return m_members; }
void Troop::setMembers(const std::vector<Member>& value) {
  if (m_members == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(members);
  m_members = value;
  if (!signalsDisabled()) {
    membersModified().fire(this, value);
  }
  setHasChanges();
}

const std::string& Troop::name() const { return m_name; }
void Troop::setName(const std::string& name) {
  if (m_name == name) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(name);
  m_name = name;
  if (!signalsDisabled()) {
    nameModified().fire(this, name);
  }
  setHasChanges();
}

std::vector<Troop::Page>& Troop::pages() { return m_pages; }
const std::vector<Troop::Page>& Troop::pages() const { return m_pages; }
void Troop::setPages(const std::vector<Page>& value) {
  if (m_pages == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(pages);
  m_pages = value;
  if (!signalsDisabled()) {
    pagesModified().fire(this, value);
  }
  setHasChanges();
}

void Troop::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(id);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(members);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(pages);
  std::ranges::for_each(m_members, [&](auto& member) { member.restoreOriginal(); });
  std::ranges::for_each(m_pages, [&](auto& page) { page.restoreOriginal(); });
}
void Troop::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(id);
  MODIFIABLE_ACCEPT_VALUE(members);
  MODIFIABLE_ACCEPT_VALUE(name);
  MODIFIABLE_ACCEPT_VALUE(pages);
  std::ranges::for_each(m_members, [&](auto& member) { member.acceptChanges(); });
  std::ranges::for_each(m_pages, [&](auto& page) { page.acceptChanges(); });
}

nlohmann::ordered_json Troop::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(id),
      MODIFIABLE_SERIALIZE_VALUE_CHILD_ARRAY(members),
      MODIFIABLE_SERIALIZE_VALUE(name),
      MODIFIABLE_SERIALIZE_VALUE_CHILD_ARRAY(pages),
  };
}

rpgmutils::signal<void(Troop*, int)>& Troop::idModified() {
  if (!m_idModified) {
    m_idModified.emplace();
  }
  return *m_idModified;
}

rpgmutils::signal<void(Troop*, const std::vector<Troop::Member>&)>& Troop::membersModified() {
  if (!m_membersModified) {
    m_membersModified.emplace();
  }
  return *m_membersModified;
}

rpgmutils::signal<void(Troop*, const std::string&)>& Troop::nameModified() {
  if (!m_nameModified) {
    m_nameModified.emplace();
  }
  return *m_nameModified;
}

rpgmutils::signal<void(Troop*, const std::vector<Troop::Page>&)>& Troop::pagesModified() {
  if (!m_pagesModified) {
    m_pagesModified.emplace();
  }
  return *m_pagesModified;
}

void to_json(nlohmann::ordered_json& j, const Troop::Conditions& c) {
  j = {
      {"actorHp", c.m_actorHp},   {"actorId", c.m_actorId},         {"actorValid", c.m_actorValid}, {"enemyHp", c.m_enemyHp}, {"enemyIndex", c.m_enemyIndex}, {"enemyValid", c.m_enemyValid},
      {"switchId", c.m_switchId}, {"switchValid", c.m_switchValid}, {"turnA", c.m_turnA},           {"turnB", c.m_turnB},     {"turnEnding", c.m_turnEnding}, {"turnValid", c.m_turnValid},
  };
}
void from_json(const nlohmann::ordered_json& j, Troop::Conditions& c) {
  c.m_actorHp = j.value("actorHp", c.m_actorHp);
  c.m_actorId = j.value("actorId", c.m_actorId);
  c.m_actorValid = j.value("actorValid", c.m_actorValid);
  c.m_enemyHp = j.value("enemyHp", c.m_enemyHp);
  c.m_enemyIndex = j.value("enemyIndex", c.m_enemyIndex);
  c.m_enemyValid = j.value("enemyValid", c.m_enemyValid);
  c.m_switchId = j.value("switchId", c.m_switchId);
  c.m_switchValid = j.value("switchValid", c.m_switchValid);
  c.m_turnA = j.value("turnA", c.m_turnA);
  c.m_turnB = j.value("turnB", c.m_turnB);
  c.m_turnEnding = j.value("turnEnding", c.m_turnEnding);
  c.m_turnValid = j.value("turnValid", c.m_turnValid);
}

void to_json(nlohmann::ordered_json& j, const Troop::Page& p) {
  j["conditions"] = p.m_conditions;
  nlohmann::ordered_json list;
  CommandParser::serialize(list, p.m_list);
  j["list"] = list;
  j["span"] = p.m_span;
}

void from_json(const nlohmann::ordered_json& j, Troop::Page& p) {
  p.m_conditions = j.value("conditions", p.m_conditions);
  CommandParser cmdParser;
  p.m_list = cmdParser.parse(j["list"]);
  p.m_span = j.value("span", p.m_span);
}

void to_json(nlohmann::ordered_json& j, const Troop::Member& m) {
  j = {
      {"enemyId", m.m_enemyId},
      {"x", m.m_x},
      {"y", m.m_y},
      {"hidden", m.m_hidden},
  };
}

void from_json(const nlohmann::ordered_json& j, Troop::Member& m) {
  m.m_enemyId = j.value("enemyId", m.m_enemyId);
  m.m_x = j.value("x", m.m_x);
  m.m_y = j.value("y", m.m_y);
  m.m_hidden = j.value("hidden", m.m_hidden);
}

void to_json(nlohmann::ordered_json& j, const Troop& t) {
  j = {
      {"id", t.m_id},
      {"members", t.m_members},
      {"name", t.m_name},
      {"pages", t.m_pages},
  };
}

void from_json(const nlohmann::ordered_json& j, Troop& t) {
  t.m_id = j.value("id", t.m_id);
  t.m_members = j.value("members", t.m_members);
  t.m_name = j.value("name", t.m_name);
  t.m_pages = j.value("pages", t.m_pages);
}