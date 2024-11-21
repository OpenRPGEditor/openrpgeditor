#pragma once

#include <map>
#include <string>
#include <nlohmann/json.hpp>

class Template {
public:
  friend void to_json(nlohmann::ordered_json& j, const Template& templ);
  friend void from_json(const nlohmann::ordered_json& j, Template& templ);

  enum class TemplateType { Command, Tint };
  std::string name;
  TemplateType type;
  std::vector<int> commands;
  std::vector<std::string> parameters;
};

void to_json(nlohmann::ordered_json& j, const Template& templ);
void from_json(const nlohmann::ordered_json& j, Template& templ);

class Templates {
public:
  friend void to_json(nlohmann::ordered_json& j, const Templates& templ);
  friend void from_json(const nlohmann::ordered_json& j, Templates& templ);
  std::vector<Template> templates;

  static Templates load(std::string_view path);
  bool serialize(std::string_view path);
};
void to_json(nlohmann::ordered_json& j, const Templates& templ);
void from_json(const nlohmann::ordered_json& j, Templates& templ);