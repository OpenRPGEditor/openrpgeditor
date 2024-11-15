#include "Core/DPIHandler.hpp"

#include <SDL2/SDL.h>
#include <imgui.h>

#include "Core/Debug/Instrumentor.hpp"
#include "Core/Window.hpp"
#include "Core/Settings.hpp"

namespace App {

float DPIHandler::get_ui_scale() {
  APP_PROFILE_FUNCTION();

  return Settings::instance()->uiScale;
}

WindowSize DPIHandler::get_dpi_aware_window_size(const Window::Settings& settings) {
  APP_PROFILE_FUNCTION();

  const float scale{DPIHandler::get_ui_scale()};
  const int width{static_cast<int>(static_cast<float>(settings.width) * scale)};
  const int height{static_cast<int>(static_cast<float>(settings.height) * scale)};
  return {width, height};
}

void DPIHandler::set_render_scale([[maybe_unused]] SDL_Renderer* renderer) {
  APP_PROFILE_FUNCTION();
  // do nothing
}

void DPIHandler::set_global_font_scaling([[maybe_unused]] ImGuiIO* io) {
  APP_PROFILE_FUNCTION();
  // do nothing
}

}  // namespace App
