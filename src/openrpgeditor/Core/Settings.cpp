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
      parser.at("uiScale").get_to(uiScale);
      parser.at("fontSize").get_to(fontSize);
      parser.at("monoFontSize").get_to(fontSize);
      parser.at("dpi").get_to(dpi);
      return true;
    }
  } catch (...) {
    // scream into the void, we don't care
  }
  return false;
}

void Settings::serialize(std::string_view path) {
  std::ofstream f(path.data());
  if (f.is_open()) {
    nlohmann::json parser{
        {"window", window},               //
        {"mru", mru},                     //
        {"lastDirectory", lastDirectory}, //
        {"uiScale", uiScale},             //
        {"fontSize", fontSize},           //
      {"monoFontSize", monoFontSize},     //
        {"dpi", dpi}                      //
    };
    f << parser.dump(4);
  }
}