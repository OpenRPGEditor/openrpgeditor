#include "Skill.hpp"

void to_json(nlohmann::ordered_json& j, const Skill& skill) {
  j = {
      {"id", skill.m_id},
      {"animationId", skill.m_animationId},
      {"damage", skill.m_damage},
      {"description", skill.m_description},
      {"effects", skill.m_effects},
      {"hitType", skill.m_hitType},
      {"iconIndex", skill.m_iconIndex},
      {"message1", skill.m_message1},
      {"message2", skill.m_message2},
      {"mpCost", skill.m_mpCost},
      {"name", skill.m_name},
      {"note", skill.m_note},
      {"occasion", skill.m_occasion},
      {"repeats", skill.m_repeats},
      {"requiredWtypeId1", skill.m_requiredWtypeId1},
      {"requiredWtypeId2", skill.m_requiredWtypeId2},
      {"scope", skill.m_scope},
      {"speed", skill.m_speed},
      {"stypeId", skill.m_stypeId},
      {"successRate", skill.m_successRate},
      {"tpCost", skill.m_tpCost},
      {"tpGain", skill.m_tpGain},
  };
}

void from_json(const nlohmann::ordered_json& j, Skill& skill) {
  skill.m_id = j.value("id", skill.m_id);
  skill.m_animationId = j.value("animationId", skill.m_animationId);
  skill.m_damage = j.value("damage", skill.m_damage);
  skill.m_description = j.value("description", skill.m_description);
  skill.m_effects = j.value("effects", skill.m_effects);
  skill.m_hitType = j.value("hitType", skill.m_hitType);
  skill.m_iconIndex = j.value("iconIndex", skill.m_iconIndex);
  skill.m_message1 = j.value("message1", skill.m_message1);
  skill.m_message2 = j.value("message2", skill.m_message2);
  skill.m_mpCost = j.value("mpCost", skill.m_mpCost);
  skill.m_name = j.value("name", skill.m_name);
  skill.m_note = j.value("note", skill.m_note);
  skill.m_occasion = j.value("occasion", skill.m_occasion);
  skill.m_repeats = j.value("repeats", skill.m_repeats);
  skill.m_scope = j.value("scope", skill.m_scope);
  skill.m_speed = j.value("speed", skill.m_speed);
  skill.m_stypeId = j.value("stypeId", skill.m_stypeId);
  skill.m_successRate = j.value("successRate", skill.m_successRate);
  skill.m_tpCost = j.value("tpCost", skill.m_tpCost);
  skill.m_tpGain = j.value("tpGain", skill.m_tpGain);
}