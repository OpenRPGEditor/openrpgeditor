#pragma once

#include "Database/Damage.hpp"
#include "Database/Effect.hpp"
#include "Database/Globals.hpp"

class Item final : public IModifiable {
  friend class ItemsSerializer;

public:
  friend void to_json(nlohmann::ordered_json& json, const Item& item);
  friend void from_json(const nlohmann::ordered_json& json, Item& item);

  Item() = default;
  Item(const Item& other);
  Item& operator=(const Item& other);
  Item(Item&& other) noexcept;
  Item& operator=(Item&& other) noexcept;

  [[nodiscard]] int id() const;
  void setId(int id);

  [[nodiscard]] int animationId() const;
  void setAnimationId(int animationId);

  [[nodiscard]] bool consumable() const;
  void setConsumable(bool consumable);

  Damage& damage();
  void setDamage(const Damage& damage);

  [[nodiscard]] const std::string& description() const;
  void setDescription(const std::string& description);

  std::vector<Effect>& effects();
  void setEffects(const std::vector<Effect>& effects);

  [[nodiscard]] HitType hitType() const;
  void setHitType(HitType hitType);

  [[nodiscard]] int iconIndex() const;
  void setIconIndex(int iconIndex);

  [[nodiscard]] int itypeId() const;
  void setItypeId(int itypeId);

  [[nodiscard]] const std::string& name() const;
  void setName(const std::string& name);

  [[nodiscard]] const std::string& note() const;
  void setNote(const std::string& note);

  [[nodiscard]] Occasion occasion() const;
  void setOccasion(Occasion occasion);

  [[nodiscard]] int price() const;
  void setPrice(int price);

  [[nodiscard]] int repeats() const;
  void setRepeats(int repeats);

  [[nodiscard]] Scope scope() const;
  void setScope(Scope scope);

  [[nodiscard]] int speed() const;
  void setSpeed(int speed);

  [[nodiscard]] int successRate() const;
  void setSuccessRate(int successRate);

  [[nodiscard]] int tpGain() const;
  void setTpGain(int tpGain);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  rpgmutils::signal<void(Item*, int)>& idModified();
  rpgmutils::signal<void(Item*, int)>& animationIdModified();
  rpgmutils::signal<void(Item*, bool)>& consumableModified();
  rpgmutils::signal<void(Item*, const Damage&)>& damageModified();
  rpgmutils::signal<void(Item*, const std::string& description)>& descriptionModified();
  rpgmutils::signal<void(Item*, const std::vector<Effect>&)>& effectsModified();
  rpgmutils::signal<void(Item*, HitType)>& hitTypeModified();
  rpgmutils::signal<void(Item*, int)>& iconIndexModified();
  rpgmutils::signal<void(Item*, int)>& itypeIdModified();
  rpgmutils::signal<void(Item*, const std::string&)>& nameModified();
  rpgmutils::signal<void(Item*, const std::string&)>& noteModified();
  rpgmutils::signal<void(Item*, Occasion)>& occasionModified();
  rpgmutils::signal<void(Item*, int)>& priceModified();
  rpgmutils::signal<void(Item*, int)>& repeatsModified();
  rpgmutils::signal<void(Item*, Scope)>& scopeModified();
  rpgmutils::signal<void(Item*, int)>& speedModified();
  rpgmutils::signal<void(Item*, int)>& successRateModified();
  rpgmutils::signal<void(Item*, int)>& tpGainModified();

  [[nodiscard]] bool isValid() const { return m_isValid; }
  void setValid(const bool valid) { m_isValid = valid; }

private:
  int m_id;
  int m_animationId;
  bool m_consumable;
  Damage m_damage;
  std::string m_description;
  std::vector<Effect> m_effects;
  HitType m_hitType;
  int m_iconIndex;
  int m_itypeId;
  std::string m_name;
  std::string m_note;
  Occasion m_occasion;
  int m_price;
  int m_repeats;
  Scope m_scope;
  int m_speed;
  int m_successRate;
  int m_tpGain;

  std::optional<int> m_oldid;
  std::optional<int> m_oldanimationId;
  std::optional<bool> m_oldconsumable;
  std::optional<Damage> m_olddamage;
  std::optional<std::string> m_olddescription;
  std::optional<std::vector<Effect>> m_oldeffects;
  std::optional<HitType> m_oldhitType;
  std::optional<int> m_oldiconIndex;
  std::optional<int> m_olditypeId;
  std::optional<std::string> m_oldname;
  std::optional<std::string> m_oldnote;
  std::optional<Occasion> m_oldoccasion;
  std::optional<int> m_oldprice;
  std::optional<int> m_oldrepeats;
  std::optional<Scope> m_oldscope;
  std::optional<int> m_oldspeed;
  std::optional<int> m_oldsuccessRate;
  std::optional<int> m_oldtpGain;

  std::optional<rpgmutils::signal<void(Item*, int)>> m_idModified;
  std::optional<rpgmutils::signal<void(Item*, int)>> m_animationModified;
  std::optional<rpgmutils::signal<void(Item*, bool)>> m_consumableModified;
  std::optional<rpgmutils::signal<void(Item*, const Damage&)>> m_damageModified;
  std::optional<rpgmutils::signal<void(Item*, const std::string& description)>> m_descriptionModified;
  std::optional<rpgmutils::signal<void(Item*, const std::vector<Effect>&)>> m_effectsModified;
  std::optional<rpgmutils::signal<void(Item*, HitType)>> m_hitTypeModified;
  std::optional<rpgmutils::signal<void(Item*, int)>> m_iconIndexModified;
  std::optional<rpgmutils::signal<void(Item*, int)>> m_itypeIdModified;
  std::optional<rpgmutils::signal<void(Item*, const std::string&)>> m_nameModified;
  std::optional<rpgmutils::signal<void(Item*, const std::string&)>> m_noteModified;
  std::optional<rpgmutils::signal<void(Item*, Occasion)>> m_occasionModified;
  std::optional<rpgmutils::signal<void(Item*, int)>> m_priceModified;
  std::optional<rpgmutils::signal<void(Item*, int)>> m_repeatsModified;
  std::optional<rpgmutils::signal<void(Item*, Scope)>> m_scopeModified;
  std::optional<rpgmutils::signal<void(Item*, int)>> m_speedModified;
  std::optional<rpgmutils::signal<void(Item*, int)>> m_successRateModified;
  std::optional<rpgmutils::signal<void(Item*, int)>> m_tpGainModified;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};
void to_json(nlohmann::ordered_json& json, const Item& item);
void from_json(const nlohmann::ordered_json& json, Item& item);