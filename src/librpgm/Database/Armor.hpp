#pragma once

#include "Database/Globals.hpp"
#include "Database/Trait.hpp"

class Armor final : public IModifiable {
  friend class ArmorsSerializer;
  friend void to_json(nlohmann::ordered_json& out, const Armor& armor);
  friend void from_json(const nlohmann::ordered_json& from, Armor& armor);

public:
  Armor() = default;
  Armor(const Armor& other);
  Armor& operator=(const Armor& other);
  Armor(Armor&& other) noexcept;
  Armor& operator=(Armor&& other) noexcept;

  [[nodiscard]] int id() const;
  void setId(int id);

  [[nodiscard]] int atypeId() const;
  void setAtypeId(int atypeId);

  [[nodiscard]] const std::string& description() const;
  void setDescription(const std::string& description);

  [[nodiscard]] int etypeId() const;
  void setEtypeId(int etypeId);

  [[nodiscard]] std::vector<Trait>& traits();
  [[nodiscard]] const std::vector<Trait>& traits() const;
  void setTraits(const std::vector<Trait>& traits);
  [[nodiscard]] Trait& trait(int index);
  [[nodiscard]] const Trait& trait(int index) const;
  void setTrait(int index, const Trait& trait);

  [[nodiscard]] int iconIndex() const;
  void setIconIndex(int iconIndex);

  [[nodiscard]] const std::string& name() const;
  void setName(const std::string& name);

  [[nodiscard]] const std::string& note() const;
  void setNote(const std::string& note);

  [[nodiscard]] const std::array<int, 8>& params() const;
  void setParams(const std::array<int, 8>& params);
  [[nodiscard]] int param(int index) const;
  void setParam(int index, int param);

  [[nodiscard]] int price() const;
  void setPrice(int price);

  [[nodiscard]] bool isValid() const;
  void setValid(bool isValid);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  rpgmutils::signal<void(Armor*, int)>& idModified();
  rpgmutils::signal<void(Armor*, int)>& atypeModified();
  rpgmutils::signal<void(Armor*, std::string_view)>& descriptionModified();
  rpgmutils::signal<void(Armor*, int)>& etypeModified();
  rpgmutils::signal<void(Armor*, const std::vector<Trait>&)>& traitsModified();
  rpgmutils::signal<void(Armor*, int, const Trait&)>& traitModified();
  rpgmutils::signal<void(Armor*, int)>& iconIndexModified();
  rpgmutils::signal<void(Armor*, std::string_view)>& nameModified();
  rpgmutils::signal<void(Armor*, std::string_view)>& noteModified();
  rpgmutils::signal<void(Armor*, const std::array<int, 8>&)>& paramsModified();
  rpgmutils::signal<void(Armor*, int, int)>& paramModified();
  rpgmutils::signal<void(Armor*, int)>& priceModified();

private:
  int m_id{};
  int m_atypeId{};
  std::string m_description;
  int m_etypeId{};
  std::vector<Trait> m_traits;
  int m_iconIndex{};
  std::string m_name;
  std::string m_note;
  std::array<int, 8> m_params{}; // Attack, Defence, M.Attack, M.Defence, Agility, Luck, Max HP, Max MP
  int m_price{};

  std::optional<int> m_oldid;
  std::optional<int> m_oldatypeId;
  std::optional<std::string> m_olddescription;
  std::optional<int> m_oldetypeId;
  std::optional<std::vector<Trait>> m_oldtraits;
  std::optional<int> m_oldiconIndex;
  std::optional<std::string> m_oldname;
  std::optional<std::string> m_oldnote;
  std::optional<std::array<int, 8>> m_oldparams; // Attack, Defence, M.Attack, M.Defence, Agility, Luck, Max HP, Max MP
  std::optional<int> m_oldprice;

  std::optional<rpgmutils::signal<void(Armor*, int)>> m_idModified;
  std::optional<rpgmutils::signal<void(Armor*, int)>> m_atypeModified;
  std::optional<rpgmutils::signal<void(Armor*, std::string_view)>> m_descriptionModified;
  std::optional<rpgmutils::signal<void(Armor*, int)>> m_etypeModified;
  std::optional<rpgmutils::signal<void(Armor*, const std::vector<Trait>&)>> m_traitsModified;
  std::optional<rpgmutils::signal<void(Armor*, int, const Trait&)>> m_traitModified;
  std::optional<rpgmutils::signal<void(Armor*, int)>> m_iconIndexModified;
  std::optional<rpgmutils::signal<void(Armor*, std::string_view)>> m_nameModified;
  std::optional<rpgmutils::signal<void(Armor*, std::string_view)>> m_noteModified;
  std::optional<rpgmutils::signal<void(Armor*, const std::array<int, 8>&)>> m_paramsModified;
  std::optional<rpgmutils::signal<void(Armor*, int, int)>> m_paramModified;
  std::optional<rpgmutils::signal<void(Armor*, int)>> m_priceModified;
  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};
void to_json(nlohmann::ordered_json& out, const Armor& armor);
void from_json(const nlohmann::ordered_json& from, Armor& armor);