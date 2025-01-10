#pragma once

#include "Database/Damage.hpp"
#include "Database/Effect.hpp"
#include "Database/Globals.hpp"

#include <nlohmann/json.hpp>
#include <string>

class Skill final : public IModifiable {
  friend class SkillsSerializer;
  friend void to_json(nlohmann::ordered_json& j, const Skill& skill);
  friend void from_json(const nlohmann::ordered_json& j, Skill& skill);

public:
  Skill() = default;
  Skill(const Skill& other);
  Skill& operator=(const Skill& other);
  Skill(Skill&& other) noexcept;
  Skill& operator=(Skill&& other) noexcept;

  [[nodiscard]] int id() const;
  void setId(int id);

  [[nodiscard]] int animationId() const;
  void setAnimationId(int id);

  Damage& damage();
  void setDamage(const Damage& damage);

  [[nodiscard]] const std::string& description() const;
  void setDescription(const std::string& description);

  std::vector<Effect>& effects();
  void setEffects(const std::vector<Effect>& effects);

  [[nodiscard]] int iconIndex() const;
  void setIconIndex(int iconIndex);

  [[nodiscard]] const std::string& message1() const;
  void setMessage1(const std::string& message1);

  [[nodiscard]] const std::string& message2() const;
  void setMessage2(const std::string& message2);

  [[nodiscard]] int mpCost() const;
  void setMpCost(int mpCost);

  [[nodiscard]] const std::string& name() const;
  void setName(const std::string& name);

  [[nodiscard]] const std::string& note() const;
  void setNote(const std::string& note);

  [[nodiscard]] Occasion occasion() const;
  void setOccasion(Occasion occasion);

  [[nodiscard]] HitType hitType() const;
  void setHitType(HitType hitType);

  [[nodiscard]] int repeats() const;
  void setRepeats(int repeats);

  [[nodiscard]] int requiredWtypeId1() const;
  void setRequiredWtypeId1(int requiredWtypeId1);

  [[nodiscard]] int requiredWtypeId2() const;
  void setRequiredWtypeId2(int requiredWtypeId2);

  [[nodiscard]] Scope scope() const;
  void setScope(Scope scope);

  [[nodiscard]] int speed() const;
  void setSpeed(int speed);

  [[nodiscard]] int stypeId() const;
  void setStypeId(int stypeId);

  [[nodiscard]] int successRate() const;
  void setSuccessRate(int successRate);

  [[nodiscard]] int tpCost() const;
  void setTpCost(int tpCost);

  [[nodiscard]] int tpGain() const;
  void setTpGain(int tpGain);

  [[nodiscard]] bool isValid() const;
  void setValid(bool isValid);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  bool isModified() const override { return IModifiable::isModified() | m_damage.isModified(); }

  rpgmutils::signal<void(Skill*, int)>& idModified();
  rpgmutils::signal<void(Skill*, int)>& animationIdModified();
  rpgmutils::signal<void(Skill*, const Damage&)>& damageModified();
  rpgmutils::signal<void(Skill*, const std::string&)>& descriptionModified();
  rpgmutils::signal<void(Skill*, const std::vector<Effect>&)>& effectsModified();
  rpgmutils::signal<void(Skill*, int)>& iconIndexModified();
  rpgmutils::signal<void(Skill*, const std::string&)>& message1Modified();
  rpgmutils::signal<void(Skill*, const std::string&)>& message2Modified();
  rpgmutils::signal<void(Skill*, int)>& mpCostModified();
  rpgmutils::signal<void(Skill*, const std::string&)>& nameModified();
  rpgmutils::signal<void(Skill*, const std::string&)>& noteModified();
  rpgmutils::signal<void(Skill*, Occasion)>& occasionModified();
  rpgmutils::signal<void(Skill*, HitType)>& hitTypeModified();
  rpgmutils::signal<void(Skill*, int)>& repeatsModified();
  rpgmutils::signal<void(Skill*, int)>& requiredWtypeId1Modified();
  rpgmutils::signal<void(Skill*, int)>& requiredWtypeId2Modified();
  rpgmutils::signal<void(Skill*, Scope)>& scopeModified();
  rpgmutils::signal<void(Skill*, int)>& speedModified();
  rpgmutils::signal<void(Skill*, int)>& stypeIdModified();
  rpgmutils::signal<void(Skill*, int)>& successRateModified();
  rpgmutils::signal<void(Skill*, int)>& tpCostModified();
  rpgmutils::signal<void(Skill*, int)>& tpGainModified();

private:
  int m_id;
  int m_animationId;
  Damage m_damage;
  std::string m_description;
  std::vector<Effect> m_effects;
  int m_iconIndex;
  std::string m_message1;
  std::string m_message2;
  int m_mpCost;
  std::string m_name;
  std::string m_note;
  Occasion m_occasion;
  HitType m_hitType;
  int m_repeats;
  int m_requiredWtypeId1; // In System?
  int m_requiredWtypeId2; // "" ""
  Scope m_scope;
  int m_speed;
  int m_stypeId; // As defined in System
  int m_successRate;
  int m_tpCost;
  int m_tpGain;

  std::optional<int> m_oldid;
  std::optional<int> m_oldanimationId;
  std::optional<Damage> m_olddamage;
  std::optional<std::string> m_olddescription;
  std::optional<std::vector<Effect>> m_oldeffects;
  std::optional<int> m_oldiconIndex;
  std::optional<std::string> m_oldmessage1;
  std::optional<std::string> m_oldmessage2;
  std::optional<int> m_oldmpCost;
  std::optional<std::string> m_oldname;
  std::optional<std::string> m_oldnote;
  std::optional<Occasion> m_oldoccasion;
  std::optional<HitType> m_oldhitType;
  std::optional<int> m_oldrepeats;
  std::optional<int> m_oldrequiredWtypeId1;
  std::optional<int> m_oldrequiredWtypeId2;
  std::optional<Scope> m_oldscope;
  std::optional<int> m_oldspeed;
  std::optional<int> m_oldstypeId;
  std::optional<int> m_oldsuccessRate;
  std::optional<int> m_oldtpCost;
  std::optional<int> m_oldtpGain;

  std::optional<rpgmutils::signal<void(Skill*, int)>> m_idModified;
  std::optional<rpgmutils::signal<void(Skill*, int)>> m_animationIdModified;
  std::optional<rpgmutils::signal<void(Skill*, const Damage&)>> m_damageModified;
  std::optional<rpgmutils::signal<void(Skill*, const std::string&)>> m_descriptionModified;
  std::optional<rpgmutils::signal<void(Skill*, const std::vector<Effect>&)>> m_effectsModified;
  std::optional<rpgmutils::signal<void(Skill*, int)>> m_iconIndexModified;
  std::optional<rpgmutils::signal<void(Skill*, const std::string&)>> m_message1Modified;
  std::optional<rpgmutils::signal<void(Skill*, const std::string&)>> m_message2Modified;
  std::optional<rpgmutils::signal<void(Skill*, int)>> m_mpCostModified;
  std::optional<rpgmutils::signal<void(Skill*, const std::string&)>> m_nameModified;
  std::optional<rpgmutils::signal<void(Skill*, const std::string&)>> m_noteModified;
  std::optional<rpgmutils::signal<void(Skill*, Occasion)>> m_occasionModified;
  std::optional<rpgmutils::signal<void(Skill*, HitType)>> m_hitTypeModified;
  std::optional<rpgmutils::signal<void(Skill*, int)>> m_repeatsModified;
  std::optional<rpgmutils::signal<void(Skill*, int)>> m_requiredWtypeId1Modified;
  std::optional<rpgmutils::signal<void(Skill*, int)>> m_requiredWtypeId2Modified;
  std::optional<rpgmutils::signal<void(Skill*, Scope)>> m_scopeModified;
  std::optional<rpgmutils::signal<void(Skill*, int)>> m_speedModified;
  std::optional<rpgmutils::signal<void(Skill*, int)>> m_stypeIdModified;
  std::optional<rpgmutils::signal<void(Skill*, int)>> m_successRateModified;
  std::optional<rpgmutils::signal<void(Skill*, int)>> m_tpCostModified;
  std::optional<rpgmutils::signal<void(Skill*, int)>> m_tpGainModified;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};