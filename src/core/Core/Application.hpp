#pragma once

#include <SDL2/SDL.h>

#include <memory>
#include <string>

#include "Core/Window.hpp"

#include "Core/Project.hpp"

#include <optional>

struct ImFont;
class Project;

namespace App {

enum class ExitStatus : int { Success = 0, Failure = 1 };

class Application {
public:
  explicit Application(const std::string& title);
  ~Application();

  Application(const Application&) = delete;
  Application(Application&&) = delete;
  Application& operator=(Application other) = delete;
  Application& operator=(Application&& other) = delete;

  ExitStatus run();
  void stop();

  void on_event(const SDL_WindowEvent& event);
  void on_minimize();
  void on_shown();
  void on_close();

  const std::unique_ptr<Window>& getWindow() const { return m_window; }

  [[nodiscard]] ImFont* getMainFont() const { return m_mainFont; }
  [[nodiscard]] ImFont* getJpFont() const { return m_jpFont; }
  [[nodiscard]] ImFont* getCJKFont() const { return m_cjkFont; }
  [[nodiscard]] ImFont* getSinhalaFont() const { return m_sinhalaFont; }
  [[nodiscard]] ImFont* getMathFont() const { return m_mathFont; }
  [[nodiscard]] ImFont* getMonoFont() const { return m_monoFont; }

private:
  ExitStatus m_exitStatus{ExitStatus::Success};
  std::unique_ptr<Window> m_window{nullptr};

  bool m_running{true};
  bool m_minimized{false};
  Project m_project;
  ImFont* m_mainFont{};
  ImFont* m_jpFont{};
  ImFont* m_cjkFont{};
  ImFont* m_sinhalaFont{};
  ImFont* m_mathFont{};
  ImFont* m_monoFont{};
};

extern Application* APP;

} // namespace App
