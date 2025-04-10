#include "Database/Armor.hpp"

Armor::Armor(const Armor& other)
: IModifiable(other)
, m_id(other.m_id)
, m_atypeId(other.m_atypeId)
, m_description(other.m_description)
, m_etypeId(other.m_etypeId)
, m_traits(other.m_traits)
, m_iconIndex(other.m_iconIndex)
, m_name(other.m_name)
, m_note(other.m_note)
, m_params(other.m_params)
, m_price(other.m_price)
, m_oldid(other.m_oldid)
, m_oldatypeId(other.m_oldatypeId)
, m_olddescription(other.m_olddescription)
, m_oldetypeId(other.m_oldetypeId)
, m_oldtraits(other.m_oldtraits)
, m_oldiconIndex(other.m_oldiconIndex)
, m_oldname(other.m_oldname)
, m_oldnote(other.m_oldnote)
, m_oldparams(other.m_oldparams)
, m_oldprice(other.m_oldprice)
, m_isValid(other.m_isValid) {}

Armor& Armor::operator=(const Armor& other) {
  IModifiable::operator=(other);
  m_id = other.m_id;
  m_atypeId = other.m_atypeId;
  m_description = other.m_description;
  m_etypeId = other.m_etypeId;
  m_traits = other.m_traits;
  m_iconIndex = other.m_iconIndex;
  m_name = other.m_name;
  m_note = other.m_note;
  m_params = other.m_params;
  m_price = other.m_price;
  m_oldid = other.m_oldid;
  m_oldatypeId = other.m_oldatypeId;
  m_olddescription = other.m_olddescription;
  m_oldetypeId = other.m_oldetypeId;
  m_oldtraits = other.m_oldtraits;
  m_oldiconIndex = other.m_oldiconIndex;
  m_oldname = other.m_oldname;
  m_oldnote = other.m_oldnote;
  m_oldparams = other.m_oldparams;
  m_oldprice = other.m_oldprice;
  m_isValid = other.m_isValid;
  return *this;
}
Armor::Armor(Armor&& other) noexcept
: IModifiable(other)
, m_id(other.m_id)
, m_atypeId(other.m_atypeId)
, m_description(other.m_description)
, m_etypeId(other.m_etypeId)
, m_traits(other.m_traits)
, m_iconIndex(other.m_iconIndex)
, m_name(other.m_name)
, m_note(other.m_note)
, m_params(other.m_params)
, m_price(other.m_price)
, m_oldid(other.m_oldid)
, m_oldatypeId(other.m_oldatypeId)
, m_olddescription(other.m_olddescription)
, m_oldetypeId(other.m_oldetypeId)
, m_oldtraits(other.m_oldtraits)
, m_oldiconIndex(other.m_oldiconIndex)
, m_oldname(other.m_oldname)
, m_oldnote(other.m_oldnote)
, m_oldparams(other.m_oldparams)
, m_oldprice(other.m_oldprice)
, m_isValid(other.m_isValid) {}

Armor& Armor::operator=(Armor&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_id = other.m_id;
  m_atypeId = other.m_atypeId;
  m_description = other.m_description;
  m_etypeId = other.m_etypeId;
  m_traits = other.m_traits;
  m_iconIndex = other.m_iconIndex;
  m_name = other.m_name;
  m_note = other.m_note;
  m_params = other.m_params;
  m_price = other.m_price;
  m_oldid = other.m_oldid;
  m_oldatypeId = other.m_oldatypeId;
  m_olddescription = other.m_olddescription;
  m_oldetypeId = other.m_oldetypeId;
  m_oldtraits = other.m_oldtraits;
  m_oldiconIndex = other.m_oldiconIndex;
  m_oldname = other.m_oldname;
  m_oldnote = other.m_oldnote;
  m_oldparams = other.m_oldparams;
  m_oldprice = other.m_oldprice;
  m_isValid = other.m_isValid;
  return *this;
}

[[nodiscard]] int Armor::id() const { return m_id; }
void Armor::setId(const int id) {
  MODIFIABLE_SET_OLD_VALUE(id);
  m_id = id;
  if (!signalsDisabled()) {
    emit_signal(idModified(), this, m_id);
  }
  setModified();
}

[[nodiscard]] int Armor::atypeId() const { return m_atypeId; }
void Armor::setAtypeId(const int atypeId) {
  MODIFIABLE_SET_OLD_VALUE(atypeId);
  m_atypeId = atypeId;
  if (!signalsDisabled()) {
   emit_signal(atypeModified(), this, m_atypeId);
  }
  setModified();
}

[[nodiscard]] const std::string& Armor::description() const { return m_description; }
void Armor::setDescription(const std::string& description) {
  MODIFIABLE_SET_OLD_VALUE(description);
  m_description = description;
  if (!signalsDisabled()) {
   emit_signal(descriptionModified(), this, m_description);
  }
  setModified();
}

[[nodiscard]] int Armor::etypeId() const { return m_etypeId; }
void Armor::setEtypeId(const int etypeId) {
  MODIFIABLE_SET_OLD_VALUE(etypeId);
  m_etypeId = etypeId;
  if (!signalsDisabled()) {
   emit_signal(etypeModified(), this, m_etypeId);
  }
  setModified();
}

[[nodiscard]] std::vector<Trait>& Armor::traits() { return m_traits; }
[[nodiscard]] const std::vector<Trait>& Armor::traits() const { return m_traits; }
void Armor::setTraits(const std::vector<Trait>& traits) {
  MODIFIABLE_SET_OLD_VALUE(traits);
  m_traits = traits;
  if (!signalsDisabled()) {
   emit_signal(traitsModified(), this, m_traits);
  }
  setModified();
}

Trait& Armor::trait(const int index) {
  assert(index >= 0 && index < m_traits.size());
  return m_traits[index];
}

const Trait& Armor::trait(const int index) const {
  assert(index >= 0 && index < m_traits.size());
  return m_traits[index];
}

void Armor::setTrait(const int index, const Trait& trait) {
  assert(index >= 0 && index < m_traits.size());
  MODIFIABLE_SET_OLD_VALUE(traits);
  m_traits[index] = trait;
  if (!signalsDisabled()) {
   emit_signal(traitModified(), this, index, trait);
  }
  setModified();
}

[[nodiscard]] int Armor::iconIndex() const { return m_iconIndex; }
void Armor::setIconIndex(const int iconIndex) {
  MODIFIABLE_SET_OLD_VALUE(iconIndex);
  m_iconIndex = iconIndex;
  if (!signalsDisabled()) {
   emit_signal(iconIndexModified(), this, m_iconIndex);
  }
  setModified();
}

[[nodiscard]] const std::string& Armor::name() const { return m_name; }
void Armor::setName(const std::string& name) {
  MODIFIABLE_SET_OLD_VALUE(name);
  m_name = name;
  if (!signalsDisabled()) {
   emit_signal(nameModified(), this, m_name);
  }
  setModified();
}

[[nodiscard]] const std::string& Armor::note() const { return m_note; }
void Armor::setNote(const std::string& note) {
  MODIFIABLE_SET_OLD_VALUE(note);
  m_note = note;
  if (!signalsDisabled()) {
   emit_signal(noteModified(), this, m_note);
  }
  setModified();
}

[[nodiscard]] const std::array<int, 8>& Armor::params() const { return m_params; }
void Armor::setParams(const std::array<int, 8>& params) {
  MODIFIABLE_SET_OLD_VALUE(params);
  m_params = params;
  if (!signalsDisabled()) {
   emit_signal(paramsModified(), this, m_params);
  }
  setModified();
}
[[nodiscard]] int Armor::param(const int index) const {
  assert(index >= 0 && index < m_params.size());
  return m_params[index];
}

void Armor::setParam(const int index, const int param) {
  assert(param >= 0 && param < m_params.size());
  MODIFIABLE_SET_OLD_VALUE(params);
  m_params[index] = param;
  if (!signalsDisabled()) {
   emit_signal(paramModified(), this, index, param);
  }
  setModified();
}

[[nodiscard]] int Armor::price() const { return m_price; }
void Armor::setPrice(const int price) {
  MODIFIABLE_SET_OLD_VALUE(price);
  m_price = price;
  if (!signalsDisabled()) {
   emit_signal(priceModified(), this, m_price);
  }
  setModified();
}
[[nodiscard]] bool Armor::isValid() const { return m_isValid; }
void Armor::setValid(const bool isValid) { m_isValid = isValid; }

void Armor::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(id);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(atypeId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(description);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(etypeId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(traits);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(iconIndex);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(note);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(params);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(price);
}

void Armor::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(id);
  MODIFIABLE_ACCEPT_VALUE(atypeId);
  MODIFIABLE_ACCEPT_VALUE(description);
  MODIFIABLE_ACCEPT_VALUE(etypeId);
  MODIFIABLE_ACCEPT_VALUE(iconIndex);
  MODIFIABLE_ACCEPT_VALUE(name);
  MODIFIABLE_ACCEPT_VALUE(note);
  MODIFIABLE_ACCEPT_VALUE(params);
  MODIFIABLE_ACCEPT_VALUE(price);
}

nlohmann::ordered_json Armor::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(id),   MODIFIABLE_SERIALIZE_VALUE(atypeId), MODIFIABLE_SERIALIZE_VALUE(description), MODIFIABLE_SERIALIZE_VALUE(etypeId), MODIFIABLE_SERIALIZE_VALUE(iconIndex),
      MODIFIABLE_SERIALIZE_VALUE(name), MODIFIABLE_SERIALIZE_VALUE(note),    MODIFIABLE_SERIALIZE_VALUE(params),      MODIFIABLE_SERIALIZE_VALUE(price),
  };
}

// SIGNALS
rpgmutils::signal<void(Armor*, int)>& Armor::idModified() {
  if (!m_idModified) {
    m_idModified.emplace();
  }
  return *m_idModified;
}

rpgmutils::signal<void(Armor*, int)>& Armor::atypeModified() {
  if (!m_atypeModified) {
    m_atypeModified.emplace();
  }
  return *m_atypeModified;
}

rpgmutils::signal<void(Armor*, std::string_view)>& Armor::descriptionModified() {
  if (!m_descriptionModified) {
    m_descriptionModified.emplace();
  }
  return *m_descriptionModified;
}

rpgmutils::signal<void(Armor*, int)>& Armor::etypeModified() {
  if (!m_etypeModified) {
    m_etypeModified.emplace();
  }
  return *m_etypeModified;
}

rpgmutils::signal<void(Armor*, const std::vector<Trait>&)>& Armor::traitsModified() {
  if (!m_traitsModified) {
    m_traitsModified.emplace();
  }
  return *m_traitsModified;
}

rpgmutils::signal<void(Armor*, int, const Trait&)>& Armor::traitModified() {
  if (!m_traitModified) {
    m_traitModified.emplace();
  }
  return *m_traitModified;
}

rpgmutils::signal<void(Armor*, int)>& Armor::iconIndexModified() {
  if (!m_iconIndexModified) {
    m_iconIndexModified.emplace();
  }
  return *m_iconIndexModified;
}

rpgmutils::signal<void(Armor*, std::string_view)>& Armor::nameModified() {
  if (!m_nameModified) {
    m_nameModified.emplace();
  }
  return *m_nameModified;
}
rpgmutils::signal<void(Armor*, std::string_view)>& Armor::noteModified() {
  if (!m_noteModified) {
    m_noteModified.emplace();
  }
  return *m_noteModified;
}
rpgmutils::signal<void(Armor*, const std::array<int, 8>&)>& Armor::paramsModified() {
  if (!m_paramsModified) {
    m_paramsModified.emplace();
  }
  return *m_paramsModified;
}
rpgmutils::signal<void(Armor*, int, int)>& Armor::paramModified() {
  if (!m_paramModified) {
    m_paramModified.emplace();
  }
  return *m_paramModified;
}
rpgmutils::signal<void(Armor*, int)>& Armor::priceModified() {
  if (!m_priceModified) {
    m_priceModified.emplace();
  }
  return *m_priceModified;
}

void to_json(nlohmann::ordered_json& out, const Armor& armor) {
  out = {
      {"id", armor.m_id},     {"atypeId", armor.m_atypeId}, {"description", armor.m_description}, {"etypeId", armor.m_etypeId}, {"traits", armor.m_traits}, {"iconIndex", armor.m_iconIndex},
      {"name", armor.m_name}, {"note", armor.m_note},       {"params", armor.m_params},           {"price", armor.m_price},
  };
}

void from_json(const nlohmann::ordered_json& from, Armor& armor) {
  armor.m_id = from.value("id", armor.m_id);
  armor.m_atypeId = from.value("atypeId", armor.m_atypeId);
  armor.m_description = from.value("description", armor.m_description);
  armor.m_etypeId = from.value("etypeId", armor.m_etypeId);
  armor.m_traits = from.value("traits", armor.m_traits);
  armor.m_iconIndex = from.value("iconIndex", armor.m_iconIndex);
  armor.m_name = from.value("name", armor.m_name);
  armor.m_note = from.value("note", armor.m_note);
  armor.m_params = from.value("params", armor.m_params);
  armor.m_price = from.value("price", armor.m_price);
}
