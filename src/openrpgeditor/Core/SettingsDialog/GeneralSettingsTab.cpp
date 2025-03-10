#include "Core/SettingsDialog/GeneralSettingsTab.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Settings.hpp"

#include <imgui.h>
#include <nfd.h>

void GeneralSettingsTab::draw() {
  if (ImGui::BeginTabItem("General")) {
    ImGui::TextUnformatted("Max Recent Projects");
    ImGui::SliderInt("##mru_max", &Settings::instance()->maxMru, 1, 20);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
      Settings::instance()->maxMru = std::clamp(Settings::instance()->maxMru, 1, 20);
      onValueChanged.fire();
    }
    // TODO: Undo/Redo stack settings

    char location[4096];
    strncpy(location, Settings::instance()->projectBaseDirectory.c_str(), 4096);
    ImGui::LabelOverLineEdit("##project_location_line_edit", "Project Base Directory Location", location, sizeof(location), 0.f, "Sets the directory where projects are stored");
    if (ImGui::IsItemDeactivatedAfterEdit()) {
      Settings::instance()->projectBaseDirectory = location;
      onValueChanged.fire();
    }
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().FramePadding.y);
    ImGui::BeginGroup();
    {
      ImGui::NewLine();
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y);
      if (ImGui::Button("Choose...")) {
        nfdu8char_t* loc;
        if (NFD_PickFolder(&loc, !Settings::instance()->projectBaseDirectory.empty() ? Settings::instance()->projectBaseDirectory.c_str() : nullptr) == NFD_OKAY) {
          const std::filesystem::path path{loc};
          Settings::instance()->projectBaseDirectory = absolute(path).generic_string();
          onValueChanged.fire();
          NFD_FreePathU8(loc);
        }
      }
      ImGui::SetItemTooltip("Select a directory to store RPG Maker game projects");
    }
    ImGui::EndGroup();

    strncpy(location, Settings::instance()->lcfProjectDirectory.c_str(), 4096);
    ImGui::LabelOverLineEdit("##lcf_location_line_edit", "LCF Base Project Directory", location, sizeof(location), 0.f, "Sets the base directory where LCF will import from");
    if (ImGui::IsItemDeactivatedAfterEdit()) {
      Settings::instance()->lcfProjectDirectory = location;
      onValueChanged.fire();
    }
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().FramePadding.y);
    ImGui::BeginGroup();
    {
      ImGui::NewLine();
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y);
      if (ImGui::Button("Choose...##2")) {
        nfdu8char_t* loc;
        if (NFD_PickFolder(&loc, !Settings::instance()->lcfProjectDirectory.empty() ? Settings::instance()->lcfProjectDirectory.c_str() : nullptr) == NFD_OKAY) {
          const std::filesystem::path path{loc};
          Settings::instance()->lcfProjectDirectory = absolute(path).generic_string();
          onValueChanged.fire();
          NFD_FreePathU8(loc);
        }
      }
      ImGui::SetItemTooltip("Select a directory where LCF will import from");
    }
    ImGui::EndGroup();

    ImGui::EndTabItem();
  }
}
