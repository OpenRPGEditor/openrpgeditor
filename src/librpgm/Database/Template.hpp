#pragma once

#include "Database/IModifiable.hpp"

#include <memory>
#include <nlohmann/json.hpp>
#include <string>

class Template final : public IModifiable {
public:
  friend void to_json(nlohmann::ordered_json& j, const Template& templ);
  friend void from_json(const nlohmann::ordered_json& j, Template& templ);

  enum class TemplateType {
    Command,
    Tint,
    Event,
  };

  Template() = default;
  Template(const Template& other);
  Template& operator=(const Template& other);
  Template(Template&& other) noexcept;
  Template& operator=(Template&& other) noexcept;
  Template(const int id, const std::string_view name, const std::string_view note, const TemplateType type, const std::string_view commands, const std::vector<int>& parameters)
  : m_id(id), m_name(name), m_note(note), m_type(type), m_commands(commands), m_parameters(parameters) {}

  [[nodiscard]] int id() const;
  void setId(int id);

  [[nodiscard]] const std::string& name() const;
  void setName(const std::string& name);

  const std::string& note() const;
  void setNote(const std::string& note);

  TemplateType type() const;
  void setType(TemplateType type);

  const std::string& commands() const;
  void setCommands(const std::string& commands);

  [[nodiscard]] std::vector<int>& parameters();
  [[nodiscard]] const std::vector<int>& parameters() const;
  void setParameters(const std::vector<int>& parameters);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  rpgmutils::signal<void(Template*, int)>& idModified();
  rpgmutils::signal<void(Template*, const std::string&)>& nameModified();
  rpgmutils::signal<void(Template*, const std::string&)>& noteModified();
  rpgmutils::signal<void(Template*, TemplateType)>& typeModified();
  rpgmutils::signal<void(Template*, const std::string&)>& commandsModified();
  rpgmutils::signal<void(Template*, const std::vector<int>&)>& parametersModified();

private:
  int m_id = 0;
  std::string m_name;
  std::string m_note;
  TemplateType m_type;
  std::string m_commands;
  std::vector<int> m_parameters;

  std::optional<int> m_oldid;
  std::optional<std::string> m_oldname;
  std::optional<std::string> m_oldnote;
  std::optional<TemplateType> m_oldtype;
  std::optional<std::string> m_oldcommands;
  std::optional<std::vector<int>> m_oldparameters;
  std::optional<rpgmutils::signal<void(Template*, int)>> m_idModified;
  std::optional<rpgmutils::signal<void(Template*, const std::string&)>> m_nameModified;
  std::optional<rpgmutils::signal<void(Template*, const std::string&)>> m_noteModified;
  std::optional<rpgmutils::signal<void(Template*, TemplateType)>> m_typeModified;
  std::optional<rpgmutils::signal<void(Template*, const std::string&)>> m_commandsModified;
  std::optional<rpgmutils::signal<void(Template*, const std::vector<int>&)>> m_parametersModified;
};

void to_json(nlohmann::ordered_json& j, const Template& templ);
void from_json(const nlohmann::ordered_json& j, Template& templ);