#include "Application.hpp"

#include <SDL2/SDL.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <memory>
#include <string>

#include "Core/DPIHandler.hpp"
#include "Core/Debug/Instrumentor.hpp"
#include "Core/Log.hpp"
#include "Core/Resources.hpp"
#include "Core/Window.hpp"
#include "Settings/Project.hpp"
#include "Database/Actors.hpp"
#include "ImGuiFileDialog.h"
#include <iostream>
namespace App {

static Actors actors;
Application::Application(const std::string& title) {
  APP_PROFILE_FUNCTION();

#if !defined(WIN32) && !defined(APPLE)
  // We want the compositor to be enabled
  SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
#endif

  const unsigned int init_flags{SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER};
  if (SDL_Init(init_flags) != 0) {
    APP_ERROR("Error: %s\n", SDL_GetError());
    m_exitStatus = ExitStatus::Failure;
  }

  m_window = std::make_unique<Window>(Window::Settings{title});
}

Application::~Application() {
  APP_PROFILE_FUNCTION();

  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_Quit();
}

ExitStatus App::Application::run() {
  APP_PROFILE_FUNCTION();

  if (m_exitStatus == ExitStatus::Failure) {
    return m_exitStatus;
  }

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io{ImGui::GetIO()};

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable |
                    ImGuiConfigFlags_ViewportsEnable;

  const char* conf_path = SDL_GetPrefPath(COMPANY_NAMESPACE, APP_NAME);
  const std::string user_config_path{conf_path};
  SDL_free((void*)conf_path);

  APP_DEBUG("User config path: {}", user_config_path);

  // Absolute imgui.ini path to preserve settings independent of app location.
  static const std::string imgui_ini_filename{user_config_path + "imgui.ini"};
  io.IniFilename = imgui_ini_filename.c_str();

  // ImGUI font
  const float font_scaling_factor{DPIHandler::get_scale()};
  const float font_size{18.0F * font_scaling_factor};
  const std::string font_path{Resources::font_path("Manrope.ttf").generic_string()};

  io.Fonts->AddFontFromFileTTF(font_path.c_str(), font_size);
  io.FontDefault = io.Fonts->AddFontFromFileTTF(font_path.c_str(), font_size);
  DPIHandler::set_global_font_scaling(&io);

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForSDLRenderer(m_window->get_native_window(), m_window->get_native_renderer());
  ImGui_ImplSDLRenderer2_Init(m_window->get_native_renderer());
  SDL_GL_SetSwapInterval(0);


  m_running = true;
  uint32_t a = SDL_GetTicks();
  double delta = 0;

  while (m_running) {
    APP_PROFILE_SCOPE("MainLoop");
    delta = SDL_GetTicks() - a;

    if (delta >= 1000.0/60.0) {
      m_window->setTitle("fps: " + std::to_string(1000.0 / delta));
      a = SDL_GetTicks();

      SDL_Event event{};
      while (SDL_PollEvent(&event) == 1) {
        APP_PROFILE_SCOPE("EventPolling");

        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT) {
          stop();
        }

        if (event.type == SDL_WINDOWEVENT &&
            event.window.windowID == SDL_GetWindowID(m_window->get_native_window())) {
          on_event(event.window);
            }
      }

      // Start the Dear ImGui frame
      ImGui_ImplSDLRenderer2_NewFrame();
      ImGui_ImplSDL2_NewFrame();

      //FIXME: Fixup monitors
      // Currently crashes for some reason, copying MainPos and MainSize is recommended by the assert, so here we are
      for (ImGuiPlatformMonitor& mon : ImGui::GetPlatformIO().Monitors) {
        mon.WorkPos = mon.MainPos;
        mon.WorkSize = mon.MainSize;
      }

      ImGui::NewFrame();

      if (!m_minimized) {
        ImGui::DockSpaceOverViewport();

        if (ImGui::BeginMainMenuBar()) {
          if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Project...", "Ctrl+N")) {
              // TODO: Implement project creation
            }
            if (ImGui::MenuItem("Open Project...", "Ctrl+O")) {
              IGFD::FileDialogConfig config;
              config.path = ".";
              ImGuiFileDialog::Instance()->OpenDialog("OpenProjectDlg", "Select a Project to Open", ".rpgproject", config);
            }
            if (ImGui::MenuItem("Close Project...")) {
              // TODO: Implement project closing
            }
            if (ImGui::MenuItem("Save Project...", "Ctlr+S")) {
              // TODO: Implement project saving
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Deployment...")) {
              // TODO: Implement game deployment
            }
            /*
            if (ImGui::MenuItem("Steam Management")) {
              // TODO: Debate on implementing this, it may not be wise, or worthwhile
            }
            */
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Ctrl+Q")) {
              stop();
            }
            ImGui::EndMenu();
          }

          ImGui::EndMainMenuBar();
        }
      }

      // First check if we have a pending project request
      if (ImGuiFileDialog::Instance()->Display("OpenProjectDlg", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking, ImVec2(300, 300))) {
        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
          std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
          std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
          Project project = Project::load(filePathName, filePath);
          if (project.isLoaded() && project.isValid()) {
            m_project.emplace(std::move(project));
          }
          // action
        }

        // close
        ImGuiFileDialog::Instance()->Close();
      }

      if (m_project && m_project->isValid()) {
        m_project->draw();
      } else if (m_project) {
        m_project.reset();
      }

      // Rendering
      ImGui::Render();

      SDL_SetRenderDrawColor(m_window->get_native_renderer(), 100, 100, 100, 255);
      SDL_RenderClear(m_window->get_native_renderer());
      ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_window->get_native_renderer());
      SDL_RenderPresent(m_window->get_native_renderer());
    }
  }

  return m_exitStatus;
}

void Application::stop() {
  APP_PROFILE_FUNCTION();

  m_running = false;
}

void Application::on_event(const SDL_WindowEvent& event) {
  APP_PROFILE_FUNCTION();

  switch (event.event) {
    case SDL_WINDOWEVENT_CLOSE:
      return on_close();
    case SDL_WINDOWEVENT_MINIMIZED:
      return on_minimize();
    case SDL_WINDOWEVENT_SHOWN:
      return on_shown();
    default:
      // Do nothing otherwise
      return;
  }
}

void Application::on_minimize() {
  APP_PROFILE_FUNCTION();

  m_minimized = true;
}

void Application::on_shown() {
  APP_PROFILE_FUNCTION();

  m_minimized = false;
}

void Application::on_close() {
  APP_PROFILE_FUNCTION();

  stop();
}

}  // namespace App
