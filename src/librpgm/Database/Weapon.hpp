#pragma once

#include "Database/Globals.hpp"
#include "Database/Trait.hpp"
#include <nlohmann/json.hpp>

class Weapon final : IModifiable {
  friend class WeaponsSerializer;
  friend void to_json(nlohmann::ordered_json& j, const Weapon& w);
  friend void from_json(const nlohmann::ordered_json& j, Weapon& w);

public:
  Weapon() = default;
  Weapon(const Weapon& other);
  Weapon& operator=(const Weapon& other);
  Weapon(Weapon&& other) noexcept;
  Weapon& operator=(Weapon&& other) noexcept;

  [[nodiscard]] int id() const;
  void setId(int id);

  [[nodiscard]] int animationId() const;
  void setAnimationId(int animationId);

  [[nodiscard]] const std::string& description() const;
  void setDescription(const std::string& description);

  [[nodiscard]] int etypeId() const;
  void setEtypeId(int etypeId);

  [[nodiscard]] std::vector<Trait>& traits();
  void setTraits(const std::vector<Trait>& traits);

  [[nodiscard]] int iconIndex() const;
  void setIconIndex(int iconIndex);

  [[nodiscard]] const std::string& name() const;
  void setName(const std::string& name);

  [[nodiscard]] const std::string& note() const;
  void setNote(const std::string& note);

  std::array<int, 8>& params();
  void setParams(const std::array<int, 8>& params);
  [[nodiscard]] int param(int idx) const;
  void setParam(int idx, int param);
  [[nodiscard]] int price() const;
  void setPrice(int price);

  [[nodiscard]] int wtypeId() const;
  void setWtypeId(int wtypeId);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  rpgmutils::signal<void(Weapon*, int)>& idModified();
  rpgmutils::signal<void(Weapon*, int)>& animationIdModified();
  rpgmutils::signal<void(Weapon*, const std::string&)>& descriptionModified();
  rpgmutils::signal<void(Weapon*, int)>& etypeIdModified();
  rpgmutils::signal<void(Weapon*, const std::vector<Trait>&)>& traitsModified();
  rpgmutils::signal<void(Weapon*, int)>& iconIndexModified();
  rpgmutils::signal<void(Weapon*, const std::string&)>& nameModified();
  rpgmutils::signal<void(Weapon*, const std::string&)>& noteModified();
  rpgmutils::signal<void(Weapon*, const std::array<int, 8>&)>& paramsModified();
  rpgmutils::signal<void(Weapon*, int)>& priceModified();
  rpgmutils::signal<void(Weapon*, int)>& wtypeIdModified();

  [[nodiscard]] bool isValid() const { return m_isValid; }
  void setValid(const bool isValid) { m_isValid = isValid; }

  bool isModified() const override {
    return IModifiable::isModified() | std::ranges::any_of(m_traits, [](const Trait& trait) { return trait.isModified(); });
  }

private:
  int m_id;
  int m_animationId;
  std::string m_description;
  int m_etypeId;
  std::vector<Trait> m_traits;
  int m_iconIndex;
  std::string m_name;
  std::string m_note;
  std::array<int, 8> m_params; // Attack, Defence, M.Attack, M.Defence, Agility, Luck, Max HP, Max MP
  int m_price;
  int m_wtypeId;

  std::optional<int> m_oldid;
  std::optional<int> m_oldanimationId;
  std::optional<std::string> m_olddescription;
  std::optional<int> m_oldetypeId;
  std::optional<std::vector<Trait>> m_oldtraits;
  std::optional<int> m_oldiconIndex;
  std::optional<std::string> m_oldname;
  std::optional<std::string> m_oldnote;
  std::optional<std::array<int, 8>> m_oldparams;
  std::optional<int> m_oldprice;
  std::optional<int> m_oldwtypeId;

  std::optional<rpgmutils::signal<void(Weapon*, int)>> m_idModified;
  std::optional<rpgmutils::signal<void(Weapon*, int)>> m_animationIdModified;
  std::optional<rpgmutils::signal<void(Weapon*, const std::string&)>> m_descriptionModified;
  std::optional<rpgmutils::signal<void(Weapon*, int)>> m_etypeIdModified;
  std::optional<rpgmutils::signal<void(Weapon*, const std::vector<Trait>&)>> m_traitsModified;
  std::optional<rpgmutils::signal<void(Weapon*, int)>> m_iconIndexModified;
  std::optional<rpgmutils::signal<void(Weapon*, const std::string&)>> m_nameModified;
  std::optional<rpgmutils::signal<void(Weapon*, const std::string&)>> m_noteModified;
  std::optional<rpgmutils::signal<void(Weapon*, const std::array<int, 8>&)>> m_paramsModified;
  std::optional<rpgmutils::signal<void(Weapon*, int)>> m_priceModified;
  std::optional<rpgmutils::signal<void(Weapon*, int)>> m_wtypeIdModified;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};

void to_json(nlohmann::ordered_json& j, const Weapon& w);
void from_json(const nlohmann::ordered_json& j, Weapon& w);