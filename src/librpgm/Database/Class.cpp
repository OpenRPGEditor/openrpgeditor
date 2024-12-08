#include "Database/Class.hpp"

void to_json(nlohmann::ordered_json& to, const Learning& learning) {
  to = {
      {"level", learning.level},
      {"note", learning.note},
      {"skillId", learning.skillId},
  };
}
void from_json(const nlohmann::ordered_json& from, Learning& learning) {
  learning.level = from.value("level", learning.level);
  learning.note = from.value("note", learning.note);
  learning.skillId = from.value("skillId", learning.skillId);
}

void to_json(nlohmann::ordered_json& to, const Class& cls) {
  to = {
      {"id", cls.m_id}, {"expParams", cls.m_expParams}, {"traits", cls.m_traits}, {"learnings", cls.m_learnings}, {"name", cls.m_name}, {"note", cls.m_note}, {"params", cls.m_params},
  };
}

void from_json(const nlohmann::ordered_json& from, Class& cls) {
  cls.m_id = from.value("id", cls.m_id);
  cls.m_expParams = from.value("expParams", cls.m_expParams);
  cls.m_traits = from.value("traits", cls.m_traits);
  cls.m_learnings = from.value("learnings", cls.m_learnings);
  cls.m_name = from.value("name", cls.m_name);
  cls.m_note = from.value("note", cls.m_note);
  cls.m_params = from.value("params", cls.m_params);
}