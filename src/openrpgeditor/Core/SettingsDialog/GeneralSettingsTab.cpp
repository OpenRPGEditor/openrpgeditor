#include "Core/SettingsDialog/GeneralSettingsTab.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Settings.hpp"

#include "Core/Application.hpp"

#include <imgui.h>
#include <SDL3/SDL_dialog.h>

#if defined(_WIN32) || defined(_WIN64)
#include <string.h>
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#else
#include <strings.h>
#endif

void GeneralSettingsTab::draw() {
  GroupBox experimentalFeatures(trNOOP("Debug & Experimental Features"), "##experimental_features", {-1, 0});
  if (experimentalFeatures.begin()) {
    // TL-NOTE: "I Know What I'm Doing" should *not* be localized
    GroupBox experimentalFeaturesConfirmation(trNOOP("Type \"I Know What I'm Doing\" to enable features."), "##experimental_features_confirmation", {-1, 0});
    if (experimentalFeaturesConfirmation.begin()) {
      ImGui::SetNextItemWidth(-1);
      ImGui::InputText("##experimental_features_confirmation_input", m_confirmationText, sizeof(m_confirmationText));
    }
    experimentalFeaturesConfirmation.end();
    const bool confirmed = !!strncasecmp(m_confirmationText, "i know what i'm doing", sizeof(m_confirmationText));
    ImGui::BeginDisabled(confirmed && !Settings::instance()->enableExperimentalFeatures);
    ImGui::Checkbox(trNOOP("Experimental"), &Settings::instance()->enableExperimentalFeatures);
    ImGui::EndDisabled();
    ImGui::BeginDisabled(confirmed && !Settings::instance()->enableDebugFeatures);
    ImGui::Checkbox(trNOOP("Debug"), &Settings::instance()->enableDebugFeatures);
    ImGui::EndDisabled();
  }
  experimentalFeatures.end();
  GroupBox mruGroupBox(trNOOP("Max Recent Projects"), "##mru_group_box", {-1, 0});
  if (mruGroupBox.begin()) {
    ImGui::SetNextItemWidth(-1);
    ImGui::SliderInt("##mru_max", &Settings::instance()->maxMru, 1, 20);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
      Settings::instance()->maxMru = std::clamp(Settings::instance()->maxMru, 1, 20);
      emit_signal(onValueChanged);
    }
  }
  mruGroupBox.end();
  // TODO: Undo/Redo stack settings

  GroupBox baseDirectoryGroupBox(trNOOP("Project Base Directory Location"), "##project_location_line_edit", {-1, 0});
  if (baseDirectoryGroupBox.begin()) {
    ImGui::BeginHorizontal("##general_settings_project_base_location_layout", {-1, 0}, 0);
    {
      char location[4096];
      strncpy(location, Settings::instance()->projectBaseDirectory.c_str(), 4096);
      const auto chooseText = trNOOP("Choose...");
      ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.75f);
      ImGui::InputText("##project_location_line_edit", location, sizeof(location), 0.f);
      if (ImGui::IsItemDeactivatedAfterEdit()) {
        Settings::instance()->projectBaseDirectory = location;
        emit_signal(onValueChanged);
      }
      ImGui::SetItemTooltip("%s", trNOOP("Sets the directory where projects are stored"));
      if (ImGui::Button(chooseText, {-1, 0})) {
        SDL_ShowOpenFolderDialog(
            [](void* userdata, const char* const* fileList, int filter) {
              if (!fileList || !*fileList || filter >= 1) {
                return;
              }
              const std::filesystem::path path{fileList[0]};
              Settings::instance()->projectBaseDirectory = absolute(path).generic_string();
              emit_signal(static_cast<ISettingsTab*>(userdata)->onValueChanged);
            },
            this, App::APP->getWindow()->getNativeWindow(), !Settings::instance()->projectBaseDirectory.empty() ? Settings::instance()->projectBaseDirectory.c_str() : nullptr, false);
      }
      ImGui::SetItemTooltip("%s", trNOOP("Select a directory to store RPG Maker game projects"));
    }
    ImGui::EndHorizontal();
  }
  baseDirectoryGroupBox.end();
}
