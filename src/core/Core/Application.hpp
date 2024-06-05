#pragma once

#include <SDL2/SDL.h>

#include <memory>
#include <string>

#include "Core/Window.hpp"

#include "Core/Project.hpp"

#include <optional>

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
private:
  ExitStatus m_exitStatus{ExitStatus::Success};
  std::unique_ptr<Window> m_window{nullptr};

  bool m_running{true};
  bool m_minimized{false};
  Project m_project;
};

extern Application* APP;

} // namespace App
