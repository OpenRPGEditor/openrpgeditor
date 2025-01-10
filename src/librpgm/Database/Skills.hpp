#pragma once

#include "Database/Skill.hpp"

class Skills : public IModifiable {
public:
  [[nodiscard]] Skill* skill(const int id) {
    for (auto& skill : m_skills) {
      if (skill.id() == id && skill.isValid()) {
        return &skill;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Skill* skill(const int id) const {
    for (const auto& skill : m_skills) {
      if (skill.id() == id && skill.isValid()) {
        return &skill;
      }
    }

    return nullptr;
  }

  std::vector<Skill>& skills() { return m_skills; }
  const std::vector<Skill>& skills() const { return m_skills; }

  int count() const { return m_skills.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    const int oldSize = m_skills.size();
    m_skills.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_skills.size(); ++i) {
        m_skills[i].setId(i);
      }
    }
  }

  bool isModified() const override {
    return IModifiable::isModified() | std::ranges::any_of(m_skills, [](const Skill& skill) { return skill.isModified(); });
  }

private:
  std::vector<Skill> m_skills;
};

void to_json(nlohmann::ordered_json& j, const Skill& skill);
void from_json(const nlohmann::ordered_json& j, Skill& skill);