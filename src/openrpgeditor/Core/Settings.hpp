#pragma once

#include "Database/Globals.hpp"
#include "nlohmann/json.hpp"

#include <deque>

struct Settings {
  struct WindowRect {
    friend void to_json(nlohmann::ordered_json& j, const WindowRect& r);
    friend void from_json(const nlohmann::ordered_json& j, WindowRect& r);
    int x{};
    int y{};
    int w{1920};
    int h{1080};
    bool maximized{false};
  };

  Settings();
  bool load(std::string_view path);
  void serialize(std::string_view path);
  WindowRect window{};
  std::deque<std::pair<std::string, std::string>> mru;
  std::string lastDirectory;
  std::string lastProject;
  std::string projectBaseDirectory;
  std::string rpgMakerLocation;
  int rpgMakerVersion{-1};
  float uiScale{1.0};
  int fontSize{12};
  int monoFontSize{12};
  std::string currentNWJSVersion;
  bool ranFirstBootWizard{false};

  [[nodiscard]] static Settings* instance() { return m_instance; }

private:
  static Settings* m_instance;
};

void to_json(nlohmann::ordered_json& j, const Settings::WindowRect& r);
void from_json(const nlohmann::ordered_json& j, Settings::WindowRect& r);