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

#include "Database/Serializable/DeserializationQueue.hpp"
#include "Database/Serializable/SerializationQueue.hpp"
#include "FirstBootWizard/ProjectLocationPage.hpp"
#include "FirstBootWizard/RPGMakerLocationAndVersionPage.hpp"
#include "FirstBootWizard/UISettingsPage.hpp"
#include "FirstBootWizard/WelcomePage.hpp"
#include "Script/Bindings.hpp"
#include "Utils/JavaMath.hpp"

#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>
#include <iostream>
#include <libcpuid/libcpuid.h>
#include <memory>
#include <moloader/moloader.hpp>
#include <nfd.h>
#include <orei18n.hpp>
#include <string>

namespace App {
constexpr auto SettingsFilename = "config.json"sv;
Application* APP = nullptr;
void Application::loadSettings() {
  if (m_settings.load(m_userConfigPath + SettingsFilename.data())) {
    m_window->setWindowSize(m_settings.window.w, m_settings.window.h);
    m_window->setWindowPosition(m_settings.window.x, m_settings.window.y);
    if (m_settings.window.maximized) {
      m_window->setMaximized();
    }
  }
}
Application::Application(const std::string& title) {
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

  m_window = std::make_unique<Window>(Window::Settings{title});

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
  style.WindowRounding = 6.f;
  style.FrameRounding = 2.0f;
  // style.ScrollbarSize = 18.f;
  style.GrabMinSize = 9.0f;
  style.GrabRounding = 4.0f;
  style.PopupRounding = 7.0;
  style.TabBorderSize = 3.f;
  style.TabRounding = 6.5f;
  style.DockingSeparatorSize = 6.f;
  style.FrameBorderSize = 1.f;
  style.FramePadding = ImVec2(8, 6);
  style.ScrollbarSize = 18.f;
}

void Application::updateGuiColors() {
  ImGuiStyle& style = ImGui::GetStyle();
  auto* colors = style.Colors;
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

void Application::updateFonts() {
  ImGuiIO& io{ImGui::GetIO()};
  // ImGUI font
  float scale = std::max(1.f, SDL_GetWindowPixelDensity(m_window->getNativeWindow()));
  if (scale > 1.f) {
    scale *= 2.f;
  }
  const float font_size = m_settings.fontSize * scale;
  const float mono_font_size = m_settings.monoFontSize * scale;
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
  builder.BuildRanges(&ranges);
  ImFontConfig config;
  io.Fonts->Clear();
  config.MergeMode = false;
  config.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LoadColor;
  m_mainFont = io.FontDefault = io.Fonts->AddFontFromFileTTF(font_path.c_str(), font_size, &config, ranges.Data);
  config.MergeMode = true;
  io.Fonts->AddFontFromFileTTF(font_path_sinhala.c_str(), font_size, &config, ranges.Data);
  io.Fonts->AddFontFromFileTTF(font_path_awesome.c_str(), font_size, &config, ranges.Data);
  io.Fonts->AddFontFromFileTTF(font_path_jetbrains.c_str(), font_size, &config, ranges.Data);
  io.Fonts->Build();

  config.MergeMode = false;
  config.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_Bold;
  m_monoFont = io.Fonts->AddFontFromFileTTF(font_path_mono.c_str(), mono_font_size, &config, ranges.Data);
  config.MergeMode = true;
  io.Fonts->AddFontFromFileTTF(font_path_sinhala.c_str(), mono_font_size, &config, ranges.Data);
  io.Fonts->AddFontFromFileTTF(font_path_awesome.c_str(), mono_font_size, &config, ranges.Data);
  io.Fonts->Build();
  io.FontGlobalScale = scale == 1.f ? scale : 1.f / scale;

  updateScale();
}

void Application::serializeSettings() { m_settings.serialize(m_userConfigPath + SettingsFilename.data()); }
ExitStatus Application::run() {
  NFD_Init();
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

  // Absolute imgui.ini path to preserve settings independent of app location.
  static const std::string imgui_ini_filename{m_userConfigPath + "imgui.ini"};
  io.IniFilename = imgui_ini_filename.c_str();

  updateFonts();

  //  Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForSDLRenderer(m_window->getNativeWindow(), m_window->getNativeRenderer());
  ImGui_ImplSDLRenderer3_Init(m_window->getNativeRenderer());
  SDL_GL_SetSwapInterval(0);

  m_running = true;
  uint32_t a = SDL_GetTicks();
  double delta = 0;
  bool warmup = true;

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
  int warmupFrames = 0;

  while (true) {
    EditorPluginManager::instance()->initializeAllPlugins();
    if (m_fontUpdateRequested && m_fontUpdateDelay <= 0) {
      ImGui_ImplSDLRenderer3_Shutdown();
      ImGui_ImplSDL3_Shutdown();
      updateFonts();
      ImGui_ImplSDL3_InitForSDLRenderer(m_window->getNativeWindow(), m_window->getNativeRenderer());
      ImGui_ImplSDLRenderer3_Init(m_window->getNativeRenderer());
      m_fontUpdateRequested = false;
    }
    if (!m_running) {
      DeserializationQueue::instance().abort();
      SerializationQueue::instance().abort();
      if (m_project) {
        m_project->close();
      }
      break;
    }
    delta = SDL_GetTicks() - a;

    SDL_Event event{};
    while (SDL_PollEvent(&event) == 1) {
      ImGui_ImplSDL3_ProcessEvent(&event);

      if (event.type == SDL_EVENT_QUIT) {
        stop();
      }

      if (event.type >= SDL_EVENT_WINDOW_SHOWN && event.type <= SDL_EVENT_WINDOW_HDR_STATE_CHANGED && event.window.windowID == SDL_GetWindowID(m_window->getNativeWindow())) {
        onEvent(event.window);
      }
    }

    if (delta >= 1000.0 / 60.0 || warmup) {

      if (m_fontUpdateRequested && m_fontUpdateDelay > 0) {
        m_fontUpdateDelay--;
      }

      if (warmup && delta >= 1000.0 / 60.0) {
        a = SDL_GetTicks();
        ++warmupFrames;
      } else if (!warmup) {
        a = SDL_GetTicks();
      }

      if (warmupFrames == 60) {
        warmup = false;
      }

      // Start the Dear ImGui frame
      ImGui_ImplSDLRenderer3_NewFrame();
      ImGui_ImplSDL3_NewFrame();

      // FIXME: Fixup monitors
      // //  Currently crashes for some reason, copying MainPos and MainSize is recommended by the assert, so here we are
      // for (ImGuiPlatformMonitor& mon : ImGui::GetPlatformIO().Monitors) {
      //   mon.WorkPos = mon.MainPos;
      //   mon.WorkSize = mon.MainSize;
      // }

      ImGui::NewFrame();

      if (m_firstBootWizard && m_firstBootWizard->draw()) {
        Settings::instance()->ranFirstBootWizard = true;
        m_firstBootWizard.reset();
      }

      if (!m_firstBootWizard) {
        m_project->draw();
        if (needsInitialProjectLoad) {
          m_project->load(Settings::instance()->lastProject, std::filesystem::path(Settings::instance()->lastProject).remove_filename().generic_string());
          needsInitialProjectLoad = false;
        }
      }

      // Rendering
      ImGui::Render();

      ImGui::EndFrame();
      ImGui::UpdatePlatformWindows();

      SDL_SetRenderDrawColor(m_window->getNativeRenderer(), 28, 38, 43, 255);
      SDL_RenderClear(m_window->getNativeRenderer());
      ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_window->getNativeRenderer());
      SDL_RenderPresent(m_window->getNativeRenderer());
    }
  }

  EditorPluginManager::instance()->shutdownAllPlugins();
  serializeSettings();
  SerializationQueue::instance().terminate();
  DeserializationQueue::instance().terminate();
  NFD_Quit();
  return m_exitStatus;
}

void Application::stop() { m_running = false; }

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
  case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
    // TODO: We should probably handle this
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

void Application::onClose() {
  if (m_project) {
    m_project->close(true);
  }
  stop();
}

} // namespace App
