#include "Core/FirstBootWizard/RPGMakerLocationAndVersionPage.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Settings.hpp"

#include <imgui.h>
#include <nfd.h>
#include <filesystem>
#include <Database/Versions.hpp>

void RPGMakerLocationAndVersionPage::draw() {
  ImGui::BeginChild("RPGMakerLocationAndVersionPage", ImVec2(0, 0),
                    ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiWindowFlags_AlwaysAutoResize,
                    ImGuiWindowFlags_NoBackground);
  char location[4096];
  strncpy(location, Settings::instance()->rpgMakerLocation.c_str(), 4096);
  if (ImGui::LabelOverLineEdit("##rpgmaker_location_line_edit", "RPG Maker Location(*)", location, sizeof(location),
                               0.f)) {
    Settings::instance()->rpgMakerLocation = location;
  }
  ImGui::SameLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().FramePadding.y);
  ImGui::BeginGroup();
  {
    ImGui::NewLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y);
    if (ImGui::Button("Choose...")) {
      nfdu8char_t* loc;
      const auto result = NFD_PickFolder(&loc, !Settings::instance()->rpgMakerLocation.empty()
                                                   ? Settings::instance()->rpgMakerLocation.c_str()
                                                   : nullptr);
      if (result == NFD_OKAY) {
        const std::filesystem::path path{loc};
        Settings::instance()->rpgMakerLocation = absolute(path).generic_string();
        NFD_FreePathU8(loc);
      }
    }
  }
  ImGui::EndGroup();
  ImGui::BeginGroup();
  {
    ImGui::Text("Version(*)");
    if (ImGui::BeginCombo("##rpgmaker_version_combo",
                          Settings::instance()->rpgMakerVersion != -1
                              ? KnownRPGMVVersions[Settings::instance()->rpgMakerVersion].data()
                              : "Please select a version")) {
      for (int i = 0; i < KnownRPGMVVersions.size(); ++i) {
        if (ImGui::Selectable(KnownRPGMVVersions[i].data(), i == Settings::instance()->rpgMakerVersion)) {
          Settings::instance()->rpgMakerVersion = i;
        }
        if (Settings::instance()->rpgMakerVersion == i) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
  }
  ImGui::EndGroup();
  ImGui::EndChild();

  m_allRequiredFieldsAreSet = std::filesystem::is_directory(Settings::instance()->rpgMakerLocation) &&
                              Settings::instance()->rpgMakerVersion != -1;
}
