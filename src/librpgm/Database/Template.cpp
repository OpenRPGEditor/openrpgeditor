#include "Database/Template.hpp"

void to_json(nlohmann::ordered_json& j, const Template& templ) {
  j = {
      {"id", templ.m_id}, {"name", templ.m_name}, {"note", templ.m_note}, {"type", templ.m_type}, {"commands", templ.m_commands}, {"parameters", templ.m_parameters},
  };
}

void from_json(const nlohmann::ordered_json& j, Template& templ) {
  templ.m_id = j.value("id", templ.m_id);
  templ.m_name = j.value("name", templ.m_name);
  templ.m_note = j.value("note", templ.m_note);
  templ.m_type = j.value("type", templ.m_type);
  templ.m_commands = j.value("commands", templ.m_commands);
  templ.m_parameters = j.value("parameters", templ.m_parameters);
}