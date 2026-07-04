#include "Editor/FirstBootWizard/RPGMakerLocationAndVersionPage.hpp"

#include "Editor/Application.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "Editor/Managers/SettingsManager.hpp"

#include <Database/Versions.hpp>
#include <filesystem>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <SDL3/SDL_dialog.h>

void RPGMakerLocationAndVersionPage::draw() {
  // TODO: Rewrite layout
  ImGui::BeginChild("RPGMakerLocationAndVersionPage", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiWindowFlags_AlwaysAutoResize, ImGuiWindowFlags_NoBackground);
  char location[4096];
  strncpy(location, SettingsManager::instance().getValue<std::string>("rpgMakerMVLocation", {}).c_str(), 4096);
  if (ImGui::LabelOverLineEdit("##rpgmakermv_location_line_edit", trNOOP("RPG Maker MV Location(*)"), location, sizeof(location), 0.f)) {
    SettingsManager::instance().setValue("rpgMakerMVLocation", location);
  }
  ImGui::SameLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().FramePadding.y);
  ImGui::BeginGroup();
  {
    ImGui::NewLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y);
    if (ImGui::Button(std::format("{}##rpgmakermv", trNOOP("Choose...")).c_str())) {
      const auto mvLocation = SettingsManager::instance().getValue<std::string>("rpgMakerMVLocation", {});
      SDL_ShowOpenFolderDialog(
          [](void*, const char* const* fileList, int filter) {
            if (!fileList || !*fileList || filter >= 1) {
              return;
            }
            const std::filesystem::path path{fileList[0]};
            SettingsManager::instance().setValue("rpgMakerMVLocation", absolute(path).generic_string());
          },
          nullptr, App::APP->getWindow()->getNativeWindow(), !mvLocation.empty() ? mvLocation.c_str() : nullptr, false);
    }
  }
  ImGui::EndGroup();
  ImGui::BeginGroup();
  {
    ImGui::Text("RPG Maker MV Version");
    const auto rpgMakerMVVersion = SettingsManager::instance().getValue("rpgMakerMVVersion", -1);
    if (ImGui::BeginCombo("##rpgmakermv_version_combo", rpgMakerMVVersion != -1 ? KnownRPGMVVersions[rpgMakerMVVersion].data() : "Latest")) {
      if (ImGui::Selectable("Latest", rpgMakerMVVersion == -1)) {
        SettingsManager::instance().setValue("rpgMakerMVVersion", -1);
      }
      for (int i = 0; i < KnownRPGMVVersions.size(); ++i) {
        if (ImGui::Selectable(KnownRPGMVVersions[i].data(), i == rpgMakerMVVersion)) {
          SettingsManager::instance().setValue("rpgMakerMVVersion", i);
        }
        if (rpgMakerMVVersion == i) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
  }
  ImGui::EndGroup();
  ORE_DISABLE_EXPERIMENTAL_BEGIN();
  ImGui::Separator();
  char location2[4096];
  strncpy(location2, SettingsManager::instance().getValue<std::string>("rpgMakerMZLocation", {}).c_str(), 4096);
  if (ImGui::LabelOverLineEdit("##rpgmakermz_location_line_edit", "RPG Maker MZ Location(*)", location2, sizeof(location2), 0.f)) {
    SettingsManager::instance().setValue("rpgMakerMZLocation", location2);
  }
  ImGui::SameLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().FramePadding.y);
  ImGui::BeginGroup();
  {
    ImGui::NewLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y);
    if (ImGui::Button(std::format("{}##rpgmakermz", trNOOP("Choose...")).c_str())) {
      const auto mzLocation = SettingsManager::instance().getValue<std::string>("rpgMakerMZLocation", {});
      SDL_ShowOpenFolderDialog(
          [](void*, const char* const* fileList, int filter) {
            if (!fileList || !*fileList || filter >= 1) {
              return;
            }
            const std::filesystem::path path{fileList[0]};
            SettingsManager::instance().setValue("rpgMakerMZLocation", absolute(path).generic_string());
          },
          nullptr, App::APP->getWindow()->getNativeWindow(), !mzLocation.empty() ? mzLocation.c_str() : nullptr, false);
    }
  }
  ImGui::EndGroup();
  ImGui::BeginGroup();
  {
    ImGui::Text("RPG Maker MZ Version");
    const auto rpgMakerMZVersion = SettingsManager::instance().getValue("rpgMakerMVVersion", -1);
    if (ImGui::BeginCombo("##rpgmakermz_version_combo", rpgMakerMZVersion != -1 ? KnownRPGMZVersions[rpgMakerMZVersion].data() : "Latest")) {
      if (ImGui::Selectable("Latest", rpgMakerMZVersion == -1)) {
        SettingsManager::instance().setValue("rpgMakerMZVersion", -1);
      }
      for (int i = 0; i < KnownRPGMZVersions.size(); ++i) {
        if (ImGui::Selectable(KnownRPGMZVersions[i].data(), i == rpgMakerMZVersion)) {
          SettingsManager::instance().setValue("rpgMakerMZVersion", i);
        }
        if (rpgMakerMZVersion == i) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
  }
  ImGui::EndGroup();
  ORE_DISABLE_EXPERIMENTAL_END();
  ImGui::EndChild();

  m_allRequiredFieldsAreSet = std::filesystem::is_directory(SettingsManager::instance().getValue<std::string>("rpgMakerMVLocation", {})) ||
                              std::filesystem::is_directory(SettingsManager::instance().getValue<std::string>("rpgMakerMZLocation", {}));
}
