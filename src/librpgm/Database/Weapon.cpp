
#include "Database/Weapon.hpp"

Weapon::Weapon(const Weapon& other)
: IModifiable(other)
, m_id(other.m_id)
, m_animationId(other.m_animationId)
, m_description(other.m_description)
, m_etypeId(other.m_etypeId)
, m_traits(other.m_traits)
, m_iconIndex(other.m_iconIndex)
, m_name(other.m_name)
, m_note(other.m_note)
, m_params(other.m_params)
, m_price(other.m_price)
, m_wtypeId(other.m_wtypeId)
, m_oldid(other.m_oldid)
, m_oldanimationId(other.m_oldanimationId)
, m_olddescription(other.m_olddescription)
, m_oldetypeId(other.m_oldetypeId)
, m_oldtraits(other.m_oldtraits)
, m_oldiconIndex(other.m_oldiconIndex)
, m_oldname(other.m_oldname)
, m_oldnote(other.m_oldnote)
, m_oldparams(other.m_oldparams)
, m_oldprice(other.m_oldprice)
, m_oldwtypeId(other.m_oldwtypeId)
, m_isValid(other.m_isValid) {}

Weapon& Weapon::operator=(const Weapon& other) {
  IModifiable::operator=(other);
  m_id = other.m_id;
  m_animationId = other.m_animationId;
  m_description = other.m_description;
  m_etypeId = other.m_etypeId;
  m_traits = other.m_traits;
  m_iconIndex = other.m_iconIndex;
  m_name = other.m_name;
  m_note = other.m_note;
  m_params = other.m_params;
  m_price = other.m_price;
  m_wtypeId = other.m_wtypeId;
  m_oldid = other.m_oldid;
  m_oldanimationId = other.m_oldanimationId;
  m_olddescription = other.m_olddescription;
  m_oldetypeId = other.m_oldetypeId;
  m_oldtraits = other.m_oldtraits;
  m_oldiconIndex = other.m_oldiconIndex;
  m_oldname = other.m_oldname;
  m_oldnote = other.m_oldnote;
  m_oldparams = other.m_oldparams;
  m_oldprice = other.m_oldprice;
  m_oldwtypeId = other.m_oldwtypeId;
  m_isValid = other.m_isValid;
  return *this;
}

Weapon::Weapon(Weapon&& other) noexcept
: IModifiable(std::move(other))
, m_id(other.m_id)
, m_animationId(other.m_animationId)
, m_description(std::move(other.m_description))
, m_etypeId(other.m_etypeId)
, m_traits(std::move(other.m_traits))
, m_iconIndex(other.m_iconIndex)
, m_name(std::move(other.m_name))
, m_note(std::move(other.m_note))
, m_params(std::move(other.m_params))
, m_price(other.m_price)
, m_wtypeId(other.m_wtypeId)
, m_oldid(other.m_oldid)
, m_oldanimationId(other.m_oldanimationId)
, m_olddescription(std::move(other.m_olddescription))
, m_oldetypeId(other.m_oldetypeId)
, m_oldtraits(std::move(other.m_oldtraits))
, m_oldiconIndex(other.m_oldiconIndex)
, m_oldname(std::move(other.m_oldname))
, m_oldnote(std::move(other.m_oldnote))
, m_oldparams(std::move(other.m_oldparams))
, m_oldprice(other.m_oldprice)
, m_oldwtypeId(other.m_oldwtypeId)
, m_isValid(other.m_isValid) {}

Weapon& Weapon::operator=(Weapon&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_id = other.m_id;
  m_animationId = other.m_animationId;
  m_description = std::move(other.m_description);
  m_etypeId = other.m_etypeId;
  m_traits = std::move(other.m_traits);
  m_iconIndex = other.m_iconIndex;
  m_name = std::move(other.m_name);
  m_note = std::move(other.m_note);
  m_params = std::move(other.m_params);
  m_price = other.m_price;
  m_wtypeId = other.m_wtypeId;
  m_oldid = other.m_oldid;
  m_oldanimationId = other.m_oldanimationId;
  m_olddescription = std::move(other.m_olddescription);
  m_oldetypeId = other.m_oldetypeId;
  m_oldtraits = std::move(other.m_oldtraits);
  m_oldiconIndex = other.m_oldiconIndex;
  m_oldname = std::move(other.m_oldname);
  m_oldnote = std::move(other.m_oldnote);
  m_oldparams = std::move(other.m_oldparams);
  m_oldprice = other.m_oldprice;
  m_oldwtypeId = other.m_oldwtypeId;
  m_isValid = other.m_isValid;
  return *this;
}

[[nodiscard]] int Weapon::id() const { return m_id; }
void Weapon::setId(const int id) {
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

[[nodiscard]] int Weapon::animationId() const { return m_animationId; }
void Weapon::setAnimationId(const int animationId) {
  if (m_animationId == animationId) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(animationId);
  m_animationId = animationId;
  if (!signalsDisabled()) {
    animationIdModified().fire(this, animationId);
  }
  setHasChanges();
}

[[nodiscard]] const std::string& Weapon::description() const { return m_description; }
void Weapon::setDescription(const std::string& description) {
  if (m_description == description) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(description);
  m_description = description;
  if (!signalsDisabled()) {
    descriptionModified().fire(this, description);
  }
  setHasChanges();
}

[[nodiscard]] int Weapon::etypeId() const { return m_etypeId; }
void Weapon::setEtypeId(const int etypeId) {
  if (m_etypeId == etypeId) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(etypeId);
  m_etypeId = etypeId;
  if (!signalsDisabled()) {
    etypeIdModified().fire(this, etypeId);
  }
  setHasChanges();
}

[[nodiscard]] std::vector<Trait>& Weapon::traits() { return m_traits; }
void Weapon::setTraits(const std::vector<Trait>& traits) {
  if (m_traits == traits) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(traits);
  m_traits = traits;
  if (!signalsDisabled()) {
    traitsModified().fire(this, m_traits);
  }
  setHasChanges();
}

[[nodiscard]] int Weapon::iconIndex() const { return m_iconIndex; }
void Weapon::setIconIndex(const int iconIndex) {
  if (m_iconIndex == iconIndex) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(iconIndex);
  m_iconIndex = iconIndex;
  if (!signalsDisabled()) {
    iconIndexModified().fire(this, m_iconIndex);
  }
  setHasChanges();
}

[[nodiscard]] const std::string& Weapon::name() const { return m_name; }
void Weapon::setName(const std::string& name) {
  if (m_name == name) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(name);
  m_name = name;
  if (!signalsDisabled()) {
    nameModified().fire(this, m_name);
  }
  setHasChanges();
}

[[nodiscard]] const std::string& Weapon::note() const { return m_note; }
void Weapon::setNote(const std::string& note) {
  if (m_note == note) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(note);
  m_note = note;
  if (!signalsDisabled()) {
    noteModified().fire(this, m_note);
  }
  setHasChanges();
}

std::array<int, 8>& Weapon::params() { return m_params; }
void Weapon::setParams(const std::array<int, 8>& params) {
  if (m_params == params) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(params);
  m_params = params;
  if (!signalsDisabled()) {
    paramsModified().fire(this, m_params);
  }
  setHasChanges();
}
[[nodiscard]] int Weapon::param(const int idx) const {
  assert(idx >= 0 && idx < m_params.size());
  return m_params[idx];
}
void Weapon::setParam(const int idx, const int param) {
  assert(param >= 0 && param < m_params.size());
  if (m_params[idx] == param) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(params);
  m_params[idx] = param;
  if (!signalsDisabled()) {
    paramsModified().fire(this, m_params);
  }
  setHasChanges();
}

[[nodiscard]] int Weapon::price() const { return m_price; }
void Weapon::setPrice(const int price) {
  if (m_price == price) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(price);
  m_price = price;
  if (!signalsDisabled()) {
    priceModified().fire(this, m_price);
  }
  setHasChanges();
}

[[nodiscard]] int Weapon::wtypeId() const { return m_wtypeId; }
void Weapon::setWtypeId(const int wtypeId) {
  if (m_wtypeId == wtypeId) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(wtypeId);
  m_wtypeId = wtypeId;
  if (!signalsDisabled()) {
    wtypeIdModified().fire(this, m_wtypeId);
  }
  setHasChanges();
}

void Weapon::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(id);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(animationId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(description);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(etypeId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(traits);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(iconIndex);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(note);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(params);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(price);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(wtypeId);
  std::ranges::for_each(m_traits, [](Trait& trait) { trait.restoreOriginal(); });
}
void Weapon::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(id);
  MODIFIABLE_ACCEPT_VALUE(animationId);
  MODIFIABLE_ACCEPT_VALUE(description);
  MODIFIABLE_ACCEPT_VALUE(etypeId);
  MODIFIABLE_ACCEPT_VALUE(traits);
  MODIFIABLE_ACCEPT_VALUE(iconIndex);
  MODIFIABLE_ACCEPT_VALUE(name);
  MODIFIABLE_ACCEPT_VALUE(note);
  MODIFIABLE_ACCEPT_VALUE(params);
  MODIFIABLE_ACCEPT_VALUE(price);
  MODIFIABLE_ACCEPT_VALUE(wtypeId);
  std::ranges::for_each(m_traits, [](Trait& trait) { trait.acceptChanges(); });
}
nlohmann::ordered_json Weapon::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(id),     MODIFIABLE_SERIALIZE_VALUE(animationId), MODIFIABLE_SERIALIZE_VALUE(description), MODIFIABLE_SERIALIZE_VALUE(etypeId),
      MODIFIABLE_SERIALIZE_VALUE(traits), MODIFIABLE_SERIALIZE_VALUE(iconIndex),   MODIFIABLE_SERIALIZE_VALUE(name),        MODIFIABLE_SERIALIZE_VALUE(note),
      MODIFIABLE_SERIALIZE_VALUE(params), MODIFIABLE_SERIALIZE_VALUE(price),       MODIFIABLE_SERIALIZE_VALUE(wtypeId),
  };
}

rpgmutils::signal<void(Weapon*, int)>& Weapon::idModified() {
  if (!m_idModified) {
    m_idModified.emplace();
  }
  return *m_idModified;
}
rpgmutils::signal<void(Weapon*, int)>& Weapon::animationIdModified() {
  if (!m_animationIdModified) {
    m_animationIdModified.emplace();
  }
  return *m_animationIdModified;
}
rpgmutils::signal<void(Weapon*, const std::string&)>& Weapon::descriptionModified() {
  if (!m_descriptionModified) {
    m_descriptionModified.emplace();
  }
  return *m_descriptionModified;
}
rpgmutils::signal<void(Weapon*, int)>& Weapon::etypeIdModified() {
  if (!m_etypeIdModified) {
    m_etypeIdModified.emplace();
  }
  return *m_etypeIdModified;
}
rpgmutils::signal<void(Weapon*, const std::vector<Trait>&)>& Weapon::traitsModified() {
  if (!m_traitsModified) {
    m_traitsModified.emplace();
  }
  return *m_traitsModified;
}
rpgmutils::signal<void(Weapon*, int)>& Weapon::iconIndexModified() {
  if (!m_iconIndexModified) {
    m_iconIndexModified.emplace();
  }
  return *m_iconIndexModified;
}
rpgmutils::signal<void(Weapon*, const std::string&)>& Weapon::nameModified() {
  if (!m_nameModified) {
    m_nameModified.emplace();
  }
  return *m_nameModified;
}
rpgmutils::signal<void(Weapon*, const std::string&)>& Weapon::noteModified() {
  if (!m_noteModified) {
    m_noteModified.emplace();
  }
  return *m_noteModified;
}
rpgmutils::signal<void(Weapon*, const std::array<int, 8>&)>& Weapon::paramsModified() {
  if (!m_paramsModified) {
    m_paramsModified.emplace();
  }
  return *m_paramsModified;
}
rpgmutils::signal<void(Weapon*, int)>& Weapon::priceModified() {
  if (!m_priceModified) {
    m_priceModified.emplace();
  }
  return *m_priceModified;
}
rpgmutils::signal<void(Weapon*, int)>& Weapon::wtypeIdModified() {
  if (!m_wtypeIdModified) {
    m_wtypeIdModified.emplace();
  }
  return *m_wtypeIdModified;
}

void to_json(nlohmann::ordered_json& j, const Weapon& w) {
  j = {
      {"id", w.m_id},
      {"animationId", w.m_animationId},
      {"description", w.m_description},
      {"etypeId", w.m_etypeId},
      {"traits", w.m_traits},
      {"iconIndex", w.m_iconIndex},
      {"name", w.m_name},
      {"note", w.m_note},
      {"params", w.m_params},
      {"price", w.m_price},
      {"wtypeId", w.m_wtypeId},
  };
}

void from_json(const nlohmann::ordered_json& j, Weapon& w) {
  w.m_id = j.value("id", w.m_id);
  w.m_animationId = j.value("animationId", w.m_animationId);
  w.m_description = j.value("description", w.m_description);
  w.m_etypeId = j.value("etypeId", w.m_etypeId);
  w.m_traits = j.value("traits", w.m_traits);
  w.m_iconIndex = j.value("iconIndex", w.m_iconIndex);
  w.m_name = j.value("name", w.m_name);
  w.m_note = j.value("note", w.m_note);
  w.m_params = j.value("params", w.m_params);
  w.m_price = j.value("price", w.m_price);
  w.m_wtypeId = j.value("wtypeId", w.m_wtypeId);
}