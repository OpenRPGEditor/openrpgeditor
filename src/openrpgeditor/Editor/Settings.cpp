#include "Editor/Settings.hpp"
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
bool Settings::load(const std::filesystem::path& path) {
  try {
    if (std::ifstream f(path); f.is_open()) {
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
    lcfProjectDirectory = parser.value("lcfProjectDirectory", lcfProjectDirectory);
    rpgMakerMVLocation = parser.value("rpgMakerMVLocation", rpgMakerMVLocation);
    rpgMakerMZLocation = parser.value("rpgMakerMZLocation", rpgMakerMZLocation);
    rpgMakerMVVersion = parser.value("rpgMakerMVVersion", rpgMakerMVVersion);
    rpgMakerMZVersion = parser.value("rpgMakerMZVersion", rpgMakerMZVersion);
    locale = parser.value("locale", locale);
    uiScale = parser.value("uiScale", uiScale);
    maxMru = parser.value("maxMru", maxMru);
    selectedNWJSVersion = parser.value("selectedNWJSVersion", selectedNWJSVersion);
    currentNWJSVersion = parser.value("currentNWJSVersion", currentNWJSVersion);
    ranFirstBootWizard = parser.value("ranFirstBootWizard", ranFirstBootWizard);
    enableExperimentalFeatures = parser.value("enableExperimentalFeatures", enableExperimentalFeatures);
    enableDebugFeatures = parser.value("enableDebugFeatures", enableDebugFeatures);
    imguiState = parser.value("imguiState", imguiState);
    return true;
  } catch (...) {}

  return false;
}
void Settings::serialize(const std::filesystem::path& path) {
  if (std::ofstream f(path); f.is_open()) {
    f << serializeToJson().dump(4);
  }
}

void from_json(const nlohmann::ordered_json& json, Settings& settings) {
  settings.window = json.value("window", settings.window);
  settings.mru = json.value("mru", settings.mru);
  settings.plugins = json.value("plugins", settings.plugins);
  settings.lastDirectory = json.value("lastDirectory", settings.lastDirectory);
  settings.lastProject = json.value("lastProject", settings.lastProject);
  settings.projectBaseDirectory = json.value("projectBaseDirectory", settings.projectBaseDirectory);
  settings.lcfProjectDirectory = json.value("lcfProjectDirectory", settings.lcfProjectDirectory);
  settings.rpgMakerMVLocation = json.value("rpgMakerMVLocation", settings.rpgMakerMVLocation);
  settings.rpgMakerMZLocation = json.value("rpgMakerMZLocation", settings.rpgMakerMZLocation);
  settings.rpgMakerMVVersion = json.value("rpgMakerMVVersion", settings.rpgMakerMVVersion);
  settings.rpgMakerMZVersion = json.value("rpgMakerMZVersion", settings.rpgMakerMZVersion);
  settings.locale = json.value("locale", settings.locale);
  settings.uiScale = json.value("uiScale", settings.uiScale);
  settings.maxMru = json.value("maxMru", settings.maxMru);
  settings.selectedNWJSVersion = json.value("selectedNWJSVersion", settings.selectedNWJSVersion);
  settings.currentNWJSVersion = json.value("currentNWJSVersion", settings.currentNWJSVersion);
  settings.ranFirstBootWizard = json.value("ranFirstBootWizard", settings.ranFirstBootWizard);
  settings.enableExperimentalFeatures = json.value("enableExperimentalFeatures", settings.enableExperimentalFeatures);
  settings.enableDebugFeatures = json.value("enableDebugFeatures", settings.enableDebugFeatures);
  settings.imguiState = json.value("imguiState", settings.imguiState);
}

nlohmann::ordered_json Settings::serializeToJson() {
  return {
      {"window", window},                                         //
      {"mru", mru},                                               //
      {"plugins", plugins},                                       //
      {"lastDirectory", lastDirectory},                           //
      {"lastProject", lastProject},                               //
      {"projectBaseDirectory", projectBaseDirectory},             //
      {"lcfProjectDirectory", lcfProjectDirectory},               //
      {"rpgMakerMVLocation", rpgMakerMVLocation},                 //
      {"rpgMakerMZLocation", rpgMakerMZLocation},                 //
      {"rpgMakerMVVersion", rpgMakerMVVersion},                   //
      {"rpgMakerMZVersion", rpgMakerMZVersion},                   //
      {"locale", locale},                                         //
      {"uiScale", uiScale},                                       //
      {"maxMru", maxMru},                                         //
      {"selectedNWJSVersion", selectedNWJSVersion},               //
      {"currentNWJSVersion", currentNWJSVersion},                 //
      {"ranFirstBootWizard", ranFirstBootWizard},                 //
      {"enableExperimentalFeatures", enableExperimentalFeatures}, //
      {"enableDebugFeatures", enableDebugFeatures},               //
      {"imguiState", imguiState},                                 //
  };
}

void to_json(nlohmann::ordered_json& json, const Settings& settings) {
  json = {
      {"window", settings.window},                                         //
      {"mru", settings.mru},                                               //
      {"plugins", settings.plugins},                                       //
      {"lastDirectory", settings.lastDirectory},                           //
      {"lastProject", settings.lastProject},                               //
      {"projectBaseDirectory", settings.projectBaseDirectory},             //
      {"lcfProjectDirectory", settings.lcfProjectDirectory},               //
      {"rpgMakerMVLocation", settings.rpgMakerMVLocation},                 //
      {"rpgMakerMZLocation", settings.rpgMakerMZLocation},                 //
      {"rpgMakerMVVersion", settings.rpgMakerMVVersion},                   //
      {"rpgMakerMZVersion", settings.rpgMakerMZVersion},                   //
      {"locale", settings.locale},                                         //
      {"uiScale", settings.uiScale},                                       //
      {"maxMru", settings.maxMru},                                         //
      {"selectedNWJSVersion", settings.selectedNWJSVersion},               //
      {"currentNWJSVersion", settings.currentNWJSVersion},                 //
      {"ranFirstBootWizard", settings.ranFirstBootWizard},                 //
      {"enableExperimentalFeatures", settings.enableExperimentalFeatures}, //
      {"enableDebugFeatures", settings.enableDebugFeatures},               //
      {"imguiState", settings.imguiState},                                 //
  };
}