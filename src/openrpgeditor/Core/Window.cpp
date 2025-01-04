#include "Window.hpp"

#include "Core/Debug/Instrumentor.hpp"
#include "Core/Log.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

namespace App {

Window::Window(const Settings& settings) {
  const auto window_flags{static_cast<SDL_WindowFlags>(SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY)};

  m_window = SDL_CreateWindow(settings.title.c_str(), settings.width, settings.height, window_flags);

  m_renderer = SDL_CreateRenderer(m_window, nullptr);

  if (m_renderer == nullptr) {
    APP_ERROR("Error creating SDL_Renderer!");
    return;
  }

  APP_DEBUG("Current SDL_Renderer: {}", SDL_GetRendererName(m_renderer));
}

Window::~Window() {

  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
}

SDL_Window* Window::getNativeWindow() const { return m_window; }

SDL_Renderer* Window::getNativeRenderer() const { return m_renderer; }

int Window::getWidth() const {
  int width{};
  SDL_GetWindowSize(m_window, &width, nullptr);
  return width;
}

int Window::getHeight() const {
  int height{};
  SDL_GetWindowSize(m_window, nullptr, &height);
  return height;
}

void Window::setMaximized() { SDL_MaximizeWindow(m_window); }

void Window::setWindowSize(int width, int height) { SDL_SetWindowSize(m_window, width, height); }

int Window::getPositionX() const {
  int x{};
  SDL_GetWindowPosition(m_window, &x, nullptr);
  return x;
}
int Window::getPositionY() const {
  int y{};
  SDL_GetWindowPosition(m_window, nullptr, &y);
  return y;
}

void Window::setWindowPosition(int x, int y) { SDL_SetWindowPosition(m_window, x, y); }

} // namespace App
