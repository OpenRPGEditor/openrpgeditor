#pragma once

#include "Database/Trait.hpp"

#include <nlohmann/json.hpp>

#include <vector>
#include <string>
#include <array>

struct Learning {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Learning, level, note, skillId);

  int level{};
  std::string note;
  int skillId{};
};

struct Class {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Class, id, expParams, traits, learnings, name, note, params);
  int id{};
  std::array<int, 4> expParams{}; // Base, Extra, Acceleration A, Acceleration B
  std::vector<Trait> traits;
  std::vector<Learning> learnings;
  std::string name;
  std::string note;
  std::array<std::vector<int>, 8> params;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};

class Classes {
public:
  static Classes load(std::string_view filename);
  void serialize(std::string_view filename);

  [[nodiscard]] Class* classType(int id) {
    for (auto& cls : m_classes) {
      if (cls.id == id && cls.m_isValid) {
        return &cls;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Class* classType(int id) const {
    for (const auto& cls : m_classes) {
      if (cls.id == id && cls.m_isValid) {
        return &cls;
      }
    }

    return nullptr;
  }

  const std::vector<Class>& classes() const { return m_classes; }
  std::vector<Class>& classes() { return m_classes; }

  int count() { return m_classes.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    int oldSize = m_classes.size();
    m_classes.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_classes.size(); ++i) {
        m_classes[i].id = i;
      }
    }
  }

private:
  std::vector<Class> m_classes;
};
