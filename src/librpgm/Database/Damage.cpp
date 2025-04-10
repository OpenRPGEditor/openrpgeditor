#include "Database/Damage.hpp"

Damage::Damage(const Damage& other)
: IModifiable(other)
, m_critical(other.m_critical)
, m_elementId(other.m_elementId)
, m_formula(other.m_formula)
, m_type(other.m_type)
, m_variance(other.m_variance)
, m_oldcritical(other.m_oldcritical)
, m_oldelementId(other.m_oldelementId)
, m_oldformula(other.m_oldformula)
, m_oldtype(other.m_oldtype)
, m_oldvariance(other.m_oldvariance) {}

Damage& Damage::operator=(const Damage& other) {
  IModifiable::operator=(other);
  m_critical = other.m_critical;
  m_elementId = other.m_elementId;
  m_formula = other.m_formula;
  m_type = other.m_type;
  m_variance = other.m_variance;
  m_oldcritical = other.m_oldcritical;
  m_oldelementId = other.m_oldelementId;
  m_oldformula = other.m_oldformula;
  m_oldtype = other.m_oldtype;
  m_oldvariance = other.m_oldvariance;
  return *this;
}

Damage::Damage(Damage&& other) noexcept
: IModifiable(std::move(other))
, m_critical(other.m_critical)
, m_elementId(other.m_elementId)
, m_formula(std::move(other.m_formula))
, m_type(other.m_type)
, m_variance(other.m_variance)
, m_oldcritical(other.m_oldcritical)
, m_oldelementId(other.m_oldelementId)
, m_oldformula(std::move(other.m_oldformula))
, m_oldtype(other.m_oldtype)
, m_oldvariance(other.m_oldvariance) {}

Damage& Damage::operator=(Damage&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_critical = other.m_critical;
  m_elementId = other.m_elementId;
  m_formula = std::move(other.m_formula);
  m_type = other.m_type;
  m_variance = other.m_variance;
  m_oldcritical = other.m_oldcritical;
  m_oldelementId = other.m_oldelementId;
  m_oldformula = std::move(other.m_oldformula);
  m_oldtype = other.m_oldtype;
  m_oldvariance = other.m_oldvariance;
  return *this;
}

bool Damage::critical() const { return m_critical; }
void Damage::setCritical(const bool critical) {
  MODIFIABLE_SET_OLD_VALUE(critical);
  m_critical = critical;
  if (!signalsDisabled()) {
   emit_signal(criticalModified(), this, critical);
  }
  setModified();
}

int Damage::elementId() const { return m_elementId; }
void Damage::setElementId(const int elementId) {
  MODIFIABLE_SET_OLD_VALUE(elementId);
  m_elementId = elementId;
  if (!signalsDisabled()) {
   emit_signal(elementIdModified(), this, elementId);
  }
  setModified();
}

const std::string& Damage::formula() const { return m_formula; }
void Damage::setFormula(const std::string& formula) {
  MODIFIABLE_SET_OLD_VALUE(formula);
  m_formula = formula;
  if (!signalsDisabled()) {
   emit_signal(formulaModified(), this, formula);
  }
  setModified();
}

DamageType Damage::type() const { return m_type; }
void Damage::setType(const DamageType type) {
  MODIFIABLE_SET_OLD_VALUE(type);
  m_type = type;
  if (!signalsDisabled()) {
   emit_signal(typeModified(), this, type);
  }
  setModified();
}

int Damage::variance() const { return m_variance; }

void Damage::setVariance(const int variance) {
  MODIFIABLE_SET_OLD_VALUE(variance);
  m_variance = variance;
  if (!signalsDisabled()) {
   emit_signal(varianceModified(), this, variance);
  }
  setModified();
}

void Damage::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(critical);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(elementId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(formula);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(type);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(variance);
}

void Damage::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(critical);
  MODIFIABLE_ACCEPT_VALUE(elementId);
  MODIFIABLE_ACCEPT_VALUE(formula);
  MODIFIABLE_ACCEPT_VALUE(type);
  MODIFIABLE_ACCEPT_VALUE(variance);
}

nlohmann::ordered_json Damage::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(critical), MODIFIABLE_SERIALIZE_VALUE(elementId), MODIFIABLE_SERIALIZE_VALUE(formula), MODIFIABLE_SERIALIZE_VALUE(type), MODIFIABLE_SERIALIZE_VALUE(variance),
  };
}

rpgmutils::signal<void(Damage*, bool)>& Damage::criticalModified() {
  if (!m_criticalModified) {
    m_criticalModified.emplace();
  }
  return *m_criticalModified;
}

rpgmutils::signal<void(Damage*, int)>& Damage::elementIdModified() {
  if (!m_elementIdModified) {
    m_elementIdModified.emplace();
  }
  return *m_elementIdModified;
}

rpgmutils::signal<void(Damage*, const std::string&)>& Damage::formulaModified() {
  if (!m_formulaModified) {
    m_formulaModified.emplace();
  }
  return *m_formulaModified;
}

rpgmutils::signal<void(Damage*, DamageType)>& Damage::typeModified() {
  if (!m_typeModified) {
    m_typeModified.emplace();
  }
  return *m_typeModified;
}

rpgmutils::signal<void(Damage*, int)>& Damage::varianceModified() {
  if (!m_varianceModified) {
    m_varianceModified.emplace();
  }
  return *m_varianceModified;
}

void to_json(nlohmann::ordered_json& j, const Damage& damage) {
  j = {
      {"critical", damage.m_critical}, {"elementId", damage.m_elementId}, {"formula", damage.m_formula}, {"type", damage.m_type}, {"variance", damage.m_variance},
  };
}
void from_json(const nlohmann::ordered_json& j, Damage& damage) {
  damage.m_critical = j.value("critical", damage.m_critical);
  damage.m_elementId = j.value("elementId", damage.m_elementId);
  damage.m_formula = j.value("formula", damage.m_formula);
  damage.m_type = j.value("type", damage.m_type);
  damage.m_variance = j.value("variance", damage.m_variance);
}