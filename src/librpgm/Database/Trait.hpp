#pragma once

#include "Database/Globals.hpp"
#include "IModifiable.hpp"

#include "nlohmann/json.hpp"

class Trait final : public IModifiable {
  friend void to_json(nlohmann::ordered_json& j, const Trait& trait);
  friend void from_json(const nlohmann::ordered_json& j, Trait& trait);

public:
  Trait() = default;
  Trait(const Trait& trait);
  Trait& operator=(const Trait& trait);
  Trait(Trait&& trait) noexcept;
  Trait& operator=(Trait&& trait) noexcept;

  TraitCode code() const;
  void setCode(TraitCode code);
  [[nodiscard]] int dataId() const;
  void setDataId(int dataId);
  [[nodiscard]] double value() const;
  void setValue(double value);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  // Signals
  rpgmutils::signal<void(Trait*, TraitCode)>& codeModified();
  rpgmutils::signal<void(Trait*, int)>& dataIdModified();
  rpgmutils::signal<void(Trait*, double)>& valueModified();

private:
  TraitCode m_code = TraitCode::Element_Rate;
  int m_dataId = 1;
  double m_value = 1.f;

  // !!!DO NOT CHANGE CASING!!!
  std::optional<TraitCode> m_oldcode = TraitCode::Element_Rate;
  std::optional<int> m_olddataId;
  std::optional<double> m_oldvalue;

  std::optional<rpgmutils::signal<void(Trait*, TraitCode)>> m_codeModified;
  std::optional<rpgmutils::signal<void(Trait*, int)>> m_dataIdModified;
  std::optional<rpgmutils::signal<void(Trait*, double)>> m_valueModified;
};
void to_json(nlohmann::ordered_json& j, const Trait& trait);
void from_json(const nlohmann::ordered_json& j, Trait& trait);