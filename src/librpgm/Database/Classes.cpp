#include "Database/Classes.hpp"

#include <fstream>

using json = nlohmann::json;

Classes Classes::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Classes classes;
  classes.m_classes.reserve(data.size());

  for (const auto& [_, value] : data.items()) {
    if (value == nullptr) {
      continue;
    }

    Class& cls = classes.m_classes.emplace_back();
    value.get_to(cls);
  }
  return classes;
}

void Classes::serialize(std::string_view filename) {
  std::ofstream file(filename.data());
  json data{nullptr};
  std::setprecision(2);

  for (const Class& cls : m_classes) {
    data.push_back(cls);
  }

  file << data.dump(4);
}