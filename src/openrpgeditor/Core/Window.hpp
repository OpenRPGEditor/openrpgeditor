#pragma once

#include <SDL3/SDL.h>

#include <string>

namespace App {

class Window {
public:
  struct Settings {
    std::string title;
    int width{1920};
    int height{1080};
    int x{-1};
    int y{-1};
    int maximized{false};
  };

  explicit Window(const Settings& settings);
  ~Window();

  Window(const Window&) = delete;
  Window(Window&&) = delete;
  Window& operator=(Window other) = delete;
  Window& operator=(Window&& other) = delete;

  [[nodiscard]] SDL_Window* getNativeWindow() const;
  [[nodiscard]] SDL_Renderer* getNativeRenderer() const;

  void setTitle(std::string_view title) {
    m_title = title;
    SDL_SetWindowTitle(m_window, title.data());
  }

  std::string_view getTitle() const { return m_title; }

  [[nodiscard]] int getWidth() const;
  [[nodiscard]] int getHeight() const;
  void setWindowSize(int width, int height);
  [[nodiscard]] int getPositionX() const;
  [[nodiscard]] int getPositionY() const;
  void setWindowPosition(int x, int y);
  void setMaximized();

private:
  SDL_Window* m_window{nullptr};
  SDL_Renderer* m_renderer{nullptr};
  // Cache this so we don't have to query into SDL every time
  std::string m_title;
};

} // namespace App
