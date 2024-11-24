#include "Core/FirstBootWizard/ProjectLocationPage.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Settings.hpp"
#include <imgui.h>
#include <nfd.h>

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
      nfdu8char_t* loc;
      const auto result = NFD_PickFolder(&loc, !Settings::instance()->projectBaseDirectory.empty() ? Settings::instance()->projectBaseDirectory.c_str() : nullptr);
      if (result == NFD_OKAY) {
        const std::filesystem::path path{loc};
        Settings::instance()->projectBaseDirectory = absolute(path).generic_string();
        NFD_FreePathU8(loc);
      }
    }
    ImGui::SetItemTooltip("Select a directory to store RPG Maker game projects");
  }
  ImGui::EndGroup();
  ImGui::EndChild();
  m_allRequiredFieldsAreSet = std::filesystem::is_directory(Settings::instance()->projectBaseDirectory);
}
