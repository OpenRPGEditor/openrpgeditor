#include "Database/Classes.hpp"

#include <fstream>

using json = nlohmann::json;

Classes Classes::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Classes classes;
  classes.m_classes.reserve(data.size());

  int i = 0;
  for (const auto& [_, value] : data.items()) {
    Class& cls = classes.m_classes.emplace_back();
    cls.m_isValid = value != nullptr;
    if (cls.m_isValid) {
      value.get_to(cls);
    } else {
      cls.id = i;
    }
    ++i;
  }
  return classes;
}

void Classes::serialize(std::string_view filename) {
  std::ofstream file(filename.data());
  json data;

  for (const auto& cls : m_classes) {
    if (cls.m_isValid) {
      data.push_back(cls);
    } else {
      data.push_back(nullptr);
    }
  }

  file << data.dump(4);
}