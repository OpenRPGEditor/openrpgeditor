#pragma once
#include "Database/Globals.hpp"
#include "Database/Trait.hpp"

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class Enemy final : public IModifiable {
  friend class EnemiesSerializer;

public:
  friend void to_json(nlohmann::ordered_json& to, const Enemy& enemy);
  friend void from_json(const nlohmann::ordered_json& from, Enemy& enemy);

  class Action final : public IModifiable {
    friend void to_json(nlohmann::ordered_json& to, const Action& action);
    friend void from_json(const nlohmann::ordered_json& from, Action& action);

  public:
    Action() = default;
    Action(const Action& other);
    Action& operator=(const Action& other);
    Action(Action&& other) noexcept;
    Action& operator=(Action&& other) noexcept;
    float conditionParam1() const;
    void setConditionParam1(float conditionParam1);

    float conditionParam2() const;
    void setConditionParam2(float conditionParam2);

    ActionCondition conditionType() const;
    void setConditionType(ActionCondition conditionType);

    int rating() const;
    void setRating(int rating);

    int skillId() const;
    void setSkillId(int skillId);

    void restoreOriginal() override;
    void acceptChanges() override;
    nlohmann::ordered_json serializeOldValues() const override;

    rpgmutils::signal<void(Action*, float)>& conditionParam1Modified();
    rpgmutils::signal<void(Action*, float)>& conditionParam2Modified();
    rpgmutils::signal<void(Action*, ActionCondition)>& conditionTypeModified();
    rpgmutils::signal<void(Action*, int)>& ratingModified();
    rpgmutils::signal<void(Action*, int)>& skillIdModified();

  private:
    float m_conditionParam1{};
    float m_conditionParam2{};
    ActionCondition m_conditionType = ActionCondition::Always;
    int m_rating{5};
    int m_skillId{1};

    std::optional<float> m_oldconditionParam1;
    std::optional<float> m_oldconditionParam2;
    std::optional<ActionCondition> m_oldconditionType;
    std::optional<int> m_oldrating;
    std::optional<int> m_oldskillId;

    std::optional<rpgmutils::signal<void(Action*, float)>> m_conditionParam1Modified;
    std::optional<rpgmutils::signal<void(Action*, float)>> m_conditionParam2Modified;
    std::optional<rpgmutils::signal<void(Action*, ActionCondition)>> m_conditionTypeModified;
    std::optional<rpgmutils::signal<void(Action*, int)>> m_ratingModified;
    std::optional<rpgmutils::signal<void(Action*, int)>> m_skillIdModified;
  };

  class DropItem final : public IModifiable {
    friend void to_json(nlohmann::ordered_json& to, const DropItem& dropitem);
    friend void from_json(const nlohmann::ordered_json& from, DropItem& dropitem);

  public:
    DropItem() = default;
    DropItem(const DropItem& other);
    DropItem& operator=(const DropItem& other);
    DropItem(DropItem&& other) noexcept;
    DropItem& operator=(DropItem&& other) noexcept;

    int dataId() const;
    void setDataId(int dataId);

    int denominator() const;
    void setDenominator(int denominator);

    int kind() const;
    void setKind(int kind);

    void restoreOriginal() override;
    void acceptChanges() override;
    nlohmann::ordered_json serializeOldValues() const override;

    rpgmutils::signal<void(DropItem*, int)>& dataIdModified();
    rpgmutils::signal<void(DropItem*, int)>& denominatorModified();
    rpgmutils::signal<void(DropItem*, int)>& kindModified();

  private:
    int m_dataId;
    int m_denominator;
    int m_kind;
    std::optional<int> m_olddataId;
    std::optional<int> m_olddenominator;
    std::optional<int> m_oldkind;

    std::optional<rpgmutils::signal<void(DropItem*, int)>> m_dataIdModified;
    std::optional<rpgmutils::signal<void(DropItem*, int)>> m_denominatorModified;
    std::optional<rpgmutils::signal<void(DropItem*, int)>> m_kindModified;
  };

  Enemy() = default;
  Enemy(const Enemy& other);
  Enemy& operator=(const Enemy& other);
  Enemy(Enemy&& other) noexcept;
  Enemy& operator=(Enemy&& other) noexcept;

  [[nodiscard]] int id() const;
  void setId(int id);

  std::vector<Action>& actions();
  const std::vector<Action>& actions() const;
  void setActions(const std::vector<Action>& actions);

  [[nodiscard]] int battlerHue() const;
  void setBattlerHue(int battlerHue);

  const std::string& battlerName() const;
  void setBattlerName(const std::string& battlerName);

  std::vector<DropItem>& dropItems();
  const std::vector<DropItem>& dropItems() const;
  void setDropItems(const std::vector<DropItem>& dropItems);

  [[nodiscard]] int exp() const;
  void setExp(int exp);

  std::vector<Trait>& traits();
  void setTraits(const std::vector<Trait>& traits);

  [[nodiscard]] int gold() const;
  void setGold(int gold);

  const std::string& name() const;
  void setName(const std::string& name);

  const std::string& note() const;
  void setNote(const std::string& note);

  const std::array<int, 8>& params() const;
  void setParams(const std::array<int, 8>& params);
  [[nodiscard]] int param(int idx) const;
  void setParam(int idx, int param);

  bool isValid() const;
  void setIsValid(bool isValid);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  bool isModified() const override;

  rpgmutils::signal<void(Enemy*, int)>& idModified();
  rpgmutils::signal<void(Enemy*, const std::vector<Action>&)>& actionsModified();
  rpgmutils::signal<void(Enemy*, int)>& battlerHueModified();
  rpgmutils::signal<void(Enemy*, const std::string&)>& battlerNameModified();
  rpgmutils::signal<void(Enemy*, const std::vector<DropItem>&)>& dropItemsModified();
  rpgmutils::signal<void(Enemy*, int)>& expModified();
  rpgmutils::signal<void(Enemy*, const std::vector<Trait>&)>& traitsModified();
  rpgmutils::signal<void(Enemy*, int)>& goldModified();
  rpgmutils::signal<void(Enemy*, const std::string&)>& nameModified();
  rpgmutils::signal<void(Enemy*, const std::string&)>& noteModified();
  rpgmutils::signal<void(Enemy*, const std::array<int, 8>&)>& paramsModified();
  rpgmutils::signal<void(Enemy*, int, int)>& paramModified();

private:
  int m_id;
  std::vector<Action> m_actions;
  int m_battlerHue;
  std::string m_battlerName;
  std::vector<DropItem> m_dropItems;
  int m_exp;
  std::vector<Trait> m_traits;
  int m_gold;
  std::string m_name;
  std::string m_note;
  std::array<int, 8> m_params; // Attack, Defence, M.Attack, M.Defence, Agility, Luck, Max HP, Max MP

  std::optional<int> m_oldid;
  std::optional<std::vector<Action>> m_oldactions;
  std::optional<int> m_oldbattlerHue;
  std::optional<std::string> m_oldbattlerName;
  std::optional<std::vector<DropItem>> m_olddropItems;
  std::optional<int> m_oldexp;
  std::optional<std::vector<Trait>> m_oldtraits;
  std::optional<int> m_oldgold;
  std::optional<std::string> m_oldname;
  std::optional<std::string> m_oldnote;
  std::optional<std::array<int, 8>> m_oldparams; // Attack, Defence, M.Attack, M.Defence, Agility, Luck, Max HP, Max MP

  std::optional<rpgmutils::signal<void(Enemy*, int)>> m_idModified;
  std::optional<rpgmutils::signal<void(Enemy*, const std::vector<Action>&)>> m_actionsModified;
  std::optional<rpgmutils::signal<void(Enemy*, int)>> m_battlerHueModified;
  std::optional<rpgmutils::signal<void(Enemy*, const std::string&)>> m_battlerNameModified;
  std::optional<rpgmutils::signal<void(Enemy*, const std::vector<DropItem>&)>> m_dropItemsModified;
  std::optional<rpgmutils::signal<void(Enemy*, int)>> m_expModified;
  std::optional<rpgmutils::signal<void(Enemy*, const std::vector<Trait>&)>> m_traitsModified;
  std::optional<rpgmutils::signal<void(Enemy*, int)>> m_goldModified;
  std::optional<rpgmutils::signal<void(Enemy*, const std::string&)>> m_nameModified;
  std::optional<rpgmutils::signal<void(Enemy*, const std::string&)>> m_noteModified;
  std::optional<rpgmutils::signal<void(Enemy*, const std::array<int, 8>&)>> m_paramsModified;
  std::optional<rpgmutils::signal<void(Enemy*, int, int)>> m_paramModified;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};
void to_json(nlohmann::ordered_json& to, const Enemy& enemy);
void from_json(const nlohmann::ordered_json& from, Enemy& enemy);
void to_json(nlohmann::ordered_json& to, const Enemy::Action& action);
void from_json(const nlohmann::ordered_json& from, Enemy::Action& action);
void to_json(nlohmann::ordered_json& to, const Enemy::DropItem& dropitem);
void from_json(const nlohmann::ordered_json& from, Enemy::DropItem& dropitem);