#include "Database/Effect.hpp"

Effect::Effect(const Effect& other)
: IModifiable(other)
, m_code(other.m_code)
, m_dataId(other.m_dataId)
, m_value1(other.m_value1)
, m_value2(other.m_value2)
, m_oldcode(other.m_oldcode)
, m_olddataId(other.m_olddataId)
, m_oldvalue1(other.m_oldvalue1)
, m_oldvalue2(other.m_oldvalue2) {}
Effect& Effect::operator=(const Effect& other) {
  IModifiable::operator=(other);
  m_code = other.m_code;
  m_dataId = other.m_dataId;
  m_value1 = other.m_value1;
  m_value2 = other.m_value2;
  m_oldcode = other.m_oldcode;
  m_olddataId = other.m_olddataId;
  m_oldvalue1 = other.m_oldvalue1;
  m_oldvalue2 = other.m_oldvalue2;
  return *this;
}
Effect::Effect(Effect&& other) noexcept
: IModifiable(std::move(other))
, m_code(other.m_code)
, m_dataId(other.m_dataId)
, m_value1(other.m_value1)
, m_value2(other.m_value2)
, m_oldcode(other.m_oldcode)
, m_olddataId(other.m_olddataId)
, m_oldvalue1(other.m_oldvalue1)
, m_oldvalue2(other.m_oldvalue2) {}
Effect& Effect::operator=(Effect&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_code = other.m_code;
  m_dataId = other.m_dataId;
  m_value1 = other.m_value1;
  m_value2 = other.m_value2;
  m_oldcode = other.m_oldcode;
  m_olddataId = other.m_olddataId;
  m_oldvalue1 = other.m_oldvalue1;
  m_oldvalue2 = other.m_oldvalue2;
  return *this;
}

EffectCode Effect::code() const { return m_code; }
void Effect::setCode(const EffectCode code) {
  MODIFIABLE_SET_OLD_VALUE(code);
  m_code = code;
  if (!signalsDisabled()) {
    codeModified().fire(this, code);
  }
  setHasChanges();
}

int Effect::dataId() const { return m_dataId; }
void Effect::setDataId(const int dataId) {
  MODIFIABLE_SET_OLD_VALUE(dataId);
  m_dataId = dataId;
  if (!signalsDisabled()) {
    dataIdModified().fire(this, m_dataId);
  }
  setHasChanges();
}

float Effect::value1() const { return m_value1; }
void Effect::setValue1(const float value1) {
  MODIFIABLE_SET_OLD_VALUE(value1);
  m_value1 = value1;
  if (!signalsDisabled()) {
    value1Modified().fire(this, m_value1);
  }
  setHasChanges();
}

int Effect::value2() const { return m_value2; }

void Effect::setValue2(const int value2) {
  MODIFIABLE_SET_OLD_VALUE(value2);
  m_value2 = value2;
  if (!signalsDisabled()) {
    value2Modified().fire(this, m_value2);
  }
  setHasChanges();
}

void Effect::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(code);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(dataId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(value1);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(value2);
}

void Effect::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(code);
  MODIFIABLE_ACCEPT_VALUE(dataId);
  MODIFIABLE_ACCEPT_VALUE(value1);
  MODIFIABLE_ACCEPT_VALUE(value2);
}

nlohmann::ordered_json Effect::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(code),
      MODIFIABLE_SERIALIZE_VALUE(dataId),
      MODIFIABLE_SERIALIZE_VALUE(value1),
      MODIFIABLE_SERIALIZE_VALUE(value2),
  };
}

rpgmutils::signal<void(Effect*, EffectCode)>& Effect::codeModified() {
  if (!m_codeModified) {
    m_codeModified.emplace();
  }
  return *m_codeModified;
}

rpgmutils::signal<void(Effect*, int)>& Effect::dataIdModified() {
  if (!m_dataIdModified) {
    m_dataIdModified.emplace();
  }
  return *m_dataIdModified;
}

rpgmutils::signal<void(Effect*, float)>& Effect::value1Modified() {
  if (!m_value1Modified) {
    m_value1Modified.emplace();
  }
  return *m_value1Modified;
}

rpgmutils::signal<void(Effect*, int)>& Effect::value2Modified() {
  if (!m_value2Modified) {
    m_value2Modified.emplace();
  }
  return *m_value2Modified;
}

void to_json(nlohmann::ordered_json& to, const Effect& effect) {
  to = {
      {"code", effect.m_code},
      {"dataId", effect.m_dataId},
      {"value1", effect.m_value1},
      {"value2", effect.m_value2},
  };
}

void from_json(const nlohmann::ordered_json& from, Effect& effect) {
  effect.m_code = from.value("code", effect.m_code);
  effect.m_dataId = from.value("dataId", effect.m_dataId);
  effect.m_value1 = from.value("value1", effect.m_value1);
  effect.m_value2 = from.value("value2", effect.m_value2);
}