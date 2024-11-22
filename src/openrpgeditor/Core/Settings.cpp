#include "Core/Settings.hpp"
#include <fstream>

Settings* Settings::m_instance = nullptr;

Settings::Settings() { m_instance = this; }
bool Settings::load(std::string_view path) {
  try {
    if (std::ifstream f(path.data()); f.is_open()) {
      nlohmann::json parser = nlohmann::json::parse(f);
      window = parser.value("window", window);
      mru = parser.value("mru", mru);
      lastDirectory = parser.value("lastDirectory", lastDirectory);
      uiScale = parser.value("uiScale", uiScale);
      fontSize = parser.value("fontSize", fontSize);
      monoFontSize = parser.value("monoFontSize", monoFontSize);
      projectBaseDirectory = parser.value("projectBaseDirectory", projectBaseDirectory);
      return true;
    }
  } catch (...) {
    // scream into the void, we don't care
  }
  return false;
}

void Settings::serialize(std::string_view path) {
  if (std::ofstream f(path.data()); f.is_open()) {
    const nlohmann::json parser{{"window", window},               //
                                {"mru", mru},                     //
                                {"lastDirectory", lastDirectory}, //
                                {"uiScale", uiScale},             //
                                {"fontSize", fontSize},           //
                                {"monoFontSize", monoFontSize},   //
                                {"projectBaseDirectory", projectBaseDirectory}};
    f << parser.dump(4);
  }
}