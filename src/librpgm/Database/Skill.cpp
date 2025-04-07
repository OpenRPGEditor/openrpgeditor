
#include "Skill.hpp"

Skill::Skill(const Skill& other)
: IModifiable(other)
, m_id(other.m_id)
, m_animationId(other.m_animationId)
, m_damage(other.m_damage)
, m_description(other.m_description)
, m_effects(other.m_effects)
, m_iconIndex(other.m_iconIndex)
, m_message1(other.m_message1)
, m_message2(other.m_message2)
, m_mpCost(other.m_mpCost)
, m_name(other.m_name)
, m_note(other.m_note)
, m_occasion(other.m_occasion)
, m_hitType(other.m_hitType)
, m_repeats(other.m_repeats)
, m_requiredWtypeId1(other.m_requiredWtypeId1)
, m_requiredWtypeId2(other.m_requiredWtypeId2)
, m_scope(other.m_scope)
, m_speed(other.m_speed)
, m_stypeId(other.m_stypeId)
, m_successRate(other.m_successRate)
, m_tpCost(other.m_tpCost)
, m_tpGain(other.m_tpGain)
, m_oldid(other.m_oldid)
, m_olddescription(other.m_olddescription)
, m_oldeffects(other.m_oldeffects)
, m_oldiconIndex(other.m_oldiconIndex)
, m_oldmessage1(other.m_oldmessage1)
, m_oldmessage2(other.m_oldmessage2)
, m_oldmpCost(other.m_oldmpCost)
, m_oldname(other.m_oldname)
, m_oldnote(other.m_oldnote)
, m_oldoccasion(other.m_oldoccasion)
, m_oldhitType(other.m_oldhitType)
, m_oldrepeats(other.m_oldrepeats)
, m_oldrequiredWtypeId1(other.m_oldrequiredWtypeId1)
, m_oldrequiredWtypeId2(other.m_oldrequiredWtypeId2)
, m_oldscope(other.m_oldscope)
, m_oldspeed(other.m_oldspeed)
, m_oldstypeId(other.m_oldstypeId)
, m_oldsuccessRate(other.m_oldsuccessRate)
, m_oldtpCost(other.m_oldtpCost)
, m_oldtpGain(other.m_oldtpGain)
, m_isValid(other.m_isValid) {}
Skill& Skill::operator=(const Skill& other) {
  IModifiable::operator=(other);
  m_id = other.m_id;
  m_animationId = other.m_animationId;
  m_damage = other.m_damage;
  m_description = other.m_description;
  m_effects = other.m_effects;
  m_iconIndex = other.m_iconIndex;
  m_message1 = other.m_message1;
  m_message2 = other.m_message2;
  m_mpCost = other.m_mpCost;
  m_name = other.m_name;
  m_note = other.m_note;
  m_occasion = other.m_occasion;
  m_hitType = other.m_hitType;
  m_repeats = other.m_repeats;
  m_requiredWtypeId1 = other.m_requiredWtypeId1;
  m_requiredWtypeId2 = other.m_requiredWtypeId2;
  m_scope = other.m_scope;
  m_speed = other.m_speed;
  m_stypeId = other.m_stypeId;
  m_successRate = other.m_successRate;
  m_tpCost = other.m_tpCost;
  m_tpGain = other.m_tpGain;
  m_oldid = other.m_oldid;
  m_olddescription = other.m_olddescription;
  m_oldeffects = other.m_oldeffects;
  m_oldiconIndex = other.m_oldiconIndex;
  m_oldmessage1 = other.m_oldmessage1;
  m_oldmessage2 = other.m_oldmessage2;
  m_oldmpCost = other.m_oldmpCost;
  m_oldname = other.m_oldname;
  m_oldnote = other.m_oldnote;
  m_oldoccasion = other.m_oldoccasion;
  m_oldhitType = other.m_oldhitType;
  m_oldrepeats = other.m_oldrepeats;
  m_oldrequiredWtypeId1 = other.m_oldrequiredWtypeId1;
  m_oldrequiredWtypeId2 = other.m_oldrequiredWtypeId2;
  m_oldscope = other.m_oldscope;
  m_oldspeed = other.m_oldspeed;
  m_oldstypeId = other.m_oldstypeId;
  m_oldsuccessRate = other.m_oldsuccessRate;
  m_oldtpCost = other.m_oldtpCost;
  m_oldtpGain = other.m_oldtpGain;
  m_isValid = other.m_isValid;
  return *this;
}
Skill::Skill(Skill&& other) noexcept
: IModifiable(std::move(other))
, m_id(other.m_id)
, m_animationId(other.m_animationId)
, m_damage(std::move(other.m_damage))
, m_description(std::move(other.m_description))
, m_effects(std::move(other.m_effects))
, m_iconIndex(other.m_iconIndex)
, m_message1(other.m_message1)
, m_message2(other.m_message2)
, m_mpCost(other.m_mpCost)
, m_name(std::move(other.m_name))
, m_note(std::move(other.m_note))
, m_occasion(other.m_occasion)
, m_hitType(other.m_hitType)
, m_repeats(other.m_repeats)
, m_requiredWtypeId1(other.m_requiredWtypeId1)
, m_requiredWtypeId2(other.m_requiredWtypeId2)
, m_scope(other.m_scope)
, m_speed(other.m_speed)
, m_stypeId(other.m_stypeId)
, m_successRate(other.m_successRate)
, m_tpCost(other.m_tpCost)
, m_tpGain(other.m_tpGain)
, m_oldid(other.m_oldid)
, m_oldanimationId(other.m_oldanimationId)
, m_olddamage(std::move(other.m_olddamage))
, m_olddescription(other.m_olddescription)
, m_oldeffects(std::move(other.m_oldeffects))
, m_oldiconIndex(other.m_oldiconIndex)
, m_oldmessage1(std::move(other.m_oldmessage1))
, m_oldmessage2(std::move(other.m_oldmessage2))
, m_oldmpCost(other.m_oldmpCost)
, m_oldname(other.m_oldname)
, m_oldnote(other.m_oldnote)
, m_oldoccasion(other.m_oldoccasion)
, m_oldhitType(other.m_oldhitType)
, m_oldrepeats(other.m_oldrepeats)
, m_oldrequiredWtypeId1(other.m_oldrequiredWtypeId1)
, m_oldrequiredWtypeId2(other.m_oldrequiredWtypeId2)
, m_oldscope(other.m_oldscope)
, m_oldspeed(other.m_oldspeed)
, m_oldstypeId(other.m_oldstypeId)
, m_oldsuccessRate(other.m_oldsuccessRate)
, m_oldtpCost(other.m_oldtpCost)
, m_oldtpGain(other.m_oldtpGain)
, m_isValid(other.m_isValid) {}

Skill& Skill::operator=(Skill&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_id = other.m_id;
  m_animationId = other.m_animationId;
  m_damage = std::move(other.m_damage);
  m_description = std::move(other.m_description);
  m_effects = std::move(other.m_effects);
  m_iconIndex = other.m_iconIndex;
  m_message1 = std::move(other.m_message1);
  m_message2 = std::move(other.m_message2);
  m_mpCost = other.m_mpCost;
  m_name = std::move(other.m_name);
  m_note = std::move(other.m_note);
  m_occasion = other.m_occasion;
  m_hitType = other.m_hitType;
  m_repeats = other.m_repeats;
  m_requiredWtypeId1 = other.m_requiredWtypeId1;
  m_requiredWtypeId2 = other.m_requiredWtypeId2;
  m_scope = other.m_scope;
  m_speed = other.m_speed;
  m_stypeId = other.m_stypeId;
  m_successRate = other.m_successRate;
  m_tpCost = other.m_tpCost;
  m_tpGain = other.m_tpGain;
  m_oldid = other.m_oldid;
  m_oldanimationId = other.m_oldanimationId;
  m_olddamage = std::move(other.m_olddamage);
  m_olddescription = std::move(other.m_olddescription);
  m_oldeffects = std::move(other.m_oldeffects);
  m_oldiconIndex = other.m_oldiconIndex;
  m_oldmessage1 = std::move(other.m_oldmessage1);
  m_oldmessage2 = std::move(other.m_oldmessage2);
  m_oldmpCost = other.m_oldmpCost;
  m_oldname = std::move(other.m_oldname);
  m_oldnote = std::move(other.m_oldnote);
  m_oldoccasion = other.m_oldoccasion;
  m_oldhitType = other.m_oldhitType;
  m_oldrepeats = other.m_oldrepeats;
  m_oldrequiredWtypeId1 = other.m_oldrequiredWtypeId1;
  m_oldrequiredWtypeId2 = other.m_oldrequiredWtypeId2;
  m_oldscope = other.m_oldscope;
  m_oldspeed = other.m_oldspeed;
  m_stypeId = other.m_stypeId;
  m_successRate = other.m_successRate;
  m_tpCost = other.m_tpCost;
  m_tpGain = other.m_tpGain;
  m_isValid = other.m_isValid;
  return *this;
}

[[nodiscard]] int Skill::id() const { return m_id; }
void Skill::setId(const int id) {
  if (m_id == id) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(id);
  m_id = id;
  if (!signalsDisabled()) {
    idModified().fire(this, id);
  }
  setModified(true);
}

[[nodiscard]] int Skill::animationId() const { return m_animationId; }
void Skill::setAnimationId(int id) {
  if (m_animationId == id) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(animationId);
  m_animationId = id;
  if (!signalsDisabled()) {
    animationIdModified().fire(this, id);
  }
  setModified();
}

Damage& Skill::damage() { return m_damage; }
void Skill::setDamage(const Damage& damage) {
  if (m_damage == damage) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(damage);
  m_damage = damage;
  if (!signalsDisabled()) {
    damageModified().fire(this, damage);
  }
  setModified();
}

[[nodiscard]] const std::string& Skill::description() const { return m_description; }
void Skill::setDescription(const std::string& description) {
  if (m_description == description) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(description);
  m_description = description;
  if (!signalsDisabled()) {
    descriptionModified().fire(this, m_description);
  }
  setModified();
}

std::vector<Effect>& Skill::effects() { return m_effects; }
void Skill::setEffects(const std::vector<Effect>& effects) {
  if (m_effects == effects) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(effects);
  m_effects = effects;
  if (!signalsDisabled()) {
    effectsModified().fire(this, m_effects);
  }
  setModified();
}

[[nodiscard]] int Skill::iconIndex() const { return m_iconIndex; }
void Skill::setIconIndex(int iconIndex) {
  if (m_iconIndex == iconIndex) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(iconIndex);
  m_iconIndex = iconIndex;
  if (!signalsDisabled()) {
    iconIndexModified().fire(this, m_iconIndex);
  }
  setModified();
}

[[nodiscard]] const std::string& Skill::message1() const { return m_message1; }
void Skill::setMessage1(const std::string& message1) {
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

[[nodiscard]] const std::string& Skill::message2() const { return m_message2; }
void Skill::setMessage2(const std::string& message2) {
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

[[nodiscard]] int Skill::mpCost() const { return m_mpCost; }
void Skill::setMpCost(int mpCost) {
  if (m_mpCost == mpCost) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(mpCost);
  m_mpCost = mpCost;
  if (!signalsDisabled()) {
    mpCostModified().fire(this, m_mpCost);
  }
  setModified();
}

[[nodiscard]] const std::string& Skill::name() const { return m_name; }
void Skill::setName(const std::string& name) {
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

[[nodiscard]] const std::string& Skill::note() const { return m_note; }
void Skill::setNote(const std::string& note) {
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

[[nodiscard]] Occasion Skill::occasion() const { return m_occasion; }
void Skill::setOccasion(Occasion occasion) {
  if (m_occasion == occasion) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(occasion);
  m_occasion = occasion;
  if (!signalsDisabled()) {
    occasionModified().fire(this, m_occasion);
  }
  setModified();
}

[[nodiscard]] HitType Skill::hitType() const { return m_hitType; }
void Skill::setHitType(HitType hitType) {
  if (m_hitType == hitType) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(hitType);
  m_hitType = hitType;
  if (!signalsDisabled()) {
    hitTypeModified().fire(this, m_hitType);
  }
  setModified();
}

[[nodiscard]] int Skill::repeats() const { return m_repeats; }
void Skill::setRepeats(int repeats) {
  if (m_repeats == repeats) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(repeats);
  m_repeats = repeats;
  if (!signalsDisabled()) {
    repeatsModified().fire(this, m_repeats);
  }
  setModified();
}

[[nodiscard]] int Skill::requiredWtypeId1() const { return m_requiredWtypeId1; }
void Skill::setRequiredWtypeId1(int requiredWtypeId1) {
  if (m_requiredWtypeId1 == requiredWtypeId1) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(requiredWtypeId1);
  m_requiredWtypeId1 = requiredWtypeId1;
  if (!signalsDisabled()) {
    requiredWtypeId1Modified().fire(this, m_requiredWtypeId1);
  }
  setModified();
}

[[nodiscard]] int Skill::requiredWtypeId2() const { return m_requiredWtypeId2; }
void Skill::setRequiredWtypeId2(int requiredWtypeId2) {
  if (m_requiredWtypeId2 == requiredWtypeId2) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(requiredWtypeId2);
  m_requiredWtypeId2 = requiredWtypeId2;
  if (!signalsDisabled()) {
    requiredWtypeId2Modified().fire(this, m_requiredWtypeId2);
  }
  setModified();
}

[[nodiscard]] Scope Skill::scope() const { return m_scope; }
void Skill::setScope(Scope scope) {
  if (m_scope == scope) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(scope);
  m_scope = scope;
  if (!signalsDisabled()) {
    scopeModified().fire(this, m_scope);
  }
  setModified();
}

[[nodiscard]] int Skill::speed() const { return m_speed; }
void Skill::setSpeed(int speed) {
  if (m_speed == speed) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(speed);
  m_speed = speed;
  if (!signalsDisabled()) {
    speedModified().fire(this, m_speed);
  }
  setModified();
}

[[nodiscard]] int Skill::stypeId() const { return m_stypeId; }
void Skill::setStypeId(int stypeId) {
  if (m_stypeId == stypeId) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(stypeId);
  m_stypeId = stypeId;
  if (!signalsDisabled()) {
    stypeIdModified().fire(this, m_stypeId);
  }
  setModified();
}

[[nodiscard]] int Skill::successRate() const { return m_successRate; }
void Skill::setSuccessRate(int successRate) {
  if (m_successRate == successRate) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(successRate);
  m_successRate = successRate;
  if (!signalsDisabled()) {
    successRateModified().fire(this, m_successRate);
  }
  setModified();
}

[[nodiscard]] int Skill::tpCost() const { return m_tpCost; }
void Skill::setTpCost(int tpCost) {
  if (m_tpCost == tpCost) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(tpCost);
  m_tpCost = tpCost;
  if (!signalsDisabled()) {
    tpCostModified().fire(this, m_tpCost);
  }
  setModified();
}

[[nodiscard]] int Skill::tpGain() const { return m_tpGain; }
void Skill::setTpGain(int tpGain) {
  if (m_tpGain == tpGain) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(tpGain);
  m_tpGain = tpGain;
  if (!signalsDisabled()) {
    tpGainModified().fire(this, m_tpGain);
  }
  setModified();
}

[[nodiscard]] bool Skill::isValid() const { return m_isValid; }
void Skill::setValid(const bool isValid) { m_isValid = isValid; }

void Skill::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(id);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(animationId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(damage);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(description);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(effects);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(iconIndex);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(message1);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(message2);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(mpCost);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(note);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(occasion);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(hitType);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(repeats);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(requiredWtypeId1);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(requiredWtypeId2);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(scope);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(speed);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(stypeId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(successRate);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(tpCost);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(tpGain);
}
void Skill::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(id);
  MODIFIABLE_ACCEPT_VALUE(animationId);
  MODIFIABLE_ACCEPT_VALUE(damage);
  MODIFIABLE_ACCEPT_VALUE(description);
  MODIFIABLE_ACCEPT_VALUE(effects);
  MODIFIABLE_ACCEPT_VALUE(iconIndex);
  MODIFIABLE_ACCEPT_VALUE(message1);
  MODIFIABLE_ACCEPT_VALUE(message2);
  MODIFIABLE_ACCEPT_VALUE(mpCost);
  MODIFIABLE_ACCEPT_VALUE(name);
  MODIFIABLE_ACCEPT_VALUE(note);
  MODIFIABLE_ACCEPT_VALUE(occasion);
  MODIFIABLE_ACCEPT_VALUE(hitType);
  MODIFIABLE_ACCEPT_VALUE(repeats);
  MODIFIABLE_ACCEPT_VALUE(requiredWtypeId1);
  MODIFIABLE_ACCEPT_VALUE(requiredWtypeId2);
  MODIFIABLE_ACCEPT_VALUE(scope);
  MODIFIABLE_ACCEPT_VALUE(speed);
  MODIFIABLE_ACCEPT_VALUE(stypeId);
  MODIFIABLE_ACCEPT_VALUE(successRate);
  MODIFIABLE_ACCEPT_VALUE(tpCost);
  MODIFIABLE_ACCEPT_VALUE(tpGain);
}
nlohmann::ordered_json Skill::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(id),
      MODIFIABLE_SERIALIZE_VALUE(animationId),
      MODIFIABLE_SERIALIZE_VALUE(damage),
      MODIFIABLE_SERIALIZE_VALUE(description),
      MODIFIABLE_SERIALIZE_VALUE_CHILD_ARRAY(effects),
      MODIFIABLE_SERIALIZE_VALUE(iconIndex),
      MODIFIABLE_SERIALIZE_VALUE(message1),
      MODIFIABLE_SERIALIZE_VALUE(message2),
      MODIFIABLE_SERIALIZE_VALUE(mpCost),
      MODIFIABLE_SERIALIZE_VALUE(name),
      MODIFIABLE_SERIALIZE_VALUE(note),
      MODIFIABLE_SERIALIZE_VALUE(occasion),
      MODIFIABLE_SERIALIZE_VALUE(hitType),
      MODIFIABLE_SERIALIZE_VALUE(repeats),
      MODIFIABLE_SERIALIZE_VALUE(requiredWtypeId1),
      MODIFIABLE_SERIALIZE_VALUE(requiredWtypeId2),
      MODIFIABLE_SERIALIZE_VALUE(scope),
      MODIFIABLE_SERIALIZE_VALUE(speed),
      MODIFIABLE_SERIALIZE_VALUE(stypeId),
      MODIFIABLE_SERIALIZE_VALUE(successRate),
      MODIFIABLE_SERIALIZE_VALUE(tpCost),
      MODIFIABLE_SERIALIZE_VALUE(tpGain),
  };
}

rpgmutils::signal<void(Skill*, int)>& Skill::idModified() {
  if (!m_idModified) {
    m_idModified.emplace();
  }
  return *m_idModified;
}
rpgmutils::signal<void(Skill*, int)>& Skill::animationIdModified() {
  if (!m_animationIdModified) {
    m_animationIdModified.emplace();
  }
  return *m_animationIdModified;
}
rpgmutils::signal<void(Skill*, const Damage&)>& Skill::damageModified() {
  if (!m_damageModified) {
    m_damageModified.emplace();
  }
  return *m_damageModified;
}
rpgmutils::signal<void(Skill*, const std::string&)>& Skill::descriptionModified() {
  if (!m_descriptionModified) {
    m_descriptionModified.emplace();
  }
  return *m_descriptionModified;
}
rpgmutils::signal<void(Skill*, const std::vector<Effect>&)>& Skill::effectsModified() {
  if (!m_effectsModified) {
    m_effectsModified.emplace();
  }
  return *m_effectsModified;
}
rpgmutils::signal<void(Skill*, int)>& Skill::iconIndexModified() {
  if (!m_iconIndexModified) {
    m_iconIndexModified.emplace();
  }
  return *m_iconIndexModified;
}
rpgmutils::signal<void(Skill*, const std::string&)>& Skill::message1Modified() {
  if (!m_message1Modified) {
    m_message1Modified.emplace();
  }
  return *m_message1Modified;
}
rpgmutils::signal<void(Skill*, const std::string&)>& Skill::message2Modified() {
  if (!m_message2Modified) {
    m_message2Modified.emplace();
  }
  return *m_message2Modified;
}
rpgmutils::signal<void(Skill*, int)>& Skill::mpCostModified() {
  if (!m_mpCostModified) {
    m_mpCostModified.emplace();
  }
  return *m_mpCostModified;
}
rpgmutils::signal<void(Skill*, const std::string&)>& Skill::nameModified() {
  if (!m_nameModified) {
    m_nameModified.emplace();
  }
  return *m_nameModified;
}
rpgmutils::signal<void(Skill*, const std::string&)>& Skill::noteModified() {
  if (!m_noteModified) {
    m_noteModified.emplace();
  }
  return *m_noteModified;
}
rpgmutils::signal<void(Skill*, Occasion)>& Skill::occasionModified() {
  if (!m_occasionModified) {
    m_occasionModified.emplace();
  }
  return *m_occasionModified;
}
rpgmutils::signal<void(Skill*, HitType)>& Skill::hitTypeModified() {
  if (!m_hitTypeModified) {
    m_hitTypeModified.emplace();
  }
  return *m_hitTypeModified;
}
rpgmutils::signal<void(Skill*, int)>& Skill::repeatsModified() {
  if (!m_repeatsModified) {
    m_repeatsModified.emplace();
  }
  return *m_repeatsModified;
}
rpgmutils::signal<void(Skill*, int)>& Skill::requiredWtypeId1Modified() {
  if (!m_requiredWtypeId1Modified) {
    m_requiredWtypeId1Modified.emplace();
  }
  return *m_requiredWtypeId1Modified;
}
rpgmutils::signal<void(Skill*, int)>& Skill::requiredWtypeId2Modified() {
  if (!m_requiredWtypeId2Modified) {
    m_requiredWtypeId2Modified.emplace();
  }
  return *m_requiredWtypeId2Modified;
}
rpgmutils::signal<void(Skill*, Scope)>& Skill::scopeModified() {
  if (!m_scopeModified) {
    m_scopeModified.emplace();
  }
  return *m_scopeModified;
}
rpgmutils::signal<void(Skill*, int)>& Skill::speedModified() {
  if (!m_speedModified) {
    m_speedModified.emplace();
  }
  return *m_speedModified;
}
rpgmutils::signal<void(Skill*, int)>& Skill::stypeIdModified() {
  if (!m_stypeIdModified) {
    m_stypeIdModified.emplace();
  }
  return *m_stypeIdModified;
}
rpgmutils::signal<void(Skill*, int)>& Skill::successRateModified() {
  if (!m_successRateModified) {
    m_successRateModified.emplace();
  }
  return *m_successRateModified;
}
rpgmutils::signal<void(Skill*, int)>& Skill::tpCostModified() {
  if (!m_tpCostModified) {
    m_tpCostModified.emplace();
  }
  return *m_tpCostModified;
}
rpgmutils::signal<void(Skill*, int)>& Skill::tpGainModified() {
  if (!m_tpGainModified) {
    m_tpGainModified.emplace();
  }
  return *m_tpGainModified;
}

void to_json(nlohmann::ordered_json& j, const Skill& skill) {
  j = {
      {"id", skill.m_id},
      {"animationId", skill.m_animationId},
      {"damage", skill.m_damage},
      {"description", skill.m_description},
      {"effects", skill.m_effects},
      {"hitType", skill.m_hitType},
      {"iconIndex", skill.m_iconIndex},
      {"message1", skill.m_message1},
      {"message2", skill.m_message2},
      {"mpCost", skill.m_mpCost},
      {"name", skill.m_name},
      {"note", skill.m_note},
      {"occasion", skill.m_occasion},
      {"repeats", skill.m_repeats},
      {"requiredWtypeId1", skill.m_requiredWtypeId1},
      {"requiredWtypeId2", skill.m_requiredWtypeId2},
      {"scope", skill.m_scope},
      {"speed", skill.m_speed},
      {"stypeId", skill.m_stypeId},
      {"successRate", skill.m_successRate},
      {"tpCost", skill.m_tpCost},
      {"tpGain", skill.m_tpGain},
  };
}

void from_json(const nlohmann::ordered_json& j, Skill& skill) {
  skill.m_id = j.value("id", skill.m_id);
  skill.m_animationId = j.value("animationId", skill.m_animationId);
  skill.m_damage = j.value("damage", skill.m_damage);
  skill.m_description = j.value("description", skill.m_description);
  skill.m_effects = j.value("effects", skill.m_effects);
  skill.m_hitType = j.value("hitType", skill.m_hitType);
  skill.m_iconIndex = j.value("iconIndex", skill.m_iconIndex);
  skill.m_message1 = j.value("message1", skill.m_message1);
  skill.m_message2 = j.value("message2", skill.m_message2);
  skill.m_mpCost = j.value("mpCost", skill.m_mpCost);
  skill.m_name = j.value("name", skill.m_name);
  skill.m_note = j.value("note", skill.m_note);
  skill.m_occasion = j.value("occasion", skill.m_occasion);
  skill.m_repeats = j.value("repeats", skill.m_repeats);
  skill.m_scope = j.value("scope", skill.m_scope);
  skill.m_speed = j.value("speed", skill.m_speed);
  skill.m_stypeId = j.value("stypeId", skill.m_stypeId);
  skill.m_successRate = j.value("successRate", skill.m_successRate);
  skill.m_tpCost = j.value("tpCost", skill.m_tpCost);
  skill.m_tpGain = j.value("tpGain", skill.m_tpGain);
}