#include "Database/Classes.hpp"

#include <fstream>

using json = nlohmann::ordered_json;

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
      {"id", cls.id},     {"expParams", cls.expParams}, {"traits", cls.traits}, {"learnings", cls.learnings},
      {"name", cls.name}, {"note", cls.note},           {"params", cls.params},
  };
}

void from_json(const nlohmann::ordered_json& from, Class& cls) {
  cls.id = from.value("id", cls.id);
  cls.expParams = from.value("expParams", cls.expParams);
  cls.traits = from.value("traits", cls.traits);
  cls.learnings = from.value("learnings", cls.learnings);
  cls.name = from.value("name", cls.name);
  cls.note = from.value("note", cls.note);
  cls.params = from.value("params", cls.params);
}

Classes Classes::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Classes classes;
  classes.m_classes.reserve(data.size());

  int i = 0;
  for (const auto& [_, value] : data.items()) {
    Class& cls = classes.m_classes.emplace_back();
    cls.m_isValid = value != nullptr;
    if (cls.m_isValid) {
      value.get_to(cls);
    } else {
      cls.id = i;
    }
    ++i;
  }
  return classes;
}

void Classes::serialize(std::string_view filename) {
  std::ofstream file(filename.data());
  json data;

  for (const auto& cls : m_classes) {
    if (cls.m_isValid) {
      data.push_back(cls);
    } else {
      data.push_back(nullptr);
    }
  }

  file << data.dump(4);
}