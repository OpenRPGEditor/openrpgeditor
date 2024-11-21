#include "Core/DPIHandler.hpp"

#include <SDL2/SDL.h>
#include <imgui.h>

#include "Core/Debug/Instrumentor.hpp"
#include "Core/Window.hpp"
#include "Core/Settings.hpp"

namespace App {

float DPIHandler::get_ui_scale() {
  return Settings::instance()->uiScale;
}

WindowSize DPIHandler::get_dpi_aware_window_size(const Window::Settings& settings) {
  return {settings.width, settings.height};
}

void DPIHandler::set_render_scale([[maybe_unused]] SDL_Renderer* renderer) {
  // do nothing
}

void DPIHandler::set_global_font_scaling([[maybe_unused]] ImGuiIO* io) {
  // do nothing
}

}  // namespace App
