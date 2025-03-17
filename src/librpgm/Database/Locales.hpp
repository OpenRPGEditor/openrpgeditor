#pragma once

#include <nlohmann/json.hpp>

class Locales final {
public:
  friend void to_json(nlohmann::ordered_json& j, const std::pair<std::string, std::string>& templ);
  friend void from_json(const nlohmann::ordered_json& j, std::pair<std::string, std::string>& templ);

  std::vector<std::pair<std::string, std::string>> load(std::string_view path);
  void loadMap(std::string_view path);
  bool serialize(std::string_view path);
  void saveCurrentLocale();

  std::vector<std::pair<std::string, std::string>> locales;

  std::string m_currentPath;
};
void to_json(nlohmann::ordered_json& j, const std::pair<std::string, std::string>& templ);
void from_json(const nlohmann::ordered_json& j, std::pair<std::string, std::string>& templ);
