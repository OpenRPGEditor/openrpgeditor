#include "Core/FirstBootWizard/RPGMakerLocationAndVersionPage.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Settings.hpp"

#include <Database/Versions.hpp>
#include <filesystem>
#include <imgui.h>
#include <nfd.h>

void RPGMakerLocationAndVersionPage::draw() {
  ImGui::BeginChild("RPGMakerLocationAndVersionPage", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiWindowFlags_AlwaysAutoResize, ImGuiWindowFlags_NoBackground);
  char location[4096];
  strncpy(location, Settings::instance()->rpgMakerMVLocation.c_str(), 4096);
  if (ImGui::LabelOverLineEdit("##rpgmaker_location_line_edit", trNOOP("RPG Maker MV Location(*)"), location, sizeof(location), 0.f)) {
    Settings::instance()->rpgMakerMVLocation = location;
  }
  ImGui::SameLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().FramePadding.y);
  ImGui::BeginGroup();
  {
    ImGui::NewLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y);
    if (ImGui::Button(std::format("{}##rpgmakermv", trNOOP("Choose...")).c_str())) {
      nfdu8char_t* loc;
      const auto result = NFD_PickFolder(&loc, !Settings::instance()->rpgMakerMVLocation.empty() ? Settings::instance()->rpgMakerMVLocation.c_str() : nullptr);
      if (result == NFD_OKAY) {
        const std::filesystem::path path{loc};
        Settings::instance()->rpgMakerMVLocation = absolute(path).generic_string();
        NFD_FreePathU8(loc);
      }
    }
  }
  ImGui::EndGroup();

  char location2[4096];
  strncpy(location, Settings::instance()->rpgMakerMZLocation.c_str(), 4096);
  if (ImGui::LabelOverLineEdit("##rpgmaker_location_line_edit", "RPG Maker MZ Location(*)", location2, sizeof(location2), 0.f)) {
    Settings::instance()->rpgMakerMZLocation = location;
  }
  ImGui::SameLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().FramePadding.y);
  ImGui::BeginGroup();
  {
    ImGui::NewLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y);
    if (ImGui::Button(std::format("{}##rpgmakermz", trNOOP("Choose...")).c_str())) {
      nfdu8char_t* loc;
      const auto result = NFD_PickFolder(&loc, !Settings::instance()->rpgMakerMZLocation.empty() ? Settings::instance()->rpgMakerMZLocation.c_str() : nullptr);
      if (result == NFD_OKAY) {
        const std::filesystem::path path{loc};
        Settings::instance()->rpgMakerMZLocation = absolute(path).generic_string();
        NFD_FreePathU8(loc);
      }
    }
  }
  ImGui::EndGroup();
  ImGui::BeginGroup();
  {
    ImGui::Text("RPG Maker MV Version");
    if (ImGui::BeginCombo("##rpgmakermv_version_combo", Settings::instance()->rpgMakerMVVersion != -1 ? KnownRPGMVVersions[Settings::instance()->rpgMakerMVVersion].data() : "Latest")) {
      for (int i = 0; i < KnownRPGMVVersions.size(); ++i) {
        if (ImGui::Selectable("Latest", Settings::instance()->rpgMakerMVVersion == -1)) {
          Settings::instance()->rpgMakerMVVersion = -1;
        }
        if (ImGui::Selectable(KnownRPGMVVersions[i].data(), i == Settings::instance()->rpgMakerMVVersion)) {
          Settings::instance()->rpgMakerMVVersion = i;
        }
        if (Settings::instance()->rpgMakerMVVersion == i) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    ImGui::Text("RPG Maker MZ Version");
    if (ImGui::BeginCombo("##rpgmakermz_version_combo", Settings::instance()->rpgMakerMZVersion != -1 ? KnownRPGMZVersions[Settings::instance()->rpgMakerMZVersion].data() : "Latest")) {
      if (ImGui::Selectable("Latest", Settings::instance()->rpgMakerMZVersion == -1)) {
        Settings::instance()->rpgMakerMZVersion = -1;
      }
      for (int i = 0; i < KnownRPGMZVersions.size(); ++i) {
        if (ImGui::Selectable(KnownRPGMZVersions[i].data(), i == Settings::instance()->rpgMakerMZVersion)) {
          Settings::instance()->rpgMakerMZVersion = i;
        }
        if (Settings::instance()->rpgMakerMZVersion == i) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
  }
  ImGui::EndGroup();
  ImGui::EndChild();

  m_allRequiredFieldsAreSet = std::filesystem::is_directory(Settings::instance()->rpgMakerMVLocation) || std::filesystem::is_directory(Settings::instance()->rpgMakerMZLocation);
}
