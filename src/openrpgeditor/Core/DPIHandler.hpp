#pragma once

#include <SDL2/SDL.h>
#include <imgui.h>

#include "Core/Window.hpp"

namespace App {

struct WindowSize {
  int width;
  int height;
};

class DPIHandler {
  static float font_scale;

public:
  [[nodiscard]] static float get_ui_scale();
  [[nodiscard]] static WindowSize get_dpi_aware_window_size(const Window::Settings& settings);

  static void set_render_scale(SDL_Renderer* renderer);
  static void set_global_font_scaling(ImGuiIO* io);

  static void set_font_scale(const float fs) { font_scale = fs; }
  static float scale_value(float v) { return v * get_ui_scale(); }
  static float scale_font_value(float v) { return v * font_scale; }
};
} // namespace App
