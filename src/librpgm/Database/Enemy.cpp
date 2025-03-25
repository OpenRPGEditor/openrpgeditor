#include "Enemy.hpp"

Enemy::Action::Action(const Action& other)
: IModifiable(other)
, m_conditionParam1(other.m_conditionParam1)
, m_conditionParam2(other.m_conditionParam2)
, m_conditionType(other.m_conditionType)
, m_rating(other.m_rating)
, m_skillId(other.m_skillId)
, m_oldconditionParam1(other.m_oldconditionParam1)
, m_oldconditionParam2(other.m_oldconditionParam2)
, m_oldconditionType(other.m_oldconditionType)
, m_oldrating(other.m_oldrating)
, m_oldskillId(other.m_oldskillId) {}

Enemy::Action& Enemy::Action::operator=(const Action& other) {
  IModifiable::operator=(other);
  m_conditionParam1 = other.m_conditionParam1;
  m_conditionParam2 = other.m_conditionParam2;
  m_conditionType = other.m_conditionType;
  m_rating = other.m_rating;
  m_skillId = other.m_skillId;
  m_oldconditionParam1 = other.m_oldconditionParam1;
  m_oldconditionParam2 = other.m_oldconditionParam2;
  m_oldconditionType = other.m_oldconditionType;
  m_oldrating = other.m_oldrating;
  m_oldskillId = other.m_oldskillId;
  return *this;
}
Enemy::Action::Action(Action&& other) noexcept
: IModifiable(std::move(other))
, m_conditionParam1(other.m_conditionParam1)
, m_conditionParam2(other.m_conditionParam2)
, m_conditionType(other.m_conditionType)
, m_rating(other.m_rating)
, m_skillId(other.m_skillId)
, m_oldconditionParam1(other.m_oldconditionParam1)
, m_oldconditionParam2(other.m_oldconditionParam2)
, m_oldconditionType(other.m_oldconditionType)
, m_oldrating(other.m_oldrating)
, m_oldskillId(other.m_oldskillId) {}

Enemy::Action& Enemy::Action::operator=(Action&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_conditionParam1 = other.m_conditionParam1;
  m_conditionParam2 = other.m_conditionParam2;
  m_conditionType = other.m_conditionType;
  m_rating = other.m_rating;
  m_skillId = other.m_skillId;
  m_oldconditionParam1 = other.m_oldconditionParam1;
  m_oldconditionParam2 = other.m_oldconditionParam2;
  m_oldconditionType = other.m_oldconditionType;
  m_oldrating = other.m_oldrating;
  m_oldskillId = other.m_oldskillId;
  return *this;
}

float Enemy::Action::conditionParam1() const { return m_conditionParam1; }
void Enemy::Action::setConditionParam1(const float conditionParam1) {
  MODIFIABLE_SET_OLD_VALUE(conditionParam1);
  m_conditionParam1 = conditionParam1;
  if (!signalsDisabled()) {
    conditionParam1Modified().fire(this, conditionParam1);
  }
  setHasChanges();
}

float Enemy::Action::conditionParam2() const { return m_conditionParam2; }
void Enemy::Action::setConditionParam2(const float conditionParam2) {
  MODIFIABLE_SET_OLD_VALUE(conditionParam2);
  m_conditionParam2 = conditionParam2;
  if (!signalsDisabled()) {
    conditionParam2Modified().fire(this, conditionParam2);
  }
  setHasChanges();
}

ActionCondition Enemy::Action::conditionType() const { return m_conditionType; }
void Enemy::Action::setConditionType(const ActionCondition conditionType) {
  MODIFIABLE_SET_OLD_VALUE(conditionType);
  m_conditionType = conditionType;
  if (!signalsDisabled()) {
    conditionTypeModified().fire(this, conditionType);
  }
  setHasChanges();
}

int Enemy::Action::rating() const { return m_rating; }
void Enemy::Action::setRating(const int rating) {
  MODIFIABLE_SET_OLD_VALUE(rating);
  m_rating = rating;
  if (!signalsDisabled()) {
    ratingModified().fire(this, rating);
  }
  setHasChanges();
}

int Enemy::Action::skillId() const { return m_skillId; }
void Enemy::Action::setSkillId(int skillId) {
  MODIFIABLE_SET_OLD_VALUE(skillId);
  m_skillId = skillId;
  if (!signalsDisabled()) {
    skillIdModified().fire(this, skillId);
  }
  setHasChanges();
}

void Enemy::Action::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(conditionParam1);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(conditionParam2);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(rating);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(skillId);
}

void Enemy::Action::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(conditionParam1);
  MODIFIABLE_ACCEPT_VALUE(conditionParam2);
  MODIFIABLE_ACCEPT_VALUE(rating);
  MODIFIABLE_ACCEPT_VALUE(skillId);
}

nlohmann::ordered_json Enemy::Action::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(conditionParam1),
      MODIFIABLE_SERIALIZE_VALUE(conditionParam2),
      MODIFIABLE_SERIALIZE_VALUE(rating),
      MODIFIABLE_SERIALIZE_VALUE(skillId),
  };
}

rpgmutils::signal<void(Enemy::Action*, float)>& Enemy::Action::conditionParam1Modified() {
  if (!m_conditionParam1Modified) {
    m_conditionParam1Modified.emplace();
  }
  return *m_conditionParam1Modified;
}

rpgmutils::signal<void(Enemy::Action*, float)>& Enemy::Action::conditionParam2Modified() {
  if (!m_conditionParam2Modified) {
    m_conditionParam2Modified.emplace();
  }
  return *m_conditionParam2Modified;
}

rpgmutils::signal<void(Enemy::Action*, ActionCondition)>& Enemy::Action::conditionTypeModified() {
  if (!m_conditionTypeModified) {
    m_conditionTypeModified.emplace();
  }
  return *m_conditionTypeModified;
}

rpgmutils::signal<void(Enemy::Action*, int)>& Enemy::Action::ratingModified() {
  if (!m_ratingModified) {
    m_ratingModified.emplace();
  }
  return *m_ratingModified;
}

rpgmutils::signal<void(Enemy::Action*, int)>& Enemy::Action::skillIdModified() {
  if (!m_skillIdModified) {
    m_skillIdModified.emplace();
  }
  return *m_skillIdModified;
}

Enemy::DropItem::DropItem(const DropItem& other)
: IModifiable(other)
, m_dataId(other.m_dataId)
, m_denominator(other.m_denominator)
, m_kind(other.m_kind)
, m_olddataId(other.m_olddataId)
, m_olddenominator(other.m_olddenominator)
, m_oldkind(other.m_oldkind) {}
Enemy::DropItem& Enemy::DropItem::operator=(const DropItem& other) {
  IModifiable::operator=(other);
  m_dataId = other.m_dataId;
  m_denominator = other.m_denominator;
  m_kind = other.m_kind;
  m_olddataId = other.m_olddataId;
  m_olddenominator = other.m_olddenominator;
  m_oldkind = other.m_oldkind;
  return *this;
}
Enemy::DropItem::DropItem(DropItem&& other) noexcept
: IModifiable(std::move(other))
, m_dataId(other.m_dataId)
, m_denominator(other.m_denominator)
, m_kind(other.m_kind)
, m_olddataId(other.m_olddataId)
, m_olddenominator(other.m_olddenominator)
, m_oldkind(other.m_oldkind) {}

Enemy::DropItem& Enemy::DropItem::operator=(DropItem&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_dataId = other.m_dataId;
  m_denominator = other.m_denominator;
  m_kind = other.m_kind;
  m_olddataId = other.m_olddataId;
  m_olddenominator = other.m_olddenominator;
  m_oldkind = other.m_oldkind;
  return *this;
}

int Enemy::DropItem::dataId() const { return m_dataId; }
void Enemy::DropItem::setDataId(const int dataId) {
  MODIFIABLE_SET_OLD_VALUE(dataId);
  m_dataId = dataId;
  if (!signalsDisabled()) {
    dataIdModified().fire(this, dataId);
  }
  setHasChanges();
}

int Enemy::DropItem::denominator() const { return m_denominator; }
void Enemy::DropItem::setDenominator(const int denominator) {
  MODIFIABLE_SET_OLD_VALUE(denominator);
  m_denominator = denominator;
  if (!signalsDisabled()) {
    denominatorModified().fire(this, denominator);
  }
  setHasChanges();
}

int Enemy::DropItem::kind() const { return m_kind; }
void Enemy::DropItem::setKind(const int kind) {
  MODIFIABLE_SET_OLD_VALUE(kind);
  m_kind = kind;
  if (!signalsDisabled()) {
    kindModified().fire(this, kind);
  }
  setHasChanges();
}

void Enemy::DropItem::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(dataId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(denominator);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(kind);
}

void Enemy::DropItem::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(dataId);
  MODIFIABLE_ACCEPT_VALUE(denominator);
  MODIFIABLE_ACCEPT_VALUE(kind);
}

nlohmann::ordered_json Enemy::DropItem::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(dataId),
      MODIFIABLE_SERIALIZE_VALUE(denominator),
      MODIFIABLE_SERIALIZE_VALUE(kind),
  };
}
rpgmutils::signal<void(Enemy::DropItem*, int)>& Enemy::DropItem::dataIdModified() {
  if (!m_dataIdModified) {
    m_dataIdModified.emplace();
  }
  return *m_dataIdModified;
}
rpgmutils::signal<void(Enemy::DropItem*, int)>& Enemy::DropItem::denominatorModified() {
  if (!m_denominatorModified) {
    m_denominatorModified.emplace();
  }
  return *m_denominatorModified;
}
rpgmutils::signal<void(Enemy::DropItem*, int)>& Enemy::DropItem::kindModified() {
  if (!m_kindModified) {
    m_kindModified.emplace();
  }
  return *m_kindModified;
}

Enemy::Enemy(const Enemy& other)
: IModifiable(other)
, m_id(other.m_id)
, m_actions(other.m_actions)
, m_battlerHue(other.m_battlerHue)
, m_battlerName(other.m_battlerName)
, m_dropItems(other.m_dropItems)
, m_exp(other.m_exp)
, m_traits(other.m_traits)
, m_gold(other.m_gold)
, m_name(other.m_name)
, m_note(other.m_note)
, m_params(other.m_params)
, m_oldid(other.m_oldid)
, m_oldactions(other.m_oldactions)
, m_oldbattlerHue(other.m_oldbattlerHue)
, m_oldbattlerName(other.m_oldbattlerName)
, m_oldexp(other.m_oldexp)
, m_oldtraits(other.m_oldtraits)
, m_oldgold(other.m_oldgold)
, m_oldname(other.m_oldname)
, m_oldnote(other.m_oldnote)
, m_oldparams(other.m_oldparams)
, m_isValid(other.m_isValid) {}
Enemy& Enemy::operator=(const Enemy& other) {
  IModifiable::operator=(other);
  m_id = other.m_id;
  m_actions = other.m_actions;
  m_battlerHue = other.m_battlerHue;
  m_battlerName = other.m_battlerName;
  m_dropItems = other.m_dropItems;
  m_exp = other.m_exp;
  m_traits = other.m_traits;
  m_gold = other.m_gold;
  m_name = other.m_name;
  m_note = other.m_note;
  m_params = other.m_params;
  m_oldid = other.m_oldid;
  m_oldactions = other.m_oldactions;
  m_oldbattlerHue = other.m_oldbattlerHue;
  m_oldbattlerName = other.m_oldbattlerName;
  m_oldexp = other.m_oldexp;
  m_oldtraits = other.m_oldtraits;
  m_oldgold = other.m_oldgold;
  m_oldname = other.m_oldname;
  m_oldnote = other.m_oldnote;
  m_oldparams = other.m_oldparams;
  m_isValid = other.m_isValid;
  return *this;
}
Enemy::Enemy(Enemy&& other) noexcept
: IModifiable(std::move(other))
, m_id(other.m_id)
, m_actions(std::move(other.m_actions))
, m_battlerHue(other.m_battlerHue)
, m_battlerName(std::move(other.m_battlerName))
, m_dropItems(std::move(other.m_dropItems))
, m_exp(other.m_exp)
, m_traits(std::move(other.m_traits))
, m_gold(other.m_gold)
, m_name(std::move(other.m_name))
, m_note(std::move(other.m_note))
, m_params(std::move(other.m_params))
, m_oldid(other.m_oldid)
, m_oldactions(std::move(other.m_oldactions))
, m_oldbattlerHue(std::move(other.m_oldbattlerHue))
, m_oldbattlerName(std::move(other.m_oldbattlerName))
, m_oldexp(other.m_oldexp)
, m_oldtraits(std::move(other.m_oldtraits))
, m_oldgold(other.m_oldgold)
, m_oldname(std::move(other.m_oldname))
, m_oldnote(std::move(other.m_oldnote))
, m_oldparams(std::move(other.m_oldparams))
, m_isValid(other.m_isValid) {}
Enemy& Enemy::operator=(Enemy&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_id = other.m_id;
  m_actions = std::move(other.m_actions);
  m_battlerHue = other.m_battlerHue;
  m_battlerName = std::move(other.m_battlerName);
  m_dropItems = std::move(other.m_dropItems);
  m_exp = other.m_exp;
  m_traits = std::move(other.m_traits);
  m_gold = other.m_gold;
  m_name = std::move(other.m_name);
  m_note = std::move(other.m_note);
  m_params = std::move(other.m_params);
  m_oldid = other.m_oldid;
  m_oldactions = std::move(other.m_oldactions);
  m_oldbattlerHue = other.m_oldbattlerHue;
  m_oldbattlerName = std::move(other.m_oldbattlerName);
  m_oldexp = other.m_oldexp;
  m_oldtraits = std::move(other.m_oldtraits);
  m_oldgold = other.m_oldgold;
  m_oldname = std::move(other.m_oldname);
  m_oldnote = std::move(other.m_oldnote);
  m_oldparams = std::move(other.m_oldparams);
  m_isValid = other.m_isValid;
  return *this;
}

[[nodiscard]] int Enemy::id() const { return m_id; }
void Enemy::setId(const int id) {
  MODIFIABLE_SET_OLD_VALUE(id);
  m_id = id;
  if (!signalsDisabled()) {
    idModified().fire(this, id);
  }
  setHasChanges();
}

std::vector<Enemy::Action>& Enemy::actions() { return m_actions; }
const std::vector<Enemy::Action>& Enemy::actions() const { return m_actions; }
void Enemy::setActions(const std::vector<Action>& actions) {
  MODIFIABLE_SET_OLD_VALUE(actions);
  m_actions = actions;
  if (!signalsDisabled()) {
    actionsModified().fire(this, actions);
  }
  setHasChanges();
}

[[nodiscard]] int Enemy::battlerHue() const { return m_battlerHue; }
void Enemy::setBattlerHue(const int battlerHue) {
  MODIFIABLE_SET_OLD_VALUE(battlerHue);
  m_battlerHue = battlerHue;
  if (!signalsDisabled()) {
    battlerHueModified().fire(this, battlerHue);
  }
  setHasChanges();
}

const std::string& Enemy::battlerName() const { return m_battlerName; }
void Enemy::setBattlerName(const std::string& battlerName) {
  MODIFIABLE_SET_OLD_VALUE(battlerName);
  m_battlerName = battlerName;
  if (!signalsDisabled()) {
    battlerNameModified().fire(this, battlerName);
  }
  setHasChanges();
}

std::vector<Enemy::DropItem>& Enemy::dropItems() { return m_dropItems; }
const std::vector<Enemy::DropItem>& Enemy::dropItems() const { return m_dropItems; }
void Enemy::setDropItems(const std::vector<DropItem>& dropItems) {
  MODIFIABLE_SET_OLD_VALUE(dropItems);
  m_dropItems = dropItems;
  if (!signalsDisabled()) {
    dropItemsModified().fire(this, dropItems);
  }
  setHasChanges();
}

[[nodiscard]] int Enemy::exp() const { return m_exp; }
void Enemy::setExp(const int exp) {
  MODIFIABLE_SET_OLD_VALUE(exp);
  m_exp = exp;
  if (!signalsDisabled()) {
    expModified().fire(this, exp);
  }
  setHasChanges();
}

std::vector<Trait>& Enemy::traits() { return m_traits; }
void Enemy::setTraits(const std::vector<Trait>& traits) {
  MODIFIABLE_SET_OLD_VALUE(traits);
  m_traits = traits;
  if (!signalsDisabled()) {
    traitsModified().fire(this, traits);
  }
  setHasChanges();
}

[[nodiscard]] int Enemy::gold() const { return m_gold; }
void Enemy::setGold(const int gold) {
  MODIFIABLE_SET_OLD_VALUE(gold);
  m_gold = gold;
  if (!signalsDisabled()) {
    goldModified().fire(this, gold);
  }
  setHasChanges();
}

const std::string& Enemy::name() const { return m_name; }
void Enemy::setName(const std::string& name) {
  MODIFIABLE_SET_OLD_VALUE(name);
  m_name = name;
  if (!signalsDisabled()) {
    nameModified().fire(this, name);
  }
  setHasChanges();
}

const std::string& Enemy::note() const { return m_note; }
void Enemy::setNote(const std::string& note) {
  MODIFIABLE_SET_OLD_VALUE(note);
  m_note = note;
  if (!signalsDisabled()) {
    noteModified().fire(this, note);
  }
  setHasChanges();
}

const std::array<int, 8>& Enemy::params() const { return m_params; }
void Enemy::setParams(const std::array<int, 8>& params) {
  MODIFIABLE_SET_OLD_VALUE(params);
  m_params = params;
  if (!signalsDisabled()) {
    paramsModified().fire(this, params);
  }
  setHasChanges();
}
[[nodiscard]] int Enemy::param(const int idx) const {
  assert(idx >= 0 && idx < m_params.size());
  return m_params[idx];
}
void Enemy::setParam(const int idx, const int param) {
  assert(idx >= 0 && idx < m_params.size());
  MODIFIABLE_SET_OLD_VALUE(params);
  m_params[idx] = param;
  if (!signalsDisabled()) {
    paramModified().fire(this, idx, param);
  }
  setHasChanges();
}

bool Enemy::isValid() const { return m_isValid; }
void Enemy::setIsValid(const bool isValid) { m_isValid = isValid; }

void Enemy::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(id);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(actions);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(battlerHue);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(battlerName);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(dropItems);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(exp);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(traits);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(gold);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(note);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(params);
}

void Enemy::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(id);
  MODIFIABLE_ACCEPT_VALUE(actions);
  MODIFIABLE_ACCEPT_VALUE(battlerHue);
  MODIFIABLE_ACCEPT_VALUE(battlerName);
  MODIFIABLE_ACCEPT_VALUE(dropItems);
  MODIFIABLE_ACCEPT_VALUE(exp);
  MODIFIABLE_ACCEPT_VALUE(traits);
  MODIFIABLE_ACCEPT_VALUE(gold);
  MODIFIABLE_ACCEPT_VALUE(name);
  MODIFIABLE_ACCEPT_VALUE(note);
  MODIFIABLE_ACCEPT_VALUE(params);
}

nlohmann::ordered_json Enemy::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(id),
      MODIFIABLE_SERIALIZE_VALUE_CHILD_ARRAY(actions),
      MODIFIABLE_SERIALIZE_VALUE(battlerHue),
      MODIFIABLE_SERIALIZE_VALUE(battlerName),
      MODIFIABLE_SERIALIZE_VALUE_CHILD_ARRAY(dropItems),
      MODIFIABLE_SERIALIZE_VALUE(exp),
      MODIFIABLE_SERIALIZE_VALUE_CHILD_ARRAY(traits),
      MODIFIABLE_SERIALIZE_VALUE(gold),
      MODIFIABLE_SERIALIZE_VALUE(name),
      MODIFIABLE_SERIALIZE_VALUE(note),
      MODIFIABLE_SERIALIZE_VALUE(params),
  };
}
bool Enemy::isModified() const {
  bool modified = IModifiable::isModified();
  modified |= std::ranges::any_of(m_actions, [](const Action& action) { return action.isModified(); });
  modified |= std::ranges::any_of(m_dropItems, [](const DropItem& dropItem) { return dropItem.isModified(); });
  modified |= std::ranges::any_of(m_traits, [](const Trait& trait) { return trait.isModified(); });
  return modified;
}

rpgmutils::signal<void(Enemy*, int)>& Enemy::idModified() {
  if (!m_idModified) {
    m_idModified.emplace();
  }
  return *m_idModified;
}
rpgmutils::signal<void(Enemy*, const std::vector<Enemy::Action>&)>& Enemy::actionsModified() {
  if (!m_actionsModified) {
    m_actionsModified.emplace();
  }
  return *m_actionsModified;
}
rpgmutils::signal<void(Enemy*, int)>& Enemy::battlerHueModified() {
  if (!m_battlerHueModified) {
    m_battlerHueModified.emplace();
  }
  return *m_battlerHueModified;
}
rpgmutils::signal<void(Enemy*, const std::string&)>& Enemy::battlerNameModified() {
  if (!m_battlerNameModified) {
    m_battlerNameModified.emplace();
  }
  return *m_battlerNameModified;
}
rpgmutils::signal<void(Enemy*, const std::vector<Enemy::DropItem>&)>& Enemy::dropItemsModified() {
  if (!m_dropItemsModified) {
    m_dropItemsModified.emplace();
  }
  return *m_dropItemsModified;
}
rpgmutils::signal<void(Enemy*, int)>& Enemy::expModified() {
  if (!m_expModified) {
    m_expModified.emplace();
  }
  return *m_expModified;
}
rpgmutils::signal<void(Enemy*, const std::vector<Trait>&)>& Enemy::traitsModified() {
  if (!m_traitsModified) {
    m_traitsModified.emplace();
  }
  return *m_traitsModified;
}
rpgmutils::signal<void(Enemy*, int)>& Enemy::goldModified() {
  if (!m_goldModified) {
    m_goldModified.emplace();
  }
  return *m_goldModified;
}
rpgmutils::signal<void(Enemy*, const std::string&)>& Enemy::nameModified() {
  if (!m_nameModified) {
    m_nameModified.emplace();
  }
  return *m_nameModified;
}
rpgmutils::signal<void(Enemy*, const std::string&)>& Enemy::noteModified() {
  if (!m_noteModified) {
    m_noteModified.emplace();
  }
  return *m_noteModified;
}
rpgmutils::signal<void(Enemy*, const std::array<int, 8>&)>& Enemy::paramsModified() {
  if (!m_paramsModified) {
    m_paramsModified.emplace();
  }
  return *m_paramsModified;
}
rpgmutils::signal<void(Enemy*, int, int)>& Enemy::paramModified() {
  if (!m_paramModified) {
    m_paramModified.emplace();
  }
  return *m_paramModified;
}

void to_json(nlohmann::ordered_json& to, const Enemy& enemy) {
  to = {
      {"id", enemy.m_id},
      {"actions", enemy.m_actions},
      {"battlerHue", enemy.m_battlerHue},
      {"battlerName", enemy.m_battlerName},
      {"dropItems", enemy.m_dropItems},
      {"exp", enemy.m_exp},
      {"traits", enemy.m_traits},
      {"gold", enemy.m_gold},
      {"name", enemy.m_name},
      {"note", enemy.m_note},
      {"params", enemy.m_params},
  };
}
void from_json(const nlohmann::ordered_json& from, Enemy& enemy) {
  enemy.m_id = from.value("id", enemy.m_id);
  enemy.m_actions = from.value("actions", enemy.m_actions);
  enemy.m_battlerHue = from.value("battlerHue", enemy.m_battlerHue);
  enemy.m_battlerName = from.value("battlerName", enemy.m_battlerName);
  enemy.m_dropItems = from.value("dropItems", enemy.m_dropItems);
  enemy.m_exp = from.value("exp", enemy.m_exp);
  enemy.m_traits = from.value("traits", enemy.m_traits);
  enemy.m_gold = from.value("gold", enemy.m_gold);
  enemy.m_name = from.value("name", enemy.m_name);
  enemy.m_note = from.value("note", enemy.m_note);
  enemy.m_params = from.value("params", enemy.m_params);
}
void to_json(nlohmann::ordered_json& to, const Enemy::Action& action) {
  to = {
      {"conditionParam1", action.m_conditionParam1},
      {"conditionParam2", action.m_conditionParam2},
      {"conditionType", action.m_conditionType},
      {"rating", action.m_rating},
      {"skillId", action.m_skillId},
  };
}
void from_json(const nlohmann::ordered_json& from, Enemy::Action& action) {
  action.m_conditionParam1 = from.value("conditionParam1", action.m_conditionParam1);
  action.m_conditionParam2 = from.value("conditionParam2", action.m_conditionParam2);
  action.m_conditionType = from.value("conditionType", action.m_conditionType);
  action.m_rating = from.value("rating", action.m_rating);
  action.m_skillId = from.value("skillId", action.m_skillId);
}

void to_json(nlohmann::ordered_json& to, const Enemy::DropItem& dropitem) {
  to = {
      {"dataId", dropitem.m_dataId},
      {"denominator", dropitem.m_denominator},
      {"kind", dropitem.m_kind},
  };
}

void from_json(const nlohmann::ordered_json& from, Enemy::DropItem& dropitem) {
  dropitem.m_dataId = from.value("dataId", dropitem.m_dataId);
  dropitem.m_denominator = from.value("denominator", dropitem.m_denominator);
  dropitem.m_kind = from.value("kind", dropitem.m_kind);
}