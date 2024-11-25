#pragma once

#include <Database/EventCommands/IEventCommand.hpp>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

class Template {
public:
  friend void to_json(nlohmann::ordered_json& j, const Template& templ);
  friend void from_json(const nlohmann::ordered_json& j, Template& templ);

  enum class TemplateType { Command, Tint, Event };
  int id = 0;
  std::string name;
  TemplateType type;
  std::string commands;
  std::vector<int> parameters;
};

void to_json(nlohmann::ordered_json& j, const Template& templ);
void from_json(const nlohmann::ordered_json& j, Template& templ);

class Templates {
private:
  mutable std::unordered_map<Template::TemplateType, std::vector<Template>> filteredCache;

public:
  friend void to_json(nlohmann::ordered_json& j, const Templates& templ);
  friend void from_json(const nlohmann::ordered_json& j, Templates& templ);
  std::vector<Template> templates;

  static Templates load(std::string_view path);
  bool serialize(std::string_view path);

  void addTemplate(const Template& tmpl) {
    templates.push_back(tmpl);
    filteredCache.clear();
  }
  std::vector<Template>& templateList(Template::TemplateType type) {
    auto it = filteredCache.find(type);
    if (it == filteredCache.end()) {
      // Filter and store the result in the cache
      std::vector<Template> filteredTemplates;
      for (const auto& tmpl : templates) {
        if (tmpl.type == type) {
          filteredTemplates.push_back(tmpl);
        }
      }
      filteredCache[type] = std::move(filteredTemplates);
    }
    return filteredCache[type];
  }
};
void to_json(nlohmann::ordered_json& j, const Templates& templ);
void from_json(const nlohmann::ordered_json& j, Templates& templ);
