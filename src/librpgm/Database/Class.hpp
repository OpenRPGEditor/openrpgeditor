#pragma once

#include "Database/Trait.hpp"
#include <nlohmann/json.hpp>
#include <string>

struct Learning {
  int level{};
  std::string note;
  int skillId{};
};
void to_json(nlohmann::ordered_json& to, const Learning& learning);
void from_json(const nlohmann::ordered_json& from, Learning& learning);

class Class {
  friend class ClassesSerializer;
  friend void to_json(nlohmann::ordered_json& to, const Class& cls);
  friend void from_json(const nlohmann::ordered_json& from, Class& cls);

public:
  [[nodiscard]] int id() const { return m_id; }
  void setId(const int id) { m_id = id; }

  [[nodiscard]] const std::array<int, 4>& expParams() const { return m_expParams; }
  void setExpParams(const std::array<int, 4>& expParams) { m_expParams = expParams; }
  void setExpParam(const int index, const int value) {
    assert(index >= 0 && index < 4);
    m_expParams[index] = value;
  }

  [[nodiscard]] std::vector<Trait>& traits() { return m_traits; }

  [[nodiscard]] std::vector<Learning>& learnings() { return m_learnings; }

  [[nodiscard]] const std::string& name() const { return m_name; }
  void setName(const std::string& name) { m_name = name; }

  [[nodiscard]] const std::string& note() const { return m_note; }
  void setNote(const std::string& note) { m_note = note; }

  [[nodiscard]] const std::array<std::array<int, 99>, 8>& params() const { return m_params; }
  void setParams(const std::array<std::array<int, 99>, 8>& params) { m_params = params; }

  [[nodiscard]] bool isValid() const { return m_isValid; }
  void setValid(const bool isValid) { m_isValid = isValid; }

private:
  int m_id{};
  std::array<int, 4> m_expParams{}; // Base, Extra, Acceleration A, Acceleration B
  std::vector<Trait> m_traits;
  std::vector<Learning> m_learnings;
  std::string m_name;
  std::string m_note;
  std::array<std::array<int, 99>, 8> m_params;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};
void to_json(nlohmann::ordered_json& to, const Class& cls);
void from_json(const nlohmann::ordered_json& from, Class& cls);