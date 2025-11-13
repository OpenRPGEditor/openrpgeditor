#include "Locales.hpp"
#include "Database/Database.hpp"

#include <format>
#include <fstream>

void to_json(nlohmann::ordered_json& j, const std::pair<std::string, std::string>& templ) {
  j = {
      {{templ.first, templ.second}},
  };
}

void from_json(const nlohmann::ordered_json& j, std::unordered_map<std::string, std::string>& templ) {
  for (const auto& element : j) {
    // Each element is a JSON object with one key-value pair
    for (auto& el : element.items()) {
      // templ.insert(std::make_pair(el.key(), el.value()));
      //  templ.emplace_back(el.key(), el.value().dump());
    }
  }
}
std::vector<std::pair<std::string, std::string>> Locales::load(const std::filesystem::path& path) {
  if (std::ifstream file(path); file.is_open()) {
    try {
      nlohmann::ordered_json data = nlohmann::ordered_json::parse(file);
      std::vector<std::pair<std::string, std::string>> ret;
      // data.get_to(ret);
      // return ret;

      // Iterate through the JSON object and insert key-value pairs into the vector
      for (auto& el : data.items()) {
        ret.emplace_back(el.key(), el.value().get<std::string>());
      }
      return ret;
    } catch (...) {}
  }

  return {};
}
void Locales::loadMap(const std::filesystem::path& path) {
  m_currentPath = path;
  locales = load(path);
}
bool Locales::serialize(const std::filesystem::path& path) const {
  std::ofstream file(path);
  try {
    if (file.is_open()) {
      nlohmann::ordered_json data;

      for (const auto& pair : locales) {
        data[pair.first] = pair.second;
      }

      file << data.dump(4);
      return true;
    }
  } catch (...) {}
  return false;
}
void Locales::saveCurrentLocale() { serialize(m_currentPath); }