#include "Core/Settings.hpp"
#include <fstream>

Settings* Settings::m_instance = nullptr;

Settings::Settings() { m_instance = this; }
bool Settings::load(std::string_view path) {
  try {
    if (std::ifstream f(path.data()); f.is_open()) {
      nlohmann::json parser = nlohmann::json::parse(f);
      parser.at("window").get_to(window);
      parser.at("mru").get_to(mru);
      parser.at("lastDirectory").get_to(lastDirectory);
      return true;
    }
  } catch(...) {
    // scream into the void, we don't care
  }
  return false;
}

void Settings::serialize(std::string_view path) {
  std::ofstream f(path.data());
  if (f.is_open()) {
    nlohmann::json parser{
        {"window", window},
        {"mru", mru},
        {"lastDirectory", lastDirectory},
    };
    f << parser.dump(4);
  }
}