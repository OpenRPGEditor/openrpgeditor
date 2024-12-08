#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <string>

class Template {
public:
  friend void to_json(nlohmann::ordered_json& j, const Template& templ);
  friend void from_json(const nlohmann::ordered_json& j, Template& templ);

  Template() = default;
  enum class TemplateType { Command, Tint, Event };

  Template(const int id, const std::string_view name, const std::string_view note, const TemplateType type, const std::string_view commands, const std::vector<int>& parameters)
  : m_id(id), m_name(name), m_note(note), m_type(type), m_commands(commands), m_parameters(parameters) {}

  [[nodiscard]] int id() const { return m_id; }
  void setId(const int id) { m_id = id; }

  [[nodiscard]] const std::string& name() const { return m_name; }
  void setName(const std::string& name) { m_name = name; }

  const std::string& note() const { return m_note; }
  void setNote(const std::string& note) { m_note = note; }

  TemplateType type() const { return m_type; }
  void setType(const TemplateType type) { m_type = type; }

  const std::string& commands() const { return m_commands; }
  void setCommands(const std::string& commands) { m_commands = commands; }

  [[nodiscard]] std::vector<int>& parameters() { return m_parameters; }
  [[nodiscard]] const std::vector<int>& parameters() const { return m_parameters; }
  void setParameters(const std::vector<int>& parameters) { m_parameters = parameters; }

private:
  int m_id = 0;
  std::string m_name;
  std::string m_note;
  TemplateType m_type;
  std::string m_commands;
  std::vector<int> m_parameters;
};

void to_json(nlohmann::ordered_json& j, const Template& templ);
void from_json(const nlohmann::ordered_json& j, Template& templ);