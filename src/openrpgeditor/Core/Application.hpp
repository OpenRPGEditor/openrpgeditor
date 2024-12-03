#pragma once

#include "FirstBootWizard.hpp"

#include <SDL2/SDL.h>

#include <memory>
#include <string>

#include "Core/Window.hpp"
#include "Project.hpp"

#include <optional>

struct ImFont;
class Project;

namespace App {

enum class ExitStatus : int { Success = 0, Failure = 1 };

class Application {
public:
  void loadSettings();
  void serializeSettings();
  explicit Application(const std::string& title);
  ~Application();
  void updateScale();
  void updateGuiColors();
  void requestFontUpdate() {
    m_fontUpdateRequested = true;
    m_fontUpdateDelay = 1;
  }

  bool fontUpdateRequestPerformed() const { return m_fontUpdateDelay <= 0 && !m_fontUpdateRequested; }

  Application(const Application&) = delete;
  Application(Application&&) = delete;
  Application& operator=(Application other) = delete;
  Application& operator=(Application&& other) = delete;

  ExitStatus run();
  void stop();

  void onEvent(const SDL_WindowEvent& event);
  void onMinimize();
  void onShown();
  void onClose();

  const std::unique_ptr<Window>& getWindow() const { return m_window; }

  [[nodiscard]] ImFont* getMainFont() const { return m_mainFont; }
  [[nodiscard]] ImFont* getMonoFont() const { return m_monoFont; }

private:
  void updateFonts();
  ExitStatus m_exitStatus{ExitStatus::Success};
  std::unique_ptr<Window> m_window{nullptr};

  bool m_running{true};
  bool m_minimized{false};
  std::string m_userConfigPath;
  Settings m_settings;
  Project m_project;
  ImFont* m_mainFont{};
  ImFont* m_monoFont{};
  std::optional<FirstBootWizard> m_firstBootWizard;
  bool m_fontUpdateRequested{false};
  int m_fontUpdateDelay = 1;
};

extern Application* APP;

} // namespace App
