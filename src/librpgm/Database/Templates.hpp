#pragma once

#include <map>
#include <string>
#include <nlohmann/json.hpp>

class Template {
public:

  enum class TemplateType {
    Command,
    Tint
  };
  std::string name;
  int type;
  std::vector<int> commands;
  std::vector<std::string> parameters;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Template, name, type, commands, parameters);
};
class Templates {
public:
  std::vector<Template> templates;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Templates, templates);

  static Templates load(std::string_view path);
  bool serialize(std::string_view path);
};