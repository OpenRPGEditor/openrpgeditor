#pragma once

#include "Database/Template.hpp"

class Templates final : public IModifiable {
  mutable std::unordered_map<Template::TemplateType, std::vector<Template>> m_filteredCache;

public:
  friend void to_json(nlohmann::ordered_json& j, const Templates& templ);
  friend void from_json(const nlohmann::ordered_json& j, Templates& templ);
  std::vector<Template> templates;

  static Templates load(const std::filesystem::path& path);
  bool serialize(const std::filesystem::path& path);

  bool isModified() const override {
    return IModifiable::isModified() | std::ranges::any_of(templates, [](const Template& t) { return t.isModified(); });
  }
  void addTemplate(const Template& tmpl) {
    templates.push_back(tmpl);
    m_filteredCache.clear();
  }
  std::vector<Template>& templateList(Template::TemplateType type) {
    auto it = m_filteredCache.find(type);
    if (it == m_filteredCache.end()) {
      // Filter and store the result in the cache
      std::vector<Template> filteredTemplates;
      for (const auto& tmpl : templates) {
        if (tmpl.type() == type) {
          filteredTemplates.push_back(tmpl);
        }
      }
      m_filteredCache[type] = std::move(filteredTemplates);
    }
    return m_filteredCache[type];
  }
};
void to_json(nlohmann::ordered_json& j, const Templates& templ);
void from_json(const nlohmann::ordered_json& j, Templates& templ);
