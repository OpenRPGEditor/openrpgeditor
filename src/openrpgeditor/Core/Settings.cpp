#include "Core/Settings.hpp"
#include <fstream>

Settings* Settings::m_instance = nullptr;

void to_json(nlohmann::ordered_json& j, const Settings::WindowRect& r) {
  j = {
      {"x", r.x}, {"y", r.y}, {"w", r.w}, {"h", r.h}, {"maximized", r.maximized},
  };
}

void from_json(const nlohmann::ordered_json& j, Settings::WindowRect& r) {
  r.x = j.value("x", r.x);
  r.y = j.value("y", r.y);
  r.w = j.value("w", r.w);
  r.h = j.value("h", r.h);
  r.maximized = j.value("maximized", r.maximized);
}

Settings::Settings() { m_instance = this; }
bool Settings::load(std::string_view path) {
  try {
    if (std::ifstream f(path.data()); f.is_open()) {
      nlohmann::ordered_json parser = nlohmann::ordered_json::parse(f);
      window = parser.value("window", window);
      mru = parser.value("mru", mru);
      lastDirectory = parser.value("lastDirectory", lastDirectory);
      lastProject = parser.value("lastProject", lastProject);
      projectBaseDirectory = parser.value("projectBaseDirectory", projectBaseDirectory);
      rpgMakerLocation = parser.value("rpgMakerLocation", rpgMakerLocation);
      rpgMakerVersion = parser.value("rpgMakerVersion", rpgMakerVersion);
      uiScale = parser.value("uiScale", uiScale);
      fontSize = parser.value("fontSize", fontSize);
      monoFontSize = parser.value("monoFontSize", monoFontSize);
      currentNWJSVersion = parser.value("currentNWJSVersion", currentNWJSVersion);
      ranFirstBootWizard = parser.value("ranFirstBootWizard", ranFirstBootWizard);
      return true;
    }
  } catch (...) {
    // scream into the void, we don't care
  }
  return false;
}

void Settings::serialize(std::string_view path) {
  if (std::ofstream f(path.data()); f.is_open()) {
    const nlohmann::ordered_json parser{{"window", window},                             //
                                        {"mru", mru},                                   //
                                        {"lastDirectory", lastDirectory},               //
                                        {"lastProject", lastProject},                   //
                                        {"projectBaseDirectory", projectBaseDirectory}, //
                                        {"rpgMakerLocation", rpgMakerLocation},         //
                                        {"rpgMakerVersion", rpgMakerVersion},           //
                                        {"uiScale", uiScale},                           //
                                        {"fontSize", fontSize},                         //
                                        {"monoFontSize", monoFontSize},                 //
                                        {"currentNWJSVersion", currentNWJSVersion},     //
                                        {"ranFirstBootWizard", ranFirstBootWizard}};    //
    f << parser.dump(4);
  }
}