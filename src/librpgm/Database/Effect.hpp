#pragma once
#include "Database/Globals.hpp"
#include "Database/IModifiable.hpp"

#include <nlohmann/json.hpp>

class Effect final : public IModifiable {
  friend void to_json(nlohmann::ordered_json& to, const Effect& effect);
  friend void from_json(const nlohmann::ordered_json& from, Effect& effect);

public:
  Effect() = default;
  Effect(const Effect& other);
  Effect& operator=(const Effect& other);
  Effect(Effect&& other) noexcept;
  Effect& operator=(Effect&& other) noexcept;

  EffectCode code() const;
  void setCode(EffectCode code);

  int dataId() const; //! Object ID depending on the selection (Add State, Remove State, Common Event, Learn Skill)
  void setDataId(int dataId);

  float value1() const; //! Combobox member value OR dataId's value -- percent values are used as (1.00 ~ -1.00)
  void setValue1(float value1);

  int value2() const; //! Never used for object id; secondary value after combobox
  void setValue2(int value2);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  rpgmutils::signal<void(Effect*, EffectCode)>& codeModified();
  rpgmutils::signal<void(Effect*, int)>& dataIdModified();
  rpgmutils::signal<void(Effect*, float)>& value1Modified();
  rpgmutils::signal<void(Effect*, int)>& value2Modified();

  bool operator==(const Effect& other) const { return m_code == other.m_code && m_dataId == other.m_dataId && m_value1 == other.m_value1 && m_value2 == other.m_value2; }

private:
  EffectCode m_code = EffectCode::Recover_HP;
  int m_dataId{0};
  float m_value1{1.f};
  int m_value2{0};

  std::optional<EffectCode> m_oldcode;
  std::optional<int> m_olddataId;
  std::optional<float> m_oldvalue1;
  std::optional<int> m_oldvalue2;

  std::optional<rpgmutils::signal<void(Effect*, EffectCode)>> m_codeModified;
  std::optional<rpgmutils::signal<void(Effect*, int)>> m_dataIdModified;
  std::optional<rpgmutils::signal<void(Effect*, float)>> m_value1Modified;
  std::optional<rpgmutils::signal<void(Effect*, int)>> m_value2Modified;
};

void to_json(nlohmann::ordered_json& to, const Effect& effect);
void from_json(const nlohmann::ordered_json& from, Effect& effect);