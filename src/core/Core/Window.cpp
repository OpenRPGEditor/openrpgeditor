#include "Window.hpp"

#include <SDL2/SDL.h>

#include "Core/DPIHandler.hpp"
#include "Core/Debug/Instrumentor.hpp"
#include "Core/Log.hpp"

namespace App {

Window::Window(const Settings& settings) {
  APP_PROFILE_FUNCTION();

  const auto window_flags{
      static_cast<SDL_WindowFlags>(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI)};
  const WindowSize size{DPIHandler::get_dpi_aware_window_size(settings)};

  m_window = SDL_CreateWindow(settings.title.c_str(),
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      size.width,
      size.height,
      window_flags);

  auto renderer_flags{
      static_cast<SDL_RendererFlags>(SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED)};
  m_renderer = SDL_CreateRenderer(m_window, -1, renderer_flags);

  if (m_renderer == nullptr) {
    APP_ERROR("Error creating SDL_Renderer!");
    return;
  }

  SDL_RendererInfo info;
  SDL_GetRendererInfo(m_renderer, &info);
  DPIHandler::set_render_scale(m_renderer);

  APP_DEBUG("Current SDL_Renderer: {}", info.name);
}

Window::~Window() {
  APP_PROFILE_FUNCTION();

  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
}

SDL_Window* Window::getNativeWindow() const {
  APP_PROFILE_FUNCTION();

  return m_window;
}

SDL_Renderer* Window::getNativeRenderer() const {
  APP_PROFILE_FUNCTION();

  return m_renderer;
}

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

void Window::setMaximized() {
  SDL_MaximizeWindow(m_window);
}

void Window::setWindowSize(int width, int height) {
  SDL_SetWindowSize(m_window, width, height);
}

int Window::getPositionX() const {
  int x {};
  SDL_GetWindowPosition(m_window, &x, nullptr);
  return x;
}
int Window::getPositionY() const {
  int y{};
  SDL_GetWindowPosition(m_window, nullptr, &y);
  return y;
}

void Window::setWindowPosition(int x, int y) {
  SDL_DisplayMode dm;
  SDL_GetCurrentDisplayMode(0, &dm);
  if (x < 0 || x + getWidth() > dm.w) {
    x = SDL_WINDOWPOS_CENTERED;
  }
  if (y < 0 || y + getHeight() > dm.h) {
    y = SDL_WINDOWPOS_CENTERED;
  }

  SDL_SetWindowPosition(m_window, x, y);
}

}  // namespace App
