#pragma once

#include <SDL2/SDL.h>

#include <string>

namespace App {

class Window {
 public:
  struct Settings {
    std::string title;
    int width{1280};
    int height{720};
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
    SDL_SetWindowTitle(m_window, title.data());
  }

 private:
  SDL_Window* m_window{nullptr};
  SDL_Renderer* m_renderer{nullptr};
};

}  // namespace App
