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

void to_json(nlohmann::ordered_json& j, const Settings::Plugin& p) {
  j = {
      {"enabled", p.enabled},
      {"compiled", p.compiled},
  };
}

void from_json(const nlohmann::ordered_json& j, Settings::Plugin& p) {
  p.enabled = j.value("enabled", p.enabled);
  p.compiled = j.value("compiled", p.compiled);
}

Settings::Settings() { m_instance = this; }
bool Settings::load(std::string_view path) {
  try {
    if (std::ifstream f(path.data()); f.is_open()) {
      return loadFromJson(nlohmann::ordered_json::parse(f));
    }
  } catch (...) {
    // scream into the void, we don't care
  }
  return false;
}

bool Settings::loadFromJson(const nlohmann::ordered_json& parser) {
  try {
    window = parser.value("window", window);
    mru = parser.value("mru", mru);
    plugins = parser.value("plugins", plugins);
    lastDirectory = parser.value("lastDirectory", lastDirectory);
    lastProject = parser.value("lastProject", lastProject);
    projectBaseDirectory = parser.value("projectBaseDirectory", projectBaseDirectory);
    rpgMakerLocation = parser.value("rpgMakerLocation", rpgMakerLocation);
    rpgMakerVersion = parser.value("rpgMakerVersion", rpgMakerVersion);
    uiScale = parser.value("uiScale", uiScale);
    fontSize = parser.value("fontSize", fontSize);
    monoFontSize = parser.value("monoFontSize", monoFontSize);
    maxMru = parser.value("maxMru", maxMru);
    currentNWJSVersion = parser.value("currentNWJSVersion", currentNWJSVersion);
    ranFirstBootWizard = parser.value("ranFirstBootWizard", ranFirstBootWizard);
    return true;
  } catch (...) {}

  return false;
}

void Settings::serialize(const std::string_view path) {
  if (std::ofstream f(path.data()); f.is_open()) {
    f << serializeToJson().dump(4);
  }
}

nlohmann::ordered_json Settings::serializeToJson() {
  return {{"window", window},                             //
          {"mru", mru},                                   //
          {"plugins", plugins},                           //
          {"lastDirectory", lastDirectory},               //
          {"lastProject", lastProject},                   //
          {"projectBaseDirectory", projectBaseDirectory}, //
          {"rpgMakerLocation", rpgMakerLocation},         //
          {"rpgMakerVersion", rpgMakerVersion},           //
          {"uiScale", uiScale},                           //
          {"fontSize", fontSize},                         //
          {"monoFontSize", monoFontSize},                 //
          {"maxMru", maxMru},                             //
          {"currentNWJSVersion", currentNWJSVersion},     //
          {"ranFirstBootWizard", ranFirstBootWizard}};    //
}