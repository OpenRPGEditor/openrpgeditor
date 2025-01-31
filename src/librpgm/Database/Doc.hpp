#pragma once

#include "Database/IModifiable.hpp"

#include <nlohmann/json.hpp>
#include <string>

class Doc final : public IModifiable {
public:
  friend void to_json(nlohmann::ordered_json& j, const Doc& templ);
  friend void from_json(const nlohmann::ordered_json& j, Doc& templ);

  Doc() = default;
  Doc(const Doc& other);
  Doc& operator=(const Doc& other);
  Doc(Doc&& other) noexcept;
  Doc& operator=(Doc&& other) noexcept;
  Doc(std::vector<std::string> texts, std::vector<std::string> names) : m_texts(texts), m_names(names) {}

  [[nodiscard]] std::vector<std::string> texts() const;
  void setTexts(std::vector<std::string> text);

  [[nodiscard]] const std::vector<std::string>& names() const;
  void setNames(const std::vector<std::string>& str);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  rpgmutils::signal<void(Doc*, std::vector<std::string>)>& textsModified();
  rpgmutils::signal<void(Doc*, const std::vector<std::string>&)>& namesModified();

  static Doc load(std::string_view path);
  bool serialize(std::string_view path);

private:
  std::vector<std::string> m_names; // Names of categories
  std::vector<std::string> m_texts;         // All text for a given category

  std::optional<std::vector<std::string>> m_oldnames;
  std::optional<std::vector<std::string>> m_oldtexts;

  std::optional<rpgmutils::signal<void(Doc*, std::vector<std::string>)>> m_textsModified;
  std::optional<rpgmutils::signal<void(Doc*, const std::vector<std::string>&)>> m_namesModified;
};

void to_json(nlohmann::ordered_json& j, const Doc& templ);
void from_json(const nlohmann::ordered_json& j, Doc& templ);