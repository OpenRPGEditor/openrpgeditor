#include "Database/Trait.hpp"

void to_json(nlohmann::ordered_json& j, const Trait& t) {
  j = {
      {"code", t.m_code},
      {"dataId", t.m_dataId},
      {"value", t.m_value},
  };
}

void from_json(const nlohmann::ordered_json& j, Trait& t) {
  t.m_code = j.value("code", t.m_code);
  t.m_dataId = j.value("dataId", t.m_dataId);
  t.m_value = j.value("value", t.m_value);
}

Trait::Trait(const Trait& trait)
: IModifiable(trait)
, m_code(trait.m_code)
, m_dataId(trait.m_dataId)
, m_value(trait.m_value) {}
Trait& Trait::operator=(const Trait& trait) {
  IModifiable::operator=(trait);
  m_code = trait.m_code;
  m_dataId = trait.m_dataId;
  m_value = trait.m_value;
  return *this;
}
Trait::Trait(Trait&& trait) noexcept
: m_code(trait.m_code)
, m_dataId(trait.m_dataId)
, m_value(trait.m_value)
, IModifiable(std::move(trait)) {}
Trait& Trait::operator=(Trait&& trait) noexcept {
  m_code = trait.m_code;
  m_dataId = trait.m_dataId;
  m_value = trait.m_value;
  IModifiable::operator=(std::move(trait));
  return *this;
}

TraitCode Trait::code() const { return m_code; }
void Trait::setCode(const TraitCode code) {
  MODIFIABLE_SET_OLD_VALUE(code);
  m_code = code;
  if (!signalsDisabled()) {
    codeModified().fire(this, code);
  }
}

[[nodiscard]] int Trait::dataId() const { return m_dataId; }
void Trait::setDataId(const int dataId) {
  MODIFIABLE_SET_OLD_VALUE(dataId);
  m_dataId = dataId;
  if (!signalsDisabled()) {
    dataIdModified().fire(this, dataId);
  }
}

[[nodiscard]] double Trait::value() const { return m_value; }
void Trait::setValue(const double value) {
  MODIFIABLE_SET_OLD_VALUE(value);
  m_value = value;
  if (!signalsDisabled()) {
    valueModified().fire(this, value);
  }
}

void Trait::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(code);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(dataId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(value);
}

void Trait::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(code);
  MODIFIABLE_ACCEPT_VALUE(dataId);
  MODIFIABLE_ACCEPT_VALUE(value);
}

nlohmann::ordered_json Trait::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(code),
      MODIFIABLE_SERIALIZE_VALUE(dataId),
      MODIFIABLE_SERIALIZE_VALUE(value),
  };
}

rpgmutils::signal<void(Trait*, TraitCode)>& Trait::codeModified() {
  if (!m_codeModified) {
    m_codeModified.emplace();
  }
  return *m_codeModified;
}

rpgmutils::signal<void(Trait*, int)>& Trait::dataIdModified() {
  if (!m_dataIdModified) {
    m_dataIdModified.emplace();
  }
  return *m_dataIdModified;
}

rpgmutils::signal<void(Trait*, double)>& Trait::valueModified() {
  if (!m_valueModified) {
    m_valueModified.emplace();
  }
  return *m_valueModified;
}