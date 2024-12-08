#pragma once

#include "Database/Globals.hpp"
#include "Database/Trait.hpp"

class Armor {
  friend class ArmorsSerializer;
  friend void to_json(nlohmann::ordered_json& out, const Armor& armor);
  friend void from_json(const nlohmann::ordered_json& from, Armor& armor);

public:
  [[nodiscard]] int id() const { return m_id; }
  void setId(const int id) { m_id = id; }

  [[nodiscard]] int atypeId() const { return m_atypeId; }
  void setAtypeId(const int atypeId) { m_atypeId = atypeId; }

  [[nodiscard]] const std::string& description() const { return m_description; }
  void setDescription(const std::string& description) { m_description = description; }

  [[nodiscard]] int etypeId() const { return m_etypeId; }
  void setEtypeId(const int etypeId) { m_etypeId = etypeId; }

  [[nodiscard]] std::vector<Trait>& traits() { return m_traits; }

  [[nodiscard]] int iconIndex() const { return m_iconIndex; }
  void setIconIndex(const int iconIndex) { m_iconIndex = iconIndex; }

  [[nodiscard]] const std::string& name() const { return m_name; }
  void setName(const std::string& name) { m_name = name; }

  [[nodiscard]] const std::string& note() const { return m_note; }
  void setNote(const std::string& note) { m_note = note; }

  [[nodiscard]] const std::array<int, 8>& params() const { return m_params; }
  void setParams(const std::array<int, 8>& params) { m_params = params; }
  [[nodiscard]] int param(const int idx) const {
    assert(idx >= 0 && idx < m_params.size());
    return m_params[idx];
  }
  void setParam(const int idx, const int param) {
    assert(param >= 0 && param < m_params.size());
    m_params[idx] = param;
  }

  [[nodiscard]] int price() const { return m_price; }
  void setPrice(const int price) { m_price = price; }

  [[nodiscard]] bool isValid() const { return m_isValid; }
  void setValid(const bool isValid) { m_isValid = isValid; }

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