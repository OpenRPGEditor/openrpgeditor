#include "Core/SettingsDialog/GeneralSettingsTab.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Settings.hpp"

#include <imgui.h>
#include <nfd.h>

#if defined(_WIN32) || defined(_WIN64)
#include <string.h>
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#else
#include <strings.h>
#endif

void GeneralSettingsTab::draw() {
  if (ImGui::BeginTabItem(trNOOP("General"))) {
    ImGui::TextUnformatted(trNOOP("Debug & Experimental Features"));
    // TL-NOTE: "I Know What I'm Doing" should *not* be localized
    ImGui::LabelOverLineEdit("##experimental_features_confirmation", trNOOP("Type \"I Know What I'm Doing\" to enable features."), m_confirmationText, sizeof(m_confirmationText), 0.f, nullptr,
                             ImGuiInputTextFlags_None);
    const bool confirmed = !!strncasecmp(m_confirmationText, "i know what i'm doing", sizeof(m_confirmationText));
    ImGui::BeginDisabled(confirmed && !Settings::instance()->enableExperimentalFeatures);
    ImGui::Checkbox(trNOOP("Experimental"), &Settings::instance()->enableExperimentalFeatures);
    ImGui::EndDisabled();
    ImGui::BeginDisabled(confirmed && !Settings::instance()->enableDebugFeatures);
    ImGui::Checkbox(trNOOP("Debug"), &Settings::instance()->enableDebugFeatures);
    ImGui::EndDisabled();
    ImGui::Separator();
    ImGui::TextUnformatted(trNOOP("Max Recent Projects"));
    ImGui::SliderInt("##mru_max", &Settings::instance()->maxMru, 1, 20);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
      Settings::instance()->maxMru = std::clamp(Settings::instance()->maxMru, 1, 20);
      onValueChanged.fire();
    }
    // TODO: Undo/Redo stack settings

    char location[4096];
    strncpy(location, Settings::instance()->projectBaseDirectory.c_str(), 4096);
    ImGui::LabelOverLineEdit("##project_location_line_edit", trNOOP("Project Base Directory Location"), location, sizeof(location), 0.f, trNOOP("Sets the directory where projects are stored"));
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
      if (ImGui::Button(trNOOP("Choose..."))) {
        nfdu8char_t* loc;
        if (NFD_PickFolder(&loc, !Settings::instance()->projectBaseDirectory.empty() ? Settings::instance()->projectBaseDirectory.c_str() : nullptr) == NFD_OKAY) {
          const std::filesystem::path path{loc};
          Settings::instance()->projectBaseDirectory = absolute(path).generic_string();
          onValueChanged.fire();
          NFD_FreePathU8(loc);
        }
      }
      ImGui::SetItemTooltip(trNOOP("Select a directory to store RPG Maker game projects"));
    }
    ImGui::EndGroup();

    ImGui::EndTabItem();
  }
}
