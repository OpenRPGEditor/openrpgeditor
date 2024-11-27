#include "Database/Templates.hpp"

#include "Database/Database.hpp"

#include <format>
#include <fstream>

void to_json(nlohmann::ordered_json& j, const Template& templ) {
  j = {
      {"id", templ.id},
      {"name", templ.name},
      {"note", templ.note},
      {"type", templ.type},
      {"commands", templ.commands},
      {"parameters", templ.parameters},
  };
}

void from_json(const nlohmann::ordered_json& j, Template& templ) {
  templ.id = j.value("id", templ.id);
  templ.name = j.value("name", templ.name);
  templ.note = j.value("note", templ.note);
  templ.type = j.value("type", templ.type);
  templ.commands = j.value("commands", templ.commands);
  templ.parameters = j.value("parameters", templ.parameters);
}

void to_json(nlohmann::ordered_json& j, const Templates& templ) {
  j = {
      {"templates", templ.templates},
  };
}

void from_json(const nlohmann::ordered_json& j, Templates& templ) { templ.templates = j.value("templates", templ.templates); }

Templates Templates::load(std::string_view path) {
  if (std::ifstream file(path.data()); file.is_open()) {
    try {
      nlohmann::ordered_json data = nlohmann::ordered_json::parse(file);
      Templates ret;
      data.get_to(ret);
      return ret;
    } catch (...) {}
  }

  return {};
}

bool Templates::serialize(const std::string_view path) {
  std::ofstream file(path.data());
  try {
    if (file.is_open()) {
      const nlohmann::ordered_json data = *this;
      file << data.dump(4);
      return true;
    }
  } catch (...) {}
  return false;
}