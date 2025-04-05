#include "Core/FirstBootWizard/ProjectLocationPage.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Settings.hpp"

#include <SDL3/SDL_dialog.h>
#include <imgui.h>

void ProjectLocationPage::draw() {
  ImGui::BeginChild("ProjectLocationPage", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiWindowFlags_AlwaysAutoResize, ImGuiWindowFlags_NoBackground);
  char location[4096];
  strncpy(location, Settings::instance()->projectBaseDirectory.c_str(), 4096);
  if (ImGui::LabelOverLineEdit("##project_location_line_edit", "Project Base Directory Location(*)", location, sizeof(location), 0.f, "Sets the directory where projects are stored")) {
    Settings::instance()->projectBaseDirectory = location;
  }
  ImGui::SameLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().FramePadding.y);
  ImGui::BeginGroup();
  {
    ImGui::NewLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y);
    if (ImGui::Button("Choose...")) {
      SDL_ShowOpenFolderDialog(
          [](void* userdata, const char* const* fileList, int filter) {
            if (!fileList || !*fileList || filter >= 1) {
              return;
            }
            const std::filesystem::path path{fileList[0]};
            Settings::instance()->projectBaseDirectory = absolute(path).generic_string();
          },
          this, App::APP->getWindow()->getNativeWindow(), !Settings::instance()->projectBaseDirectory.empty() ? Settings::instance()->projectBaseDirectory.c_str() : nullptr, false);
    }
    ImGui::SetItemTooltip("Select a directory to store RPG Maker game projects");
  }
  ImGui::EndGroup();
  ImGui::EndChild();
  m_allRequiredFieldsAreSet = std::filesystem::is_directory(Settings::instance()->projectBaseDirectory);
}
