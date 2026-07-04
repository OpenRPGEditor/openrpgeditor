#include "Editor/SettingsDialog/RPGMakerLocationAndVersionTab.hpp"
#include "Editor/Application.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "Editor/Managers/SettingsManager.hpp"

#include <Database/Versions.hpp>

#include "orei18n.hpp"

#include <imgui.h>
#include <SDL3/SDL_dialog.h>

void RPGMakerLocationAndVersionTab::draw() {
  char location[4096];
  strncpy(location, SettingsManager::instance().getValue<std::string>("rpgMakerMVLocation", {}).c_str(), 4096);
  if (ImGui::LabelOverLineEdit("##rpgmakermv_location_line_edit", trNOOP("RPG Maker MV Location"), location, sizeof(location), 0.f)) {
    SettingsManager::instance().setValue("rpgMakerMVLocation", location);
    emit_signal(onValueChanged);
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
          [](void* userdata, const char* const* fileList, int filter) {
            if (!fileList || !*fileList || filter >= 1) {
              return;
            }
            const std::filesystem::path path{fileList[0]};
            SettingsManager::instance().setValue("rpgMakerMVLocation", absolute(path).generic_string());
            emit_signal(static_cast<ISettingsTab*>(userdata)->onValueChanged);
          },
          this, App::APP->getWindow()->getNativeWindow(), !mvLocation.empty() ? mvLocation.c_str() : nullptr, false);
    }
  }
  ImGui::EndGroup();
  ImGui::BeginGroup();
  {
    ImGui::Text("RPG Maker MV Version");
    const auto mvVersion = SettingsManager::instance().getValue("rpgMakerMVVersion", -1);
    if (ImGui::BeginCombo("##rpgmakermv_version_combo", mvVersion != -1 ? KnownRPGMVVersions[mvVersion].data() : "Latest")) {
      if (ImGui::Selectable("Latest", mvVersion == -1)) {
        SettingsManager::instance().setValue("rpgMakerMVVersion", -1);
      }
      for (int i = 0; i < KnownRPGMVVersions.size(); ++i) {
        if (ImGui::Selectable(KnownRPGMVVersions[i].data(), i == mvVersion)) {
          SettingsManager::instance().setValue("rpgMakerMVVersion", i);
          emit_signal(onValueChanged);
        }
        if (mvVersion == i) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
  }
  ImGui::EndGroup();
  ImGui::Separator();
  char location2[4096];
  strncpy(location2, SettingsManager::instance().getValue<std::string>("rpgMakerMZLocation", {}).c_str(), 4096);
  if (ImGui::LabelOverLineEdit("##rpgmakermz_location_line_edit", "RPG Maker MZ Location", location2, sizeof(location2), 0.f)) {
    SettingsManager::instance().setValue("rpgMakerMZLocation", location2);
  }
  ImGui::SameLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().FramePadding.y);
  ImGui::BeginGroup();
  {
    ImGui::NewLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y);
    if (ImGui::Button(std::format("{}##rpgmakermz", trNOOP("Choose...")).c_str())) {
      const auto mzLocation = SettingsManager::instance().getValue<std::string>("rpgMakerMVLocation", {});
      SDL_ShowOpenFolderDialog(
          [](void* userdata, const char* const* fileList, int filter) {
            if (!fileList || !*fileList || filter >= 1) {
              return;
            }
            const std::filesystem::path path{fileList[0]};
            SettingsManager::instance().setValue("rpgMakerMZLocation", absolute(path).generic_string());
            emit_signal(static_cast<ISettingsTab*>(userdata)->onValueChanged);
          },
          this, App::APP->getWindow()->getNativeWindow(), !mzLocation.empty() ? mzLocation.c_str() : nullptr, false);
    }
  }
  ImGui::EndGroup();
  ImGui::BeginGroup();
  {
    ImGui::Text("RPG Maker MZ Version");
    const auto mzVersion = SettingsManager::instance().getValue("rpgMakerMZVersion", -1);
    if (ImGui::BeginCombo("##rpgmakermz_version_combo", mzVersion != -1 ? KnownRPGMZVersions[mzVersion].data() : "Latest")) {
      if (ImGui::Selectable("Latest", mzVersion == -1)) {
        SettingsManager::instance().setValue("rpgMakerMZVersion", -1);
      }
      for (int i = 0; i < KnownRPGMZVersions.size(); ++i) {
        if (ImGui::Selectable(KnownRPGMZVersions[i].data(), i == mzVersion)) {
          SettingsManager::instance().setValue("rpgMakerMZVersion", i);
          emit_signal(onValueChanged);
        }
        if (mzVersion == i) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
  }
  ImGui::EndGroup();
}
