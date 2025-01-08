#pragma once
#include "Database/Globals.hpp"
#include "IModifiable.hpp"

#include <nlohmann/json.hpp>
#include <string>

class Damage final : public IModifiable {
  friend void to_json(nlohmann::ordered_json& j, const Damage& damage);
  friend void from_json(const nlohmann::ordered_json& j, Damage& damage);

public:
  Damage() = default;
  Damage(const Damage& other);
  Damage& operator=(const Damage& other);
  Damage(Damage&& other) noexcept;
  Damage& operator=(Damage&& other) noexcept;

  bool critical() const;
  void setCritical(bool critical);

  int elementId() const;
  void setElementId(int elementId);

  const std::string& formula() const;
  void setFormula(const std::string& formula);

  DamageType type() const;
  void setType(DamageType type);

  int variance() const;
  void setVariance(int variance);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  rpgmutils::signal<void(Damage*, bool)>& criticalModified();
  rpgmutils::signal<void(Damage*, int)>& elementIdModified();
  rpgmutils::signal<void(Damage*, const std::string&)>& formulaModified();
  rpgmutils::signal<void(Damage*, DamageType)>& typeModified();
  rpgmutils::signal<void(Damage*, int)>& varianceModified();

  bool operator==(const Damage& other) const {
    return m_critical == other.m_critical && m_elementId == other.m_elementId && m_formula == other.m_formula && m_type == other.m_type && m_variance == other.m_variance;
  }

private:
  bool m_critical;
  int m_elementId;
  std::string m_formula;
  DamageType m_type;
  int m_variance;

  std::optional<bool> m_oldcritical;
  std::optional<int> m_oldelementId;
  std::optional<std::string> m_oldformula;
  std::optional<DamageType> m_oldtype;
  std::optional<int> m_oldvariance;

  std::optional<rpgmutils::signal<void(Damage*, bool)>> m_criticalModified;
  std::optional<rpgmutils::signal<void(Damage*, int)>> m_elementIdModified;
  std::optional<rpgmutils::signal<void(Damage*, const std::string&)>> m_formulaModified;
  std::optional<rpgmutils::signal<void(Damage*, DamageType)>> m_typeModified;
  std::optional<rpgmutils::signal<void(Damage*, int)>> m_varianceModified;
};

void to_json(nlohmann::ordered_json& j, const Damage& damage);
void from_json(const nlohmann::ordered_json& j, Damage& damage);