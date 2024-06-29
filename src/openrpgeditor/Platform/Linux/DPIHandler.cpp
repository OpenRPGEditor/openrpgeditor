#include "Core/DPIHandler.hpp"

#include <SDL2/SDL.h>
#include <imgui.h>

#include "Core/Debug/Instrumentor.hpp"
#include "Core/Window.hpp"
#include "Core/Application.hpp"
#include "Core/Settings.hpp"

namespace App {

float DPIHandler::get_ui_scale() {
  APP_PROFILE_FUNCTION();

  return Settings::instance()->uiScale;
}

float DPIHandler::get_font_scale() {
  float default_dpi = Settings::instance()->dpi;
  float dpi{default_dpi};

  SDL_GetDisplayDPI(SDL_GetWindowDisplayIndex(APP->getWindow()->getNativeWindow()), nullptr, &dpi, nullptr);

  return dpi / default_dpi;
}

WindowSize DPIHandler::get_dpi_aware_window_size(const Window::Settings& settings) {
  APP_PROFILE_FUNCTION();

  return {settings.width, settings.height};
}

void DPIHandler::set_render_scale([[maybe_unused]] SDL_Renderer* renderer) {
  APP_PROFILE_FUNCTION();
  // do nothing
}

void DPIHandler::set_global_font_scaling([[maybe_unused]] ImGuiIO* io) {
  APP_PROFILE_FUNCTION();
  // io->FontGlobalScale = get_scale();
}

} // namespace App
