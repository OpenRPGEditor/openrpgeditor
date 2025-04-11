#include "Application.hpp"

#include "App/ProjectInfo.hpp"

#include "Core/Debug/Instrumentor.hpp"
#include "Core/EditorPlugin/EditorPluginManager.hpp"
#include "Core/Log.hpp"
#include "Core/Resources.hpp"
#include "Core/Script/ScriptEngine.hpp"
#include "Core/Settings.hpp"
#include "Core/Window.hpp"
#include "misc/freetype/imgui_freetype.h"

#include "Database/Serializable/FileQueue.hpp"
#include "FirstBootWizard/ProjectLocationPage.hpp"
#include "FirstBootWizard/RPGMakerLocationAndVersionPage.hpp"
#include "FirstBootWizard/UISettingsPage.hpp"
#include "FirstBootWizard/WelcomePage.hpp"
#include "Script/Bindings.hpp"
#include "Utils/JavaMath.hpp"

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <IconsFontAwesome6.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>
#include <iostream>
#include <libcpuid/libcpuid.h>
#include <memory>
#include <moloader/moloader.hpp>
#include <SDL3/SDL.h>

#include <orei18n.hpp>
#include <string>

namespace App {
constexpr auto SettingsFilename = "config.json"sv;
Application* APP = nullptr;
void Application::loadSettings() { m_settings.load(m_userConfigPath + SettingsFilename.data()); }

Application::Application() {
  const auto curlocale = std::locale("en_US.UTF-8");
  std::cout << "System locale: " << curlocale.name() << std::endl;
  std::locale::global(curlocale);
  if (!cpuid_present()) {
    std::cerr << "CPU does not support this CPU" << std::endl;
  } else {
    std::cout << magic_enum::enum_name<cpu_vendor_t>(cpuid_get_vendor()) << std::endl;
  }
#if !defined(WIN32) && !defined(APPLE)
  // We want the compositor to be enabled
  SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
  /* TODO This needs fixes before we can switch to wayland, for now force x11 */
  // SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland");
  SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11");
#endif

  const unsigned int init_flags{SDL_INIT_VIDEO | SDL_INIT_GAMEPAD};
  if (!SDL_Init(init_flags)) {
    APP_ERROR("Error: {}\n", SDL_GetError());
    m_exitStatus = ExitStatus::Failure;
  }

  const char* conf_path = SDL_GetPrefPath(COMPANY_NAMESPACE, APP_NAME);
  m_userConfigPath = std::string{conf_path};
  SDL_free((void*)conf_path);

  Math::seed(1);
  Math::use2kRandom(true);

  loadSettings();
  if (m_settings.locale.empty()) {
    m_settings.locale = curlocale.name();
  }
  APP_DEBUG("User config path: {}", m_userConfigPath);
  // TODO: Detect system locale and automatically use that on first boot
  moloader::load(m_userConfigPath + "/locales/" + m_settings.locale + ".mo");

  m_window = std::make_unique<Window>(Window::Settings{kApplicationTitle, m_settings.window.w, m_settings.window.h, m_settings.window.x, m_settings.window.y, m_settings.window.maximized});
  APP = this;
  if (!ScriptEngine::instance()->initialize()) {
    APP_FATAL("Failed to initialize script manager");
    abort();
  }
  EditorPluginManager::instance()->initialize();
}

Application::~Application() {
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImPlot::DestroyContext();
  ImGui::DestroyContext();

  SDL_Quit();
}

void Application::updateScale() {
  ImGui::GetStyle() = ImGuiStyle();
  updateGuiColors();

  ImGuiStyle& style = ImGui::GetStyle();

  // style.WindowRounding = 3.f;
  // style.FrameRounding = 1.0f;
  // style.GrabMinSize = 4.5f;
  // style.GrabRounding = 2.0f;
  // style.PopupRounding = 3.5;
  // style.TabBorderSize = 1.5f;
  // style.TabRounding = 3.25f;
  // style.DockingSeparatorSize = 3.f;
  // style.FrameBorderSize = 1.f;
  // style.FramePadding = ImVec2(4, 3);
  // style.ScrollbarSize = 10.f;

  // General window settings
  style.WindowPadding = ImVec2(6, 6);
  style.WindowRounding = 2.5f;
  style.FrameRounding = 2.5f;
  style.ScrollbarRounding = 2.5f;
  style.GrabRounding = 2.5f;
  style.TabRounding = 2.5f;
  style.WindowBorderSize = 0.5f;
  style.FrameBorderSize = 1.0f;
  style.PopupBorderSize = 1.0f;
  style.PopupRounding = 2.5f;

  // Additional styles
  style.FramePadding = ImVec2(4.0f, 3.f);
  style.ItemSpacing = ImVec2(4.0f, 3.f);
  style.IndentSpacing = 10.0f;
  style.ScrollbarSize = 12.f;

  const auto scale = std::max(SDL_GetWindowPixelDensity(m_window->getNativeWindow()), SDL_GetWindowDisplayScale(m_window->getNativeWindow()));
  style.ScaleAllSizes(scale);
}

void Application::updateGuiColors() {
  ImGuiStyle& style = ImGui::GetStyle();
  ImVec4* colors = style.Colors;

  const auto theme = SDL_GetSystemTheme();

  if (theme == SDL_SYSTEM_THEME_DARK) {
    // Setting the colors
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.f);
    colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);

    // Accent colors changed to darker olive-green/grey shades
    colors[ImGuiCol_CheckMark] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);        // Dark gray for check marks
    colors[ImGuiCol_SliderGrab] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);       // Dark gray for sliders
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); // Slightly lighter gray when active
    colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);           // Button background (dark gray)
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);    // Button hover state
    colors[ImGuiCol_ButtonActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);     // Button active state
    colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);           // Dark gray for menu headers
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.35, 0.35, 0.35, 1.00f);       // Slightly darker on hover
    colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);     // Darker gray when active
    colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);        // Separators in dark gray
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f); // Resize grips in dark gray
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);        // Tabs background
    colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f); // Darker gray on hover
    colors[ImGuiCol_TabActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f); // Docking preview in gray
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f); // Empty dock background
  } else if (theme == SDL_SYSTEM_THEME_LIGHT) {
    // Setting the colors (Light version)
    colors[ImGuiCol_Text] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f); // Light background
    colors[ImGuiCol_ChildBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Light frame background
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);

    // Accent colors with a soft pastel gray-green
    colors[ImGuiCol_CheckMark] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f); // Soft gray-green for check marks
    colors[ImGuiCol_SliderGrab] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.60f, 0.70f, 0.60f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Light button background
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f); // Accent color for resize grips
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.60f, 0.70f, 0.60f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.65f, 0.75f, 0.65f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Tabs background
    colors[ImGuiCol_TabHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f); // Docking preview in gray-green
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
  } else {
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.19f, 0.30f, 0.38f, 0.97f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
  }
}

void Application::updateFonts() {
  ImGuiIO& io{ImGui::GetIO()};
  // ImGUI font
  const float scale = std::max(SDL_GetWindowPixelDensity(m_window->getNativeWindow()), SDL_GetWindowDisplayScale(m_window->getNativeWindow()));
  const float font_size = m_settings.fontSize;
  const float mono_font_size = m_settings.monoFontSize;
  const std::string font_path{Resources::font_path("MPLUSRounded1c-Medium.ttf").generic_string()};
  const std::string font_path_sinhala{Resources::font_path("NotoSansSinhala-Medium.ttf").generic_string()};
  const std::string font_path_jetbrains{Resources::font_path("JetBrainsMono-Medium.ttf").generic_string()};
  const std::string font_path_awesome{Resources::font_path("Font Awesome 6 Free-Solid-900.otf").generic_string()};
  const std::string font_path_mono{Resources::font_path("mplus-1m-regular.ttf").generic_string()};

  static constexpr ImWchar specialChar[] = {
      /* clang-format off */
      0x0001, 0xFFFF,
      0
      /* clang-format on */
  };

  ImVector<ImWchar> ranges;
  ImFontGlyphRangesBuilder builder;
  builder.AddRanges(specialChar);
  builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
  builder.AddRanges(io.Fonts->GetGlyphRangesJapanese());
  builder.BuildRanges(&ranges);
  ImFontConfig config;
  config.RasterizerDensity = scale;
  io.Fonts->Clear();
  config.MergeMode = false;
  m_mainFont = io.FontDefault = io.Fonts->AddFontFromFileTTF(font_path.c_str(), font_size, &config, ranges.Data);
  io.Fonts->Build();
  config.MergeMode = true;
  io.Fonts->AddFontFromFileTTF(font_path_sinhala.c_str(), font_size, &config, ranges.Data);
  io.Fonts->Build();
  io.Fonts->AddFontFromFileTTF(font_path_jetbrains.c_str(), font_size, &config, ranges.Data);
  io.Fonts->Build();
  config.GlyphMinAdvanceX = font_size;
  io.Fonts->AddFontFromFileTTF(font_path_awesome.c_str(), font_size, &config, ranges.Data);
  io.Fonts->AddCustomRectFontGlyph(m_mainFont, ICON_BLANK, font_size, font_size, font_size);
  io.Fonts->Build();

  config = ImFontConfig();
  // config.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_Bold;
  m_monoFont = io.Fonts->AddFontFromFileTTF(font_path_mono.c_str(), mono_font_size, &config, ranges.Data);
  io.Fonts->Build();
  config.MergeMode = true;
  io.Fonts->AddFontFromFileTTF(font_path_sinhala.c_str(), mono_font_size, &config, ranges.Data);
  io.Fonts->Build();
  config.GlyphMinAdvanceX = mono_font_size;
  io.Fonts->AddFontFromFileTTF(font_path_awesome.c_str(), mono_font_size, &config, ranges.Data);
  io.Fonts->AddCustomRectFontGlyph(m_monoFont, ICON_BLANK, mono_font_size, mono_font_size, mono_font_size);
  io.Fonts->Build();
  io.FontGlobalScale = 1.f / scale;

  updateScale();
}

void Application::serializeSettings() {
  if (!Database::instance()) {
    size_t len = 0;
    const char* state = ImGui::SaveIniSettingsToMemory(&len);
    m_settings.imguiState = {state, len};
  }
  m_settings.serialize(m_userConfigPath + SettingsFilename.data());
}
ExitStatus Application::run() {
  /* Do an initial clear */
  SDL_SetRenderDrawColor(m_window->getNativeRenderer(), 28, 38, 43, 255);
  SDL_RenderClear(m_window->getNativeRenderer());
  SDL_RenderPresent(m_window->getNativeRenderer());

  if (m_exitStatus == ExitStatus::Failure) {
    return m_exitStatus;
  }

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  /* Allocate a much larger temp buffer for large strings */
  // ImGui::GetCurrentContext()->TempBuffer.resize(8192 * 3 + 2);
  ImPlot::CreateContext();
  ImGuiIO& io{ImGui::GetIO()};

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_DpiEnableScaleFonts |
                    ImGuiConfigFlags_DpiEnableScaleViewports;
  io.ConfigWindowsMoveFromTitleBarOnly = true;
  io.ConfigInputTrickleEventQueue = false;
  // io.ConfigDockingWithShift = true;
  io.IniFilename = nullptr;

  //  Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForSDLRenderer(m_window->getNativeWindow(), m_window->getNativeRenderer());
  ImGui_ImplSDLRenderer3_Init(m_window->getNativeRenderer());
  SDL_GL_SetSwapInterval(0);

  ImGui::LoadIniSettingsFromMemory(m_settings.imguiState.c_str(), m_settings.imguiState.length());
  updateFonts();

  m_running = true;

  m_project.emplace();
  bool needsInitialProjectLoad =
      !Settings::instance()->lastProject.empty() && std::filesystem::is_regular_file(Settings::instance()->lastProject) &&
      (std::filesystem::path(Settings::instance()->lastProject).extension() == ".rpgproject" || std::filesystem::path(Settings::instance()->lastProject).extension() == ".rmmzproject");
  // Check if we need to run the FirstBoot wizard
  if (!Settings::instance()->ranFirstBootWizard) {
    m_firstBootWizard.emplace();
    m_firstBootWizard->addPage(std::make_shared<WelcomePage>());
    m_firstBootWizard->addPage(std::make_shared<UISettingsPage>());
    m_firstBootWizard->addPage(std::make_shared<RPGMakerLocationAndVersionPage>());
    m_firstBootWizard->addPage(std::make_shared<ProjectLocationPage>());
  }

  SDL_ShowWindow(m_window->getNativeWindow());
  SDL_RaiseWindow(m_window->getNativeWindow());
  SDL_FlashWindow(m_window->getNativeWindow(), SDL_FLASH_UNTIL_FOCUSED);

  float frameTime = 0.f;
  float saveTime = 0.f;
  while (m_running || FileQueue::instance().hasTasks() || m_projectSerialize || m_projectCloseRequest) {
    EditorPluginManager::instance()->initializeAllPlugins();

    SDL_Event event{};
    SDL_PumpEvents();
    while (SDL_PollEvent(&event) == 1) {
      ImGui_ImplSDL3_ProcessEvent(&event);

      if (event.type == SDL_EVENT_QUIT) {
        stop();
      }

      if (event.type == SDL_EVENT_SYSTEM_THEME_CHANGED) {
        updateGuiColors();
      }

      if (event.type >= SDL_EVENT_WINDOW_SHOWN && event.type <= SDL_EVENT_WINDOW_HDR_STATE_CHANGED && event.window.windowID == SDL_GetWindowID(m_window->getNativeWindow())) {
        onEvent(event.window);
      }
    }

    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();

    ImGui::NewFrame();
    frameTime += ImGui::GetIO().DeltaTime;
    saveTime += ImGui::GetIO().DeltaTime;

    if (m_firstBootWizard && m_firstBootWizard->draw()) {
      Settings::instance()->ranFirstBootWizard = true;
      m_firstBootWizard.reset();
    }

    if (!m_firstBootWizard) {
      m_project->draw(m_doQuit && m_userClosed);
      if (needsInitialProjectLoad) {
        m_project->load(Settings::instance()->lastProject, std::filesystem::path(Settings::instance()->lastProject).remove_filename().generic_string());
        needsInitialProjectLoad = false;
      }
      if (m_doQuit || m_projectCloseRequest) {
        if (m_project && !m_userClosed) {
          const auto [closed, quit, serialize] = m_project->close(true);
          if (closed && quit) {
            m_running = !m_doQuit;
            m_userClosed = true;
          } else if (closed) {
            m_doQuit = false;
            m_running = true;
            m_projectCloseRequest = false;
            m_userClosed = false;
          }
          m_projectSerialize = serialize;
          if (m_projectSerialize) {
            Database::instance()->serializeProject();
          }
        }
        saveTime = 0.f;
      }

      if (!FileQueue::instance().hasTasks() && m_userClosed && m_projectCloseRequest) {
        // Actually close the project to revert active state
        if (m_project) {
          m_project->close();
        }
        if (m_doQuit) {
          m_project.reset();
        }
        m_projectCloseRequest = false;
        m_userClosed = false;
      }
    }

    // Rendering
    ImGui::Render();

    ImGui::UpdatePlatformWindows();

    SDL_SetRenderDrawColor(m_window->getNativeRenderer(),                       //
                           ImGui::GetStyleColorVec4(ImGuiCol_WindowBg).x * 255, //
                           ImGui::GetStyleColorVec4(ImGuiCol_WindowBg).y * 255, //
                           ImGui::GetStyleColorVec4(ImGuiCol_WindowBg).z * 255, 255);
    SDL_RenderClear(m_window->getNativeRenderer());
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_window->getNativeRenderer());
    SDL_RenderPresent(m_window->getNativeRenderer());

    // Process file queues at every 1000th of a second to prevent UI stalls
#ifndef DEBUG
    if (frameTime >= (1.f / 1000.f)) {
#else
    if (frameTime >= (1.f / 20.f)) {
#endif
      FileQueue::instance().proc();
      frameTime = 0.f;
    }

    if (io.WantSaveIniSettings && saveTime >= (1.f / 60.f)) {
      serializeSettings();
      saveTime = 0.f;
    }
  }

  EditorPluginManager::instance()->shutdownAllPlugins();
  assert(!FileQueue::instance().hasTasks());
  FileQueue::instance().reset();
  return m_exitStatus;
}

void Application::stop() {
  m_doQuit = true;
  m_projectCloseRequest = true;
}

void Application::onEvent(const SDL_WindowEvent& event) {
  switch (event.type) {
  case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
    return onClose();
  case SDL_EVENT_WINDOW_MINIMIZED:
    return onMinimize();
  case SDL_EVENT_WINDOW_SHOWN:
    return onShown();
  case SDL_EVENT_WINDOW_RESIZED: {
    if (!(SDL_GetWindowFlags(m_window->getNativeWindow()) & SDL_WINDOW_MAXIMIZED)) {
      Settings::instance()->window.w = event.data1;
      Settings::instance()->window.h = event.data2;
    }
    Settings::instance()->window.maximized = SDL_GetWindowFlags(m_window->getNativeWindow()) & SDL_WINDOW_MAXIMIZED;
    break;
  }
  case SDL_EVENT_WINDOW_MOVED: {
    Settings::instance()->window.x = event.data1;
    Settings::instance()->window.y = event.data2;
    Settings::instance()->window.maximized = SDL_GetWindowFlags(m_window->getNativeWindow()) & SDL_WINDOW_MAXIMIZED;
    break;
  }
  case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
  case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
    const auto scale = SDL_GetWindowDisplayScale(m_window->getNativeWindow());
    const auto pixelDensity = SDL_GetWindowPixelDensity(m_window->getNativeWindow());
    if (ImGui::GetWindowDpiScale() != scale && ImGui::GetWindowDpiScale() != pixelDensity) {
      requestFontUpdate();
      updateScale();
    }
    break;
  }
  case SDL_EVENT_WINDOW_MAXIMIZED: {
    Settings::instance()->window.maximized = SDL_GetWindowFlags(m_window->getNativeWindow()) & SDL_WINDOW_MAXIMIZED;
    break;
  }
  default:
    // Do nothing otherwise
    return;
  }
}

void Application::onMinimize() { m_minimized = true; }

void Application::onShown() { m_minimized = false; }

void Application::onClose() { stop(); }

void Application::cancelShutdown() {
  FileQueue::instance().reset();
  m_running = true;
  m_userClosed = false;
  m_projectSerialize = false;
  m_projectCloseRequest = false;
  m_doQuit = false;
}

} // namespace App
