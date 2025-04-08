#include "Database/State.hpp"

State::State(const State& other)
: IModifiable(other)
, m_id(other.m_id)
, m_autoRemovalTiming(other.m_autoRemovalTiming)
, m_iconIndex(other.m_iconIndex)
, m_minTurns(other.m_minTurns)
, m_maxTurns(other.m_maxTurns)
, m_message1(other.m_message1)
, m_message2(other.m_message2)
, m_message3(other.m_message3)
, m_message4(other.m_message4)
, m_motion(other.m_motion)
, m_name(other.m_name)
, m_note(other.m_note)
, m_overlay(other.m_overlay)
, m_priority(other.m_priority)
, m_releaseByDamage(other.m_releaseByDamage)
, m_removeAtBattleEnd(other.m_removeAtBattleEnd)
, m_removeByDamage(other.m_removeByDamage)
, m_removeByRestriction(other.m_removeByRestriction)
, m_removeByWalking(other.m_removeByWalking)
, m_restriction(other.m_restriction)
, m_stepsToRemove(other.m_stepsToRemove)
, m_traits(other.m_traits)
, m_oldid(other.m_oldid)
, m_oldautoRemovalTiming(other.m_oldautoRemovalTiming)
, m_oldiconIndex(other.m_oldiconIndex)
, m_oldminTurns(other.m_oldminTurns)
, m_oldmaxTurns(other.m_oldmaxTurns)
, m_oldmessage1(other.m_oldmessage1)
, m_oldmessage2(other.m_oldmessage2)
, m_oldmessage3(other.m_oldmessage3)
, m_oldmessage4(other.m_oldmessage4)
, m_oldmotion(other.m_oldmotion)
, m_oldname(other.m_oldname)
, m_oldnote(other.m_oldnote)
, m_oldoverlay(other.m_oldoverlay)
, m_oldpriority(other.m_oldpriority)
, m_oldreleaseByDamage(other.m_oldreleaseByDamage)
, m_oldremoveAtBattleEnd(other.m_oldremoveAtBattleEnd)
, m_oldremoveByDamage(other.m_oldremoveByDamage)
, m_oldremoveByRestriction(other.m_oldremoveByRestriction)
, m_oldremoveByWalking(other.m_oldremoveByWalking)
, m_oldrestriction(other.m_oldrestriction)
, m_oldstepsToRemove(other.m_oldstepsToRemove)
, m_oldtraits(other.m_oldtraits)
, m_isValid(other.m_isValid) {}
State& State::operator=(const State& other) {
  IModifiable::operator=(other);
  m_id = other.m_id;
  m_autoRemovalTiming = other.m_autoRemovalTiming;
  m_iconIndex = other.m_iconIndex;
  m_minTurns = other.m_minTurns;
  m_maxTurns = other.m_maxTurns;
  m_message1 = other.m_message1;
  m_message2 = other.m_message2;
  m_message3 = other.m_message3;
  m_message4 = other.m_message4;
  m_motion = other.m_motion;
  m_name = other.m_name;
  m_note = other.m_note;
  m_overlay = other.m_overlay;
  m_priority = other.m_priority;
  m_releaseByDamage = other.m_releaseByDamage;
  m_removeAtBattleEnd = other.m_removeAtBattleEnd;
  m_removeByDamage = other.m_removeByDamage;
  m_removeByRestriction = other.m_removeByRestriction;
  m_removeByWalking = other.m_removeByWalking;
  m_restriction = other.m_restriction;
  m_stepsToRemove = other.m_stepsToRemove;
  m_traits = other.m_traits;
  m_oldid = other.m_oldid;
  m_oldautoRemovalTiming = other.m_oldautoRemovalTiming;
  m_oldiconIndex = other.m_oldiconIndex;
  m_oldminTurns = other.m_oldminTurns;
  m_oldmaxTurns = other.m_oldmaxTurns;
  m_oldmessage1 = other.m_oldmessage1;
  m_oldmessage2 = other.m_oldmessage2;
  m_oldmessage3 = other.m_oldmessage3;
  m_oldmessage4 = other.m_oldmessage4;
  m_oldmotion = other.m_oldmotion;
  m_oldname = other.m_oldname;
  m_oldnote = other.m_oldnote;
  m_oldoverlay = other.m_oldoverlay;
  m_oldpriority = other.m_oldpriority;
  m_oldreleaseByDamage = other.m_oldreleaseByDamage;
  m_oldremoveAtBattleEnd = other.m_oldremoveAtBattleEnd;
  m_oldremoveByDamage = other.m_oldremoveByDamage;
  m_oldremoveByRestriction = other.m_oldremoveByRestriction;
  m_oldremoveByWalking = other.m_oldremoveByWalking;
  m_oldrestriction = other.m_oldrestriction;
  m_oldstepsToRemove = other.m_oldstepsToRemove;
  m_oldtraits = other.m_oldtraits;
  m_isValid = other.m_isValid;
  return *this;
}
State::State(State&& other) noexcept
: IModifiable(std::move(other))
, m_id(other.m_id)
, m_autoRemovalTiming(other.m_autoRemovalTiming)
, m_iconIndex(other.m_iconIndex)
, m_minTurns(other.m_minTurns)
, m_maxTurns(other.m_maxTurns)
, m_message1(std::move(other.m_message1))
, m_message2(std::move(other.m_message2))
, m_message3(std::move(other.m_message3))
, m_message4(std::move(other.m_message4))
, m_motion(other.m_motion)
, m_name(std::move(other.m_name))
, m_note(std::move(other.m_note))
, m_overlay(other.m_overlay)
, m_priority(other.m_priority)
, m_releaseByDamage(other.m_releaseByDamage)
, m_removeAtBattleEnd(other.m_removeAtBattleEnd)
, m_removeByDamage(other.m_removeByDamage)
, m_removeByRestriction(other.m_removeByRestriction)
, m_removeByWalking(other.m_removeByWalking)
, m_restriction(other.m_restriction)
, m_stepsToRemove(other.m_stepsToRemove)
, m_traits(std::move(other.m_traits))
, m_oldid(other.m_oldid)
, m_oldautoRemovalTiming(other.m_oldautoRemovalTiming)
, m_oldiconIndex(other.m_oldiconIndex)
, m_oldminTurns(other.m_oldminTurns)
, m_oldmaxTurns(other.m_oldmaxTurns)
, m_oldmessage1(std::move(other.m_oldmessage1))
, m_oldmessage2(std::move(other.m_oldmessage2))
, m_oldmessage3(std::move(other.m_oldmessage3))
, m_oldmessage4(std::move(other.m_oldmessage4))
, m_oldmotion(other.m_oldmotion)
, m_oldname(std::move(other.m_oldname))
, m_oldnote(std::move(other.m_oldnote))
, m_oldoverlay(other.m_oldoverlay)
, m_oldpriority(other.m_oldpriority)
, m_oldreleaseByDamage(other.m_oldreleaseByDamage)
, m_oldremoveAtBattleEnd(other.m_oldremoveAtBattleEnd)
, m_oldremoveByDamage(other.m_oldremoveByDamage)
, m_oldremoveByRestriction(other.m_oldremoveByRestriction)
, m_oldremoveByWalking(other.m_oldremoveByWalking)
, m_oldrestriction(other.m_oldrestriction)
, m_oldstepsToRemove(other.m_oldstepsToRemove)
, m_oldtraits(std::move(other.m_oldtraits))
, m_isValid(other.m_isValid) {}
State& State::operator=(State&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_id = other.m_id;
  m_autoRemovalTiming = other.m_autoRemovalTiming;
  m_iconIndex = other.m_iconIndex;
  m_minTurns = other.m_minTurns;
  m_maxTurns = other.m_maxTurns;
  m_message1 = std::move(other.m_message1);
  m_message2 = std::move(other.m_message2);
  m_message3 = std::move(other.m_message3);
  m_message4 = std::move(other.m_message4);
  m_motion = other.m_motion;
  m_name = std::move(other.m_name);
  m_note = std::move(other.m_note);
  m_overlay = other.m_overlay;
  m_priority = other.m_priority;
  m_releaseByDamage = other.m_releaseByDamage;
  m_removeAtBattleEnd = other.m_removeAtBattleEnd;
  m_removeByDamage = other.m_removeByDamage;
  m_removeByRestriction = other.m_removeByRestriction;
  m_removeByWalking = other.m_removeByWalking;
  m_restriction = other.m_restriction;
  m_stepsToRemove = other.m_stepsToRemove;
  m_traits = std::move(other.m_traits);
  m_oldid = other.m_oldid;
  m_oldautoRemovalTiming = other.m_oldautoRemovalTiming;
  m_oldiconIndex = other.m_oldiconIndex;
  m_oldminTurns = other.m_oldminTurns;
  m_oldmaxTurns = other.m_oldmaxTurns;
  m_oldmessage1 = std::move(other.m_oldmessage1);
  m_oldmessage2 = std::move(other.m_oldmessage2);
  m_oldmessage3 = std::move(other.m_oldmessage3);
  m_oldmessage4 = std::move(other.m_oldmessage4);
  m_oldmotion = other.m_oldmotion;
  m_oldname = std::move(other.m_oldname);
  m_oldnote = std::move(other.m_oldnote);
  m_oldoverlay = other.m_oldoverlay;
  m_oldpriority = other.m_oldpriority;
  m_oldreleaseByDamage = other.m_oldreleaseByDamage;
  m_oldremoveAtBattleEnd = other.m_oldremoveAtBattleEnd;
  m_oldremoveByDamage = other.m_oldremoveByDamage;
  m_oldremoveByRestriction = other.m_oldremoveByRestriction;
  m_oldremoveByWalking = other.m_oldremoveByWalking;
  m_oldrestriction = other.m_oldrestriction;
  m_oldstepsToRemove = other.m_oldstepsToRemove;
  m_oldtraits = std::move(other.m_oldtraits);
  m_isValid = other.m_isValid;
  return *this;
}

[[nodiscard]] int State::id() const { return m_id; }
void State::setId(int id) {
  if (m_id == id) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(id);
  m_id = id;
  if (!signalsDisabled()) {
    idModified().fire(this, id);
  }
  setModified();
}

[[nodiscard]] AutoRemovalTiming State::autoRemovalTiming() const { return m_autoRemovalTiming; }
void State::setAutoRemovalTiming(AutoRemovalTiming autoRemovalTiming) {
  if (m_autoRemovalTiming == autoRemovalTiming) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(autoRemovalTiming);
  m_autoRemovalTiming = autoRemovalTiming;
  if (!signalsDisabled()) {
    autoRemovalTimingModified().fire(this, autoRemovalTiming);
  }
  setModified();
}

[[nodiscard]] int State::chanceByDamage() const { return m_chanceByDamage; }
void State::setChanceByDamage(int chanceByDamage) {
  if (m_chanceByDamage == chanceByDamage) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(chanceByDamage);
  m_chanceByDamage = chanceByDamage;
  if (!signalsDisabled()) {
    chanceByDamageModified().fire(this, chanceByDamage);
  }
  setModified();
}

[[nodiscard]] int State::iconIndex() const { return m_iconIndex; }
void State::setIconIndex(int iconIndex) {
  if (m_iconIndex == iconIndex) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(iconIndex);
  m_iconIndex = iconIndex;
  if (!signalsDisabled()) {
    iconIndexModified().fire(this, iconIndex);
  }
  setModified();
}

[[nodiscard]] int State::minTurns() const { return m_minTurns; }
void State::setMinTurns(int minTurns) {
  if (m_minTurns == minTurns) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(minTurns);
  m_minTurns = minTurns;
  if (!signalsDisabled()) {
    minTurnsModified().fire(this, m_minTurns);
  }
  setModified();
}

[[nodiscard]] int State::maxTurns() const { return m_maxTurns; }
void State::setMaxTurns(int maxTurns) {
  if (m_maxTurns == maxTurns) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(maxTurns);
  m_maxTurns = maxTurns;
  if (!signalsDisabled()) {
    maxTurnsModified().fire(this, m_maxTurns);
  }
  setModified();
}

[[nodiscard]] const std::string& State::message1() const { return m_message1; }
void State::setMessage1(const std::string& message1) {
  if (m_message1 == message1) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(message1);
  m_message1 = message1;
  if (!signalsDisabled()) {
    message1Modified().fire(this, m_message1);
  }
  setModified();
}

[[nodiscard]] const std::string& State::message2() const { return m_message2; }
void State::setMessage2(const std::string& message2) {
  if (m_message2 == message2) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(message2);
  m_message2 = message2;
  if (!signalsDisabled()) {
    message2Modified().fire(this, m_message2);
  }
  setModified();
}
[[nodiscard]] const std::string& State::message3() const { return m_message3; }
void State::setMessage3(const std::string& message3) {
  if (m_message3 == message3) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(message3);
  m_message3 = message3;
  if (!signalsDisabled()) {
    message3Modified().fire(this, m_message3);
  }
  setModified();
}

[[nodiscard]] const std::string& State::message4() const { return m_message4; }
void State::setMessage4(const std::string& message4) {
  if (m_message4 == message4) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(message4);
  m_message4 = message4;
  if (!signalsDisabled()) {
    message4Modified().fire(this, m_message4);
  }
  setModified();
}

[[nodiscard]] StateMotion State::motion() const { return m_motion; }
void State::setMotion(StateMotion motion) {
  if (m_motion == motion) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(motion);
  m_motion = motion;
  if (!signalsDisabled()) {
    motionModified().fire(this, m_motion);
  }
  setModified();
}

[[nodiscard]] const std::string& State::name() const { return m_name; }
void State::setName(const std::string& name) {
  if (m_name == name) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(name);
  m_name = name;
  if (!signalsDisabled()) {
    nameModified().fire(this, m_name);
  }
  setModified();
}

[[nodiscard]] const std::string& State::note() const { return m_note; }
void State::setNote(const std::string& note) {
  if (m_note == note) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(note);
  m_note = note;
  if (!signalsDisabled()) {
    noteModified().fire(this, m_note);
  }
  setModified();
}

[[nodiscard]] StateOverlay State::overlay() const { return m_overlay; }
void State::setOverlay(StateOverlay overlay) {
  if (m_overlay == overlay) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(overlay);
  m_overlay = overlay;
  if (!signalsDisabled()) {
    overlayModified().fire(this, m_overlay);
  }
  setModified();
}

[[nodiscard]] int State::priority() const { return m_priority; }
void State::setPriority(int priority) {
  if (m_priority == priority) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(priority);
  m_priority = priority;
  if (!signalsDisabled()) {
    priorityModified().fire(this, m_priority);
  }
  setModified();
}

[[nodiscard]] bool State::releaseByDamage() const { return m_releaseByDamage; }
void State::setReleaseByDamage(bool releaseByDamage) {
  if (m_releaseByDamage == releaseByDamage) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(releaseByDamage);
  m_releaseByDamage = releaseByDamage;
  if (!signalsDisabled()) {
    releaseByDamageModified().fire(this, m_releaseByDamage);
  }
  setModified();
}

[[nodiscard]] bool State::removeAtBattleEnd() const { return m_removeAtBattleEnd; }
void State::setRemoveAtBattleEnd(bool removeAtBattleEnd) {
  if (m_removeAtBattleEnd == removeAtBattleEnd) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(removeAtBattleEnd);
  m_removeAtBattleEnd = removeAtBattleEnd;
  if (!signalsDisabled()) {
    removeAtBattleEndModified().fire(this, m_removeAtBattleEnd);
  }
  setModified();
}

[[nodiscard]] bool State::removeByDamage() const { return m_removeByDamage; }
void State::setRemoveByDamage(bool removeByDamage) {
  if (m_removeByDamage == removeByDamage) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(removeByDamage);
  m_removeByDamage = removeByDamage;
  if (!signalsDisabled()) {
    removeByDamageModified().fire(this, m_removeByDamage);
  }
  setModified();
}

[[nodiscard]] bool State::removeByRestriction() const { return m_removeByRestriction; }
void State::setRemoveByRestriction(bool removeByRestriction) {
  if (m_removeByRestriction == removeByRestriction) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(removeByRestriction);
  m_removeByRestriction = removeByRestriction;
  if (!signalsDisabled()) {
    removeByRestrictionModified().fire(this, m_removeByRestriction);
  }
  setModified();
}

[[nodiscard]] bool State::removeByWalking() const { return m_removeByWalking; }
void State::setRemoveByWalking(bool removeByWalking) {
  if (m_removeByWalking == removeByWalking) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(removeByWalking);
  m_removeByWalking = removeByWalking;
  if (!signalsDisabled()) {
    removeByWalkingModified().fire(this, m_removeByWalking);
  }
  setModified();
}

[[nodiscard]] StateRestriction State::restriction() const { return m_restriction; }
void State::setRestriction(StateRestriction restriction) {
  if (m_restriction == restriction) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(restriction);
  m_restriction = restriction;
  if (!signalsDisabled()) {
    restrictionModified().fire(this, m_restriction);
  }
  setModified();
}

[[nodiscard]] int State::stepsToRemove() const { return m_stepsToRemove; }
void State::setStepsToRemove(int stepsToRemove) {
  if (m_stepsToRemove == stepsToRemove) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(stepsToRemove);
  m_stepsToRemove = stepsToRemove;
  if (!signalsDisabled()) {
    stepsToRemoveModified().fire(this, m_stepsToRemove);
  }
  setModified();
}

[[nodiscard]] std::vector<Trait>& State::traits() { return m_traits; }
void State::setTraits(const std::vector<Trait>& traits) {
  if (m_traits == traits) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(traits);
  m_traits = traits;
  if (!signalsDisabled()) {
    traitsModified().fire(this, m_traits);
  }
  setModified();
}

void State::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(id);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(autoRemovalTiming);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(chanceByDamage);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(iconIndex);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(minTurns);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(maxTurns);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(message1);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(message2);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(message3);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(message4);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(motion);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(note);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(overlay);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(priority);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(releaseByDamage);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(removeAtBattleEnd);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(removeByDamage);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(removeByRestriction);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(removeByWalking);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(restriction);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(stepsToRemove);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(traits);

  for (auto& trait : m_traits) {
    trait.restoreOriginal();
  }
}
void State::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(id);
  MODIFIABLE_ACCEPT_VALUE(autoRemovalTiming);
  MODIFIABLE_ACCEPT_VALUE(chanceByDamage);
  MODIFIABLE_ACCEPT_VALUE(iconIndex);
  MODIFIABLE_ACCEPT_VALUE(minTurns);
  MODIFIABLE_ACCEPT_VALUE(maxTurns);
  MODIFIABLE_ACCEPT_VALUE(message1);
  MODIFIABLE_ACCEPT_VALUE(message2);
  MODIFIABLE_ACCEPT_VALUE(message3);
  MODIFIABLE_ACCEPT_VALUE(message4);
  MODIFIABLE_ACCEPT_VALUE(motion);
  MODIFIABLE_ACCEPT_VALUE(name);
  MODIFIABLE_ACCEPT_VALUE(note);
  MODIFIABLE_ACCEPT_VALUE(overlay);
  MODIFIABLE_ACCEPT_VALUE(priority);
  MODIFIABLE_ACCEPT_VALUE(releaseByDamage);
  MODIFIABLE_ACCEPT_VALUE(removeAtBattleEnd);
  MODIFIABLE_ACCEPT_VALUE(removeByDamage);
  MODIFIABLE_ACCEPT_VALUE(removeByRestriction);
  MODIFIABLE_ACCEPT_VALUE(removeByWalking);
  MODIFIABLE_ACCEPT_VALUE(restriction);
  MODIFIABLE_ACCEPT_VALUE(stepsToRemove);
  MODIFIABLE_ACCEPT_VALUE(traits);

  for (auto& trait : m_traits) {
    trait.acceptChanges();
  }
}

nlohmann::ordered_json State::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(id),
      MODIFIABLE_SERIALIZE_VALUE(autoRemovalTiming),
      MODIFIABLE_SERIALIZE_VALUE(chanceByDamage),
      MODIFIABLE_SERIALIZE_VALUE(iconIndex),
      MODIFIABLE_SERIALIZE_VALUE(minTurns),
      MODIFIABLE_SERIALIZE_VALUE(maxTurns),
      MODIFIABLE_SERIALIZE_VALUE(message1),
      MODIFIABLE_SERIALIZE_VALUE(message2),
      MODIFIABLE_SERIALIZE_VALUE(message3),
      MODIFIABLE_SERIALIZE_VALUE(message4),
      MODIFIABLE_SERIALIZE_VALUE(motion),
      MODIFIABLE_SERIALIZE_VALUE(name),
      MODIFIABLE_SERIALIZE_VALUE(note),
      MODIFIABLE_SERIALIZE_VALUE(overlay),
      MODIFIABLE_SERIALIZE_VALUE(priority),
      MODIFIABLE_SERIALIZE_VALUE(releaseByDamage),
      MODIFIABLE_SERIALIZE_VALUE(removeAtBattleEnd),
      MODIFIABLE_SERIALIZE_VALUE(removeByDamage),
      MODIFIABLE_SERIALIZE_VALUE(removeByRestriction),
      MODIFIABLE_SERIALIZE_VALUE(removeByWalking),
      MODIFIABLE_SERIALIZE_VALUE(restriction),
      MODIFIABLE_SERIALIZE_VALUE(stepsToRemove),
      MODIFIABLE_SERIALIZE_VALUE_CHILD_ARRAY(traits),
  };
}
rpgmutils::signal<void(State*, int)>& State::idModified() {
  if (!m_idModified) {
    m_idModified.emplace();
  }
  return *m_idModified;
}
rpgmutils::signal<void(State*, AutoRemovalTiming)>& State::autoRemovalTimingModified() {
  if (!m_autoRemovalTimingModified) {
    m_autoRemovalTimingModified.emplace();
  }
  return *m_autoRemovalTimingModified;
}
rpgmutils::signal<void(State*, int)>& State::chanceByDamageModified() {
  if (!m_chanceByDamageModified) {
    m_chanceByDamageModified.emplace();
  }
  return *m_chanceByDamageModified;
}
rpgmutils::signal<void(State*, int)>& State::iconIndexModified() {
  if (!m_iconIndexModified) {
    m_iconIndexModified.emplace();
  }
  return *m_iconIndexModified;
}
rpgmutils::signal<void(State*, int)>& State::minTurnsModified() {
  if (!m_minTurnsModified) {
    m_minTurnsModified.emplace();
  }
  return *m_minTurnsModified;
}
rpgmutils::signal<void(State*, int)>& State::maxTurnsModified() {
  if (!m_maxTurnsModified) {
    m_maxTurnsModified.emplace();
  }
  return *m_maxTurnsModified;
}
rpgmutils::signal<void(State*, const std::string&)>& State::message1Modified() {
  if (!m_message1Modified) {
    m_message1Modified.emplace();
  }
  return *m_message1Modified;
}
rpgmutils::signal<void(State*, const std::string&)>& State::message2Modified() {
  if (!m_message2Modified) {
    m_message2Modified.emplace();
  }
  return *m_message2Modified;
}
rpgmutils::signal<void(State*, const std::string&)>& State::message3Modified() {
  if (!m_message3Modified) {
    m_message3Modified.emplace();
  }
  return *m_message3Modified;
}
rpgmutils::signal<void(State*, const std::string&)>& State::message4Modified() {
  if (!m_message4Modified) {
    m_message4Modified.emplace();
  }
  return *m_message4Modified;
}
rpgmutils::signal<void(State*, StateMotion)>& State::motionModified() {
  if (!m_motionModified) {
    m_motionModified.emplace();
  }
  return *m_motionModified;
}
rpgmutils::signal<void(State*, const std::string&)>& State::nameModified() {
  if (!m_nameModified) {
    m_nameModified.emplace();
  }
  return *m_nameModified;
}
rpgmutils::signal<void(State*, const std::string&)>& State::noteModified() {
  if (!m_noteModified) {
    m_noteModified.emplace();
  }
  return *m_noteModified;
}
rpgmutils::signal<void(State*, StateOverlay)>& State::overlayModified() {
  if (!m_overlayModified) {
    m_overlayModified.emplace();
  }
  return *m_overlayModified;
}
rpgmutils::signal<void(State*, int)>& State::priorityModified() {
  if (!m_priorityModified) {
    m_priorityModified.emplace();
  }
  return *m_priorityModified;
}
rpgmutils::signal<void(State*, bool)>& State::releaseByDamageModified() {
  if (!m_releaseByDamageModified) {
    m_releaseByDamageModified.emplace();
  }
  return *m_releaseByDamageModified;
}
rpgmutils::signal<void(State*, bool)>& State::removeAtBattleEndModified() {
  if (!m_removeAtBattleEndModified) {
    m_removeAtBattleEndModified.emplace();
  }
  return *m_removeAtBattleEndModified;
}
rpgmutils::signal<void(State*, bool)>& State::removeByDamageModified() {
  if (!m_removeByDamageModified) {
    m_removeByDamageModified.emplace();
  }
  return *m_removeByDamageModified;
}
rpgmutils::signal<void(State*, bool)>& State::removeByRestrictionModified() {
  if (!m_removeByRestrictionModified) {
    m_removeByRestrictionModified.emplace();
  }
  return *m_removeByRestrictionModified;
}
rpgmutils::signal<void(State*, bool)>& State::removeByWalkingModified() {
  if (!m_removeByWalkingModified) {
    m_removeByWalkingModified.emplace();
  }
  return *m_removeByWalkingModified;
}
rpgmutils::signal<void(State*, StateRestriction)>& State::restrictionModified() {
  if (!m_restrictionModified) {
    m_restrictionModified.emplace();
  }
  return *m_restrictionModified;
}
rpgmutils::signal<void(State*, int)>& State::stepsToRemoveModified() {
  if (!m_stepsToRemoveModified) {
    m_stepsToRemoveModified.emplace();
  }
  return *m_stepsToRemoveModified;
}
rpgmutils::signal<void(State*, const std::vector<Trait>&)>& State::traitsModified() {
  if (!m_traitsModified) {
    m_traitsModified.emplace();
  }
  return *m_traitsModified;
}
void to_json(nlohmann::ordered_json& j, const State& state) {
  j = {
      {"id", state.m_id},
      {"autoRemovalTiming", state.m_autoRemovalTiming},
      {"chanceByDamage", state.m_chanceByDamage},
      {"iconIndex", state.m_iconIndex},
      {"maxTurns", state.m_maxTurns},
      {"message1", state.m_message1},
      {"message2", state.m_message2},
      {"message3", state.m_message3},
      {"message4", state.m_message4},
      {"minTurns", state.m_minTurns},
      {"motion", state.m_motion},
      {"name", state.m_name},
      {"note", state.m_note},
      {"overlay", state.m_overlay},
      {"priority", state.m_priority},
      {"releaseByDamage", state.m_releaseByDamage},
      {"removeAtBattleEnd", state.m_removeAtBattleEnd},
      {"removeByDamage", state.m_removeByDamage},
      {"removeByRestriction", state.m_removeByRestriction},
      {"removeByWalking", state.m_removeByWalking},
      {"restriction", state.m_restriction},
      {"stepsToRemove", state.m_stepsToRemove},
      {"traits", state.m_traits},
  };
}

void from_json(const nlohmann::ordered_json& j, State& state) {
  state.m_id = j.value("id", state.m_id);
  state.m_autoRemovalTiming = j.value("autoRemovalTiming", state.m_autoRemovalTiming);
  state.m_chanceByDamage = j.value("chanceByDamage", state.m_chanceByDamage);
  state.m_iconIndex = j.value("iconIndex", state.m_iconIndex);
  state.m_maxTurns = j.value("maxTurns", state.m_maxTurns);
  state.m_message1 = j.value("message1", state.m_message1);
  state.m_message2 = j.value("message2", state.m_message2);
  state.m_message3 = j.value("message3", state.m_message3);
  state.m_message4 = j.value("message4", state.m_message4);
  state.m_minTurns = j.value("minTurns", state.m_minTurns);
  state.m_motion = j.value("motion", state.m_motion);
  state.m_name = j.value("name", state.m_name);
  state.m_note = j.value("note", state.m_note);
  state.m_overlay = j.value("overlay", state.m_overlay);
  state.m_priority = j.value("priority", state.m_priority);
  state.m_releaseByDamage = j.value("releaseByDamage", state.m_releaseByDamage);
  state.m_removeAtBattleEnd = j.value("removeAtBattleEnd", state.m_removeAtBattleEnd);
  state.m_removeByRestriction = j.value("removeByRestriction", state.m_removeByRestriction);
  state.m_removeByWalking = j.value("removeByWalking", state.m_removeByWalking);
  state.m_restriction = j.value("restriction", state.m_restriction);
  state.m_stepsToRemove = j.value("stepsToRemove", state.m_stepsToRemove);
  state.m_traits = j.value("traits", state.m_traits);
}
