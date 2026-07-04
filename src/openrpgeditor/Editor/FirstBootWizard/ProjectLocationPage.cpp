#include "Editor/FirstBootWizard/ProjectLocationPage.hpp"
#include "Editor/Application.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <SDL3/SDL_dialog.h>

void ProjectLocationPage::draw() {
  ImGui::BeginChild("ProjectLocationPage", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiWindowFlags_AlwaysAutoResize, ImGuiWindowFlags_NoBackground);
  char location[4096];
  strncpy(location, SettingsManager::instance().getValue<std::string>("projectBaseDirectory", {}).c_str(), 4096);
  if (ImGui::LabelOverLineEdit("##project_location_line_edit", "Project Base Directory Location(*)", location, sizeof(location), 0.f, "Sets the directory where projects are stored")) {
    SettingsManager::instance().setValue("projectBaseDirectory", location);
  }
  ImGui::SameLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().FramePadding.y);
  ImGui::BeginGroup();
  {
    ImGui::NewLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y);
    if (ImGui::Button("Choose...")) {
      const auto baseDir = SettingsManager::instance().getValue<std::string>("projectBaseDirectory", {});
      SDL_ShowOpenFolderDialog(
          [](void* userdata, const char* const* fileList, int filter) {
            if (!fileList || !*fileList || filter >= 1) {
              return;
            }
            const std::filesystem::path path{fileList[0]};
            SettingsManager::instance().setValue("projectBaseDirectory", absolute(path).generic_string());
          },
          this, App::APP->getWindow()->getNativeWindow(), !baseDir.empty() ? baseDir.c_str() : nullptr, false);
    }
    ImGui::SetItemTooltip("Select a directory to store RPG Maker game projects");
  }
  ImGui::EndGroup();
  ImGui::EndChild();
  m_allRequiredFieldsAreSet = std::filesystem::is_directory(SettingsManager::instance().getValue<std::string>("projectBaseDirectory", {}));
}
