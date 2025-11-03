#pragma once

#include "Database/Globals.hpp"
#include "nlohmann/json.hpp"

#include <deque>
#include <SDL3/SDL_video.h>

#define ORE_CHECK_DEBUG_BEGIN() if (Settings::instance()->enableDebugFeatures) {
#define ORE_CHECK_DEBUG_END() }

#define ORE_CHECK_EXPERIMENTAL_BEGIN() if (Settings::instance()->enableExperimentalFeatures) {
#define ORE_CHECK_EXPERIMENTAL_END() }

#define ORE_DISABLE_EXPERIMENTAL_BEGIN() ImGui::BeginDisabled(!Settings::instance()->enableExperimentalFeatures)

#define ORE_DISABLE_EXPERIMENTAL_END() ImGui::EndDisabled()

struct Settings {
  struct WindowRect {
    friend void to_json(nlohmann::ordered_json& j, const WindowRect& r);
    friend void from_json(const nlohmann::ordered_json& j, WindowRect& r);
    int x{SDL_WINDOWPOS_CENTERED};
    int y{SDL_WINDOWPOS_CENTERED};
    int w{1920};
    int h{1080};
    bool maximized{false};
  };

  struct Plugin {
    friend void to_json(nlohmann::ordered_json& j, const Plugin& r);
    friend void from_json(const nlohmann::ordered_json& j, Plugin& p);
    bool enabled{false};
    bool compiled{false};
  };

  Settings();
  bool load(const std::filesystem::path& path);
  bool loadFromJson(const nlohmann::ordered_json& parser);
  void serialize(const std::filesystem::path& path);
  nlohmann::ordered_json serializeToJson();
  WindowRect window{};
  std::deque<std::pair<std::string, std::string>> mru;
  std::map<std::string, Plugin> plugins; // Base directory -> settings
  std::string lastDirectory;
  std::string lastProject;
  std::string projectBaseDirectory;
  std::string lcfProjectDirectory;
  std::string rpgMakerMVLocation;
  std::string rpgMakerMZLocation;
  std::string locale;
  int rpgMakerMVVersion{-1};
  int rpgMakerMZVersion{-1};
  float uiScale{1.0f};
  int maxMru{10};
  std::string selectedNWJSVersion;
  std::string currentNWJSVersion;
  bool ranFirstBootWizard{false};

  bool enableExperimentalFeatures{false};
  bool enableDebugFeatures
  // We want this enabled by default in debug builds
#ifndef NDEBUG
      {true};
#else
      {false};
#endif

  [[nodiscard]] static Settings* instance() { return m_instance; }

  std::string imguiState;

private:
  static Settings* m_instance;
};

void to_json(nlohmann::ordered_json& j, const Settings::WindowRect& r);
void from_json(const nlohmann::ordered_json& j, Settings::WindowRect& r);