#pragma once

#include "Database/IModifiable.hpp"

#include <nlohmann/json.hpp>
#include <string>

class Docs final : public IModifiable {
public:
  friend void to_json(nlohmann::ordered_json& j, const Docs& templ);
  friend void from_json(const nlohmann::ordered_json& j, Docs& templ);

  Docs() = default;
  Docs(const Docs& other);
  Docs& operator=(const Docs& other);
  Docs(Docs&& other) noexcept;
  Docs& operator=(Docs&& other) noexcept;
  Docs(const int id, std::string str) : m_id(id), m_string(str) {}

  [[nodiscard]] int id() const;
  void setId(int id);

  [[nodiscard]] const std::string& str() const;
  void setString(const std::string& str);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  rpgmutils::signal<void(Docs*, int)>& idModified();
  rpgmutils::signal<void(Docs*, const std::string&)>& stringModified();

  static Docs load(std::string_view path);
  bool serialize(std::string_view path);

private:
  int m_id = 0;
  std::string m_string;

  std::optional<int> m_oldid;
  std::optional<std::string> m_oldstring;
  std::optional<rpgmutils::signal<void(Docs*, int)>> m_idModified;
  std::optional<rpgmutils::signal<void(Docs*, const std::string&)>> m_stringModified;
};

void to_json(nlohmann::ordered_json& j, const Docs& templ);
void from_json(const nlohmann::ordered_json& j, Docs& templ);