#include "Skills.hpp"

#include <fstream>

using json = nlohmann::json;

void to_json(nlohmann::json& j, const Skill& skill) {
  j = {
      {"id", skill.id},
      {"animationId", skill.animationId},
      {"damage", skill.damage},
      {"description", skill.description},
      {"effects", skill.effects},
      {"hitType", skill.hitType},
      {"iconIndex", skill.iconIndex},
      {"message1", skill.message1},
      {"message2", skill.message2},
      {"mpCost", skill.mpCost},
      {"name", skill.name},
      {"note", skill.note},
      {"occasion", skill.occasion},
      {"repeats", skill.repeats},
      {"requiredWtypeId1", skill.requiredWtypeId1},
      {"requiredWtypeId2", skill.requiredWtypeId2},
      {"scope", skill.scope},
      {"speed", skill.speed},
      {"stypeId", skill.stypeId},
      {"successRate", skill.successRate},
      {"tpCost", skill.tpCost},
      {"tpGain", skill.tpGain},
  };
}

void from_json(const nlohmann::json& j, Skill& skill) {
  skill.id = j.value("id", skill.id);
  skill.animationId = j.value("animationId", skill.animationId);
  skill.damage = j.value("damage", skill.damage);
  skill.description = j.value("description", skill.description);
  skill.effects = j.value("effects", skill.effects);
  skill.hitType = j.value("hitType", skill.hitType);
  skill.iconIndex = j.value("iconIndex", skill.iconIndex);
  skill.message1 = j.value("message1", skill.message1);
  skill.message2 = j.value("message2", skill.message2);
  skill.mpCost = j.value("mpCost", skill.mpCost);
  skill.name = j.value("name", skill.name);
  skill.note = j.value("note", skill.note);
  skill.occasion = j.value("occasion", skill.occasion);
  skill.repeats = j.value("repeats", skill.repeats);
  skill.scope = j.value("scope", skill.scope);
  skill.speed = j.value("speed", skill.speed);
  skill.stypeId = j.value("stypeId", skill.stypeId);
  skill.successRate = j.value("successRate", skill.successRate);
  skill.tpCost = j.value("tpCost", skill.tpCost);
  skill.tpGain = j.value("tpGain", skill.tpGain);
}

Skills Skills::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Skills skills;
  skills.m_skills.reserve(data.size());

  int i = 0;
  for (const auto& [_, value] : data.items()) {
    Skill& skill = skills.m_skills.emplace_back();
    skill.m_isValid = value != nullptr;
    if (skill.m_isValid) {
      value.get_to(skill);
    } else {
      skill.id = i;
    }
    ++i;
  }
  return skills;
}

bool Skills::serialize(std::string_view filename) {
  // TODO: Temporary file for atomic writes

  std::ofstream file(filename.data());
  json data{nullptr};

  for (const Skill& skill : m_skills) {
    if (skill.m_isValid) {
      data.push_back(skill);
    } else {
      data.push_back(nullptr);
    }
  }

  file << data.dump(4);

  return true;
}
