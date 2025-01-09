#include "Database/Item.hpp"

Item::Item(const Item& other)
: IModifiable(other)
, m_id(other.m_id)
, m_animationId(other.m_animationId)
, m_damage(other.m_damage)
, m_consumable(other.m_consumable)
, m_description(other.m_description)
, m_effects(other.m_effects)
, m_hitType(other.m_hitType)
, m_iconIndex(other.m_iconIndex)
, m_itypeId(other.m_itypeId)
, m_name(other.m_name)
, m_note(other.m_note)
, m_occasion(other.m_occasion)
, m_price(other.m_price)
, m_repeats(other.m_repeats)
, m_scope(other.m_scope)
, m_speed(other.m_speed)
, m_successRate(other.m_successRate)
, m_tpGain(other.m_tpGain)
, m_oldid(other.m_id)
, m_oldanimationId(other.m_animationId)
, m_olddamage(other.m_damage)
, m_oldconsumable(other.m_consumable)
, m_olddescription(other.m_description)
, m_oldeffects(other.m_effects)
, m_oldhitType(other.m_hitType)
, m_oldiconIndex(other.m_iconIndex)
, m_olditypeId(other.m_itypeId)
, m_oldname(other.m_name)
, m_oldnote(other.m_note)
, m_oldoccasion(other.m_occasion)
, m_oldprice(other.m_price)
, m_oldrepeats(other.m_repeats)
, m_oldscope(other.m_scope)
, m_oldspeed(other.m_speed)
, m_oldsuccessRate(other.m_successRate)
, m_oldtpGain(other.m_tpGain)
, m_isValid(other.m_isValid) {}
Item& Item::operator=(const Item& other) {
  IModifiable::operator=(other);
  m_id = other.m_id;
  m_animationId = other.m_animationId;
  m_damage = other.m_damage;
  m_consumable = other.m_consumable;
  m_description = other.m_description;
  m_effects = other.m_effects;
  m_hitType = other.m_hitType;
  m_iconIndex = other.m_iconIndex;
  m_itypeId = other.m_itypeId;
  m_name = other.m_name;
  m_note = other.m_note;
  m_occasion = other.m_occasion;
  m_price = other.m_price;
  m_repeats = other.m_repeats;
  m_scope = other.m_scope;
  m_speed = other.m_speed;
  m_successRate = other.m_successRate;
  m_tpGain = other.m_tpGain;
  m_oldid = other.m_oldid;
  m_oldanimationId = other.m_oldanimationId;
  m_olddamage = other.m_olddamage;
  m_oldconsumable = other.m_oldconsumable;
  m_olddescription = other.m_olddescription;
  m_oldeffects = other.m_oldeffects;
  m_oldhitType = other.m_oldhitType;
  m_oldiconIndex = other.m_oldiconIndex;
  m_olditypeId = other.m_olditypeId;
  m_oldname = other.m_oldname;
  m_oldnote = other.m_oldnote;
  m_oldoccasion = other.m_oldoccasion;
  m_oldprice = other.m_oldprice;
  m_oldrepeats = other.m_oldrepeats;
  m_oldscope = other.m_oldscope;
  m_oldspeed = other.m_oldspeed;
  m_oldsuccessRate = other.m_oldsuccessRate;
  m_oldtpGain = other.m_oldtpGain;
  m_isValid = other.m_isValid;
  return *this;
}
Item::Item(Item&& other) noexcept
: IModifiable(std::move(other))
, m_id(other.m_id)
, m_animationId(other.m_animationId)
, m_damage(std::move(other.m_damage))
, m_consumable(other.m_consumable)
, m_description(std::move(other.m_description))
, m_effects(other.m_effects)
, m_hitType(other.m_hitType)
, m_iconIndex(other.m_iconIndex)
, m_itypeId(other.m_itypeId)
, m_name(std::move(other.m_name))
, m_note(std::move(other.m_note))
, m_occasion(other.m_occasion)
, m_price(other.m_price)
, m_repeats(other.m_repeats)
, m_scope(other.m_scope)
, m_speed(other.m_speed)
, m_successRate(other.m_successRate)
, m_tpGain(other.m_tpGain)
, m_oldid(other.m_id)
, m_oldanimationId(other.m_animationId)
, m_olddamage(std::move(other.m_damage))
, m_oldconsumable(other.m_consumable)
, m_olddescription(std::move(other.m_description))
, m_oldeffects(other.m_effects)
, m_oldhitType(other.m_hitType)
, m_olditypeId(other.m_itypeId)
, m_oldiconIndex(other.m_iconIndex)
, m_oldname(std::move(other.m_name))
, m_oldnote(std::move(other.m_note))
, m_oldoccasion(other.m_occasion)
, m_oldprice(other.m_price)
, m_oldrepeats(other.m_repeats)
, m_oldscope(other.m_scope)
, m_oldspeed(other.m_speed)
, m_oldsuccessRate(other.m_successRate)
, m_oldtpGain(other.m_tpGain)
, m_isValid(other.m_isValid) {}
Item& Item::operator=(Item&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_id = other.m_id;
  m_animationId = other.m_animationId;
  m_damage = std::move(other.m_damage);
  m_consumable = other.m_consumable;
  m_description = std::move(other.m_description);
  m_effects = std::move(other.m_effects);
  m_hitType = other.m_hitType;
  m_iconIndex = other.m_iconIndex;
  m_itypeId = other.m_itypeId;
  m_name = std::move(other.m_name);
  m_note = std::move(other.m_note);
  m_occasion = other.m_occasion;
  m_price = other.m_price;
  m_repeats = other.m_repeats;
  m_scope = other.m_scope;
  m_speed = other.m_speed;
  m_successRate = other.m_successRate;
  m_tpGain = other.m_tpGain;
  m_oldid = other.m_oldid;
  m_oldanimationId = other.m_oldanimationId;
  m_olddamage = other.m_olddamage;
  m_oldconsumable = other.m_oldconsumable;
  m_olddescription = std::move(other.m_olddescription);
  m_oldeffects = other.m_oldeffects;
  m_oldhitType = other.m_oldhitType;
  m_oldiconIndex = other.m_oldiconIndex;
  m_oldname = std::move(other.m_oldname);
  m_oldnote = std::move(other.m_oldnote);
  m_oldoccasion = other.m_oldoccasion;
  m_oldprice = other.m_oldprice;
  m_oldrepeats = other.m_oldrepeats;
  m_oldscope = other.m_oldscope;
  m_oldspeed = other.m_oldspeed;
  m_oldsuccessRate = other.m_oldsuccessRate;
  m_oldtpGain = other.m_oldtpGain;
  m_isValid = other.m_isValid;
  return *this;
}

[[nodiscard]] int Item::id() const { return m_id; }
void Item::setId(const int id) {
  if (id == m_id) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(id);
  m_id = id;
  if (!signalsDisabled()) {
    idModified().fire(this, id);
  }
setHasChanges();
}

[[nodiscard]] int Item::animationId() const { return m_animationId; }
void Item::setAnimationId(const int animationId) {
  if (animationId == m_animationId) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(animationId);
  m_animationId = animationId;
  if (!signalsDisabled()) {
    animationIdModified().fire(this, animationId);
  }
setHasChanges();
}

[[nodiscard]] bool Item::consumable() const { return m_consumable; }
void Item::setConsumable(const bool consumable) {
  if (consumable == m_consumable) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(consumable);
  m_consumable = consumable;
  if (!signalsDisabled()) {
    idModified().fire(this, m_consumable);
  }
setHasChanges();
}

Damage& Item::damage() { return m_damage; }
void Item::setDamage(const Damage& damage) {
  if (damage == m_damage) {
    return;
  }
  m_damage = damage;
  if (!signalsDisabled()) {
    damageModified().fire(this, m_damage);
  }
setHasChanges();
}

[[nodiscard]] const std::string& Item::description() const { return m_description; }
void Item::setDescription(const std::string& description) {
  if (description == m_description) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(description);
  m_description = description;
  if (!signalsDisabled()) {
    descriptionModified().fire(this, m_description);
  }
setHasChanges();
}

std::vector<Effect>& Item::effects() { return m_effects; }
void Item::setEffects(const std::vector<Effect>& effects) {
  if (effects == m_effects) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(effects);
  m_effects = effects;
  if (!signalsDisabled()) {
    effectsModified().fire(this, m_effects);
  }
setHasChanges();
}

[[nodiscard]] HitType Item::hitType() const { return m_hitType; }
void Item::setHitType(const HitType hitType) {
  if (hitType == m_hitType) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(hitType);
  m_hitType = hitType;
  if (!signalsDisabled()) {
    hitTypeModified().fire(this, m_hitType);
  }
setHasChanges();
}

[[nodiscard]] int Item::iconIndex() const { return m_iconIndex; }
void Item::setIconIndex(const int iconIndex) {
  if (iconIndex == m_iconIndex) {
    return;
  }
  m_iconIndex = iconIndex;
  if (!signalsDisabled()) {
    iconIndexModified().fire(this, m_iconIndex);
  }
setHasChanges();
}

[[nodiscard]] int Item::itypeId() const { return m_itypeId; }
void Item::setItypeId(const int itypeId) {
  if (itypeId == m_itypeId) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(itypeId);
  m_itypeId = itypeId;
  if (!signalsDisabled()) {
    itypeIdModified().fire(this, m_itypeId);
  }
setHasChanges();
}

[[nodiscard]] const std::string& Item::name() const { return m_name; }
void Item::setName(const std::string& name) {
  if (name == m_name) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(name);
  m_name = name;
  if (!signalsDisabled()) {
    nameModified().fire(this, m_name);
  }
setHasChanges();
}

[[nodiscard]] const std::string& Item::note() const { return m_note; }
void Item::setNote(const std::string& note) {
  if (note == m_note) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(note);
  m_note = note;
  if (!signalsDisabled()) {
    noteModified().fire(this, m_note);
  }
setHasChanges();
}

[[nodiscard]] Occasion Item::occasion() const { return m_occasion; }
void Item::setOccasion(const Occasion occasion) {
  if (occasion == m_occasion) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(occasion);
  m_occasion = occasion;
  if (!signalsDisabled()) {
    occasionModified().fire(this, m_occasion);
  }
setHasChanges();
}

[[nodiscard]] int Item::price() const { return m_price; }
void Item::setPrice(const int price) {
  if (price == m_price) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(price);
  m_price = price;
  if (!signalsDisabled()) {
    priceModified().fire(this, m_price);
  }
setHasChanges();
}

[[nodiscard]] int Item::repeats() const { return m_repeats; }
void Item::setRepeats(const int repeats) {
  if (repeats == m_repeats) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(repeats);
  m_repeats = repeats;
  if (!signalsDisabled()) {
    repeatsModified().fire(this, m_repeats);
  }
setHasChanges();
}

[[nodiscard]] Scope Item::scope() const { return m_scope; }
void Item::setScope(const Scope scope) {
  if (scope == m_scope) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(scope);
  m_scope = scope;
  if (!signalsDisabled()) {
    scopeModified().fire(this, m_scope);
  }
setHasChanges();
}

[[nodiscard]] int Item::speed() const { return m_speed; }
void Item::setSpeed(const int speed) {
  if (speed == m_speed) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(speed);
  m_speed = speed;
  if (!signalsDisabled()) {
    speedModified().fire(this, m_speed);
  }
setHasChanges();
}

[[nodiscard]] int Item::successRate() const { return m_successRate; }
void Item::setSuccessRate(const int successRate) {
  if (successRate == m_successRate) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(successRate);
  m_successRate = successRate;
  if (!signalsDisabled()) {
    successRateModified().fire(this, m_successRate);
  }
setHasChanges();
}

[[nodiscard]] int Item::tpGain() const { return m_tpGain; }
void Item::setTpGain(const int tpGain) {
  if (tpGain == m_tpGain) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(tpGain);
  m_tpGain = tpGain;
  if (!signalsDisabled()) {
    tpGainModified().fire(this, m_tpGain);
  }
setHasChanges();
}

void Item::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(id);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(animationId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(consumable);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(damage);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(description);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(effects);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(hitType);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(iconIndex);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(itypeId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(note);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(occasion);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(repeats);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(scope);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(successRate);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(tpGain);
}

void Item::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(id);
  MODIFIABLE_ACCEPT_VALUE(animationId);
  MODIFIABLE_ACCEPT_VALUE(consumable);
  MODIFIABLE_ACCEPT_VALUE(damage);
  MODIFIABLE_ACCEPT_VALUE(description);
  MODIFIABLE_ACCEPT_VALUE(effects);
  MODIFIABLE_ACCEPT_VALUE(hitType);
  MODIFIABLE_ACCEPT_VALUE(iconIndex);
  MODIFIABLE_ACCEPT_VALUE(itypeId);
  MODIFIABLE_ACCEPT_VALUE(name);
  MODIFIABLE_ACCEPT_VALUE(note);
  MODIFIABLE_ACCEPT_VALUE(occasion);
  MODIFIABLE_ACCEPT_VALUE(repeats);
  MODIFIABLE_ACCEPT_VALUE(scope);
  MODIFIABLE_ACCEPT_VALUE(successRate);
  MODIFIABLE_ACCEPT_VALUE(tpGain);
}

nlohmann::ordered_json Item::serializeOldValues() const {

  return {
      MODIFIABLE_SERIALIZE_VALUE(id),
      MODIFIABLE_SERIALIZE_VALUE(animationId),
      MODIFIABLE_SERIALIZE_VALUE(damage),
      MODIFIABLE_SERIALIZE_VALUE(description),
      MODIFIABLE_SERIALIZE_VALUE_CHILD_ARRAY(effects),
      MODIFIABLE_SERIALIZE_VALUE(hitType),
      MODIFIABLE_SERIALIZE_VALUE(iconIndex),
      MODIFIABLE_SERIALIZE_VALUE(name),
      MODIFIABLE_SERIALIZE_VALUE(note),
      MODIFIABLE_SERIALIZE_VALUE(occasion),
      MODIFIABLE_SERIALIZE_VALUE(price),
      MODIFIABLE_SERIALIZE_VALUE(repeats),
      MODIFIABLE_SERIALIZE_VALUE(scope),
      MODIFIABLE_SERIALIZE_VALUE(speed),
      MODIFIABLE_SERIALIZE_VALUE(successRate),
      MODIFIABLE_SERIALIZE_VALUE(tpGain),
  };
}

rpgmutils::signal<void(Item*, int)>& Item::idModified() {
  if (!m_idModified) {
    m_idModified.emplace();
  }
  return *m_idModified;
}
rpgmutils::signal<void(Item*, int)>& Item::animationIdModified() {
  if (!m_animationModified) {
    m_animationModified.emplace();
  }
  return *m_animationModified;
}
rpgmutils::signal<void(Item*, bool)>& Item::consumableModified() {
  if (!m_consumableModified) {
    m_consumableModified.emplace();
  }
  return *m_consumableModified;
}
rpgmutils::signal<void(Item*, const Damage&)>& Item::damageModified() {
  if (!m_damageModified) {
    m_damageModified.emplace();
  }
  return *m_damageModified;
}
rpgmutils::signal<void(Item*, const std::string& description)>& Item::descriptionModified() {
  if (!m_descriptionModified) {
    m_descriptionModified.emplace();
  }
  return *m_descriptionModified;
}
rpgmutils::signal<void(Item*, const std::vector<Effect>&)>& Item::effectsModified() {
  if (!m_effectsModified) {
    m_effectsModified.emplace();
  }
  return *m_effectsModified;
}
rpgmutils::signal<void(Item*, HitType)>& Item::hitTypeModified() {
  if (!m_hitTypeModified) {
    m_hitTypeModified.emplace();
  }
  return *m_hitTypeModified;
}
rpgmutils::signal<void(Item*, int)>& Item::iconIndexModified() {
  if (!m_iconIndexModified) {
    m_iconIndexModified.emplace();
  }
  return *m_iconIndexModified;
}
rpgmutils::signal<void(Item*, int)>& Item::itypeIdModified() {
  if (!m_itypeIdModified) {
    m_itypeIdModified.emplace();
  }
  return *m_itypeIdModified;
}
rpgmutils::signal<void(Item*, const std::string&)>& Item::nameModified() {
  if (!m_nameModified) {
    m_nameModified.emplace();
  }
  return *m_nameModified;
}
rpgmutils::signal<void(Item*, const std::string&)>& Item::noteModified() {
  if (!m_noteModified) {
    m_noteModified.emplace();
  }
  return *m_noteModified;
}
rpgmutils::signal<void(Item*, Occasion)>& Item::occasionModified() {
  if (!m_occasionModified) {
    m_occasionModified.emplace();
  }
  return *m_occasionModified;
}
rpgmutils::signal<void(Item*, int)>& Item::priceModified() {
  if (!m_priceModified) {
    m_priceModified.emplace();
  }
  return *m_priceModified;
}
rpgmutils::signal<void(Item*, int)>& Item::repeatsModified() {
  if (!m_repeatsModified) {
    m_repeatsModified.emplace();
  }
  return *m_repeatsModified;
}
rpgmutils::signal<void(Item*, Scope)>& Item::scopeModified() {
  if (!m_scopeModified) {
    m_scopeModified.emplace();
  }
  return *m_scopeModified;
}
rpgmutils::signal<void(Item*, int)>& Item::speedModified() {
  if (!m_speedModified) {
    m_speedModified.emplace();
  }
  return *m_speedModified;
}
rpgmutils::signal<void(Item*, int)>& Item::successRateModified() {
  if (!m_successRateModified) {
    m_successRateModified.emplace();
  }
  return *m_successRateModified;
}
rpgmutils::signal<void(Item*, int)>& Item::tpGainModified() {
  if (!m_tpGainModified) {
    m_tpGainModified.emplace();
  }
  return *m_tpGainModified;
}

void to_json(nlohmann::ordered_json& json, const Item& item) {
  json = {
      {"id", item.m_id},           {"animationId", item.m_animationId}, {"consumable", item.m_consumable}, {"damage", item.m_damage},   {"description", item.m_description},
      {"effects", item.m_effects}, {"hitType", item.m_hitType},         {"iconIndex", item.m_iconIndex},   {"itypeId", item.m_itypeId}, {"name", item.m_name},
      {"note", item.m_note},       {"occasion", item.m_occasion},       {"price", item.m_price},           {"repeats", item.m_repeats}, {"scope", item.m_scope},
      {"speed", item.m_speed},     {"successRate", item.m_successRate}, {"tpGain", item.m_tpGain},
  };
}
void from_json(const nlohmann::ordered_json& json, Item& item) {
  item.m_id = json.value("id", item.m_id);
  item.m_animationId = json.value("animationId", item.m_animationId);
  item.m_consumable = json.value("consumable", item.m_consumable);
  item.m_damage = json.value("damage", item.m_damage);
  item.m_description = json.value("description", item.m_description);
  item.m_effects = json.value("effects", item.m_effects);
  item.m_hitType = json.value("hitType", item.m_hitType);
  item.m_iconIndex = json.value("iconIndex", item.m_iconIndex);
  item.m_itypeId = json.value("itypeId", item.m_itypeId);
  item.m_name = json.value("name", item.m_name);
  item.m_note = json.value("note", item.m_note);
  item.m_occasion = json.value("occasion", item.m_occasion);
  item.m_price = json.value("price", item.m_price);
  item.m_repeats = json.value("repeats", item.m_repeats);
  item.m_scope = json.value("scope", item.m_scope);
  item.m_speed = json.value("speed", item.m_speed);
  item.m_successRate = json.value("successRate", item.m_successRate);
  item.m_tpGain = json.value("tpGain", item.m_tpGain);
}
