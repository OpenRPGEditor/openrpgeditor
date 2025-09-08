#pragma once

#include "Core/ApplicationThemeManager.hpp"
#include "Core/FirstBootWizard.hpp"

#include <SDL3/SDL.h>

#include <memory>
#include <string>

#include "Core/MainWindow.hpp"
#include "Core/Window.hpp"

#include <optional>

struct ImFont;

namespace App {
#define kApplicationTitle trNOOP("Open RPG Editor")

enum class ExitStatus : int { Success = 0, Failure = 1 };

class Application {
public:
  void loadSettings();
  void serializeSettings();
  explicit Application();
  ~Application();
  void updateScale();
  void updateGuiColors();
  void requestScaleUpdate() { m_requestScaleUpdate = true; }

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

  std::string_view userConfigPath() const { return m_userConfigPath; };

  void cancelShutdown();

  void closeProject() {
    m_userClosed = false;
    m_projectCloseRequest = true;
  }

private:
  void updateFonts();
  ExitStatus m_exitStatus{ExitStatus::Success};
  std::unique_ptr<Window> m_window{nullptr};

  bool m_running{true};
  bool m_minimized{false};
  std::string m_userConfigPath;
  Settings m_settings;
  std::optional<MainWindow> m_mainWindow;
  ImFont* m_mainFont{};
  ImFont* m_monoFont{};
  std::optional<FirstBootWizard> m_firstBootWizard;
  ApplicationThemeManager m_themeManager;
  bool m_requestScaleUpdate{false};
  bool m_projectCloseRequest{false};
  bool m_userClosed{false};
  bool m_projectSerialize{false};
  bool m_doQuit{false};
};

extern Application* APP;

} // namespace App
