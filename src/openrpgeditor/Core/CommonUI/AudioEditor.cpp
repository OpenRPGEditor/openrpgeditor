#include "Core/CommonUI/AudioEditor.hpp"

#include "Core/ImGuiExt/imgui-knobs.h"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Log.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include <IconsFontAwesome6.h>
using namespace std::string_view_literals;
bool AudioEditor::draw() {
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 2) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  // Audio List

  ImGui::BeginHorizontal("##audioRenderer_layout", ImGui::GetContentRegionAvail());
  {

    if (ImGui::BeginTable("##bgm_audio_contents", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY, ImVec2{500, 550})) {

      ImGui::TableSetupScrollFreeze(1, 0);
      ImGui::TableSetupColumn("File");
      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      ImGui::BeginDisabled(m_audioDir.value().isParentDirectory());
      if (ImGui::Selectable("\u21B0 ..", false, static_cast<int>(ImGuiSelectableFlags_SelectOnNav) | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SelectOnClick)) {
        if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
          m_audioDir.value().moveUp();
          m_audios = m_audioDir->getDirectoryContents();
          m_folders = m_audioDir.value().getDirectories();
          m_selected = 0;
          m_selectedFolder = -1;
        }
      }
      ImGui::EndDisabled();
      for (int i = 0; i < m_folders.size(); ++i) {
        const auto& folderName = std::format("{} {}", ICON_FA_FOLDER_OPEN, m_folders[i]);
        if (ImGui::Selectable(folderName.c_str(), m_selectedFolder == i,
                              static_cast<int>(ImGuiSelectableFlags_SelectOnNav) | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SelectOnClick)) {
          m_selectedFolder = i;
        }
        if (m_selectedFolder == i && ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
          m_audioDir->setDirectory(i);
          m_folders = m_audioDir.value().getDirectories();
          m_audios = m_audioDir.value().getDirectoryContents();
          m_selected = 0;
          m_selectedFolder = -1;
        }
      }

      if (ImGui::SelectableWithBorder("None", m_selected == 0, ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
        m_selected = 0;
        m_audio.setName("");
      }

      for (int n = 0; n < m_audios.size(); n++) {
        ImGui::TableNextColumn();
        const bool isSelected = (m_selected == n + 1);
        if (ImGui::SelectableWithBorder(m_audios.at(n).c_str(), isSelected, ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
          m_selected = n + 1;
          m_audio.setName(m_audioDir.value().isParentDirectory() ? m_audios.at(m_selected - 1) : m_audioDir.value().getPathPrefix() + '/' + m_audios.at(m_selected - 1));
          if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
            playAudio(m_audio.name().c_str());
          }
          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndTable();
    }
    ImGui::Spring();
    ImGui::BeginVertical("##audioRenderer_buttons", {-1, -1});
    {
      if (ImGui::Button("Play", ImVec2(100, 0))) {
        if (m_selected > 0)
          playAudio(m_audio.name().c_str());
      }
      if (ImGui::Button("Stop", ImVec2(100, 0))) {
        stopAudio();
      }

      ImGui::Separator();
      int volume = m_audio.volume();
      if (ImGuiKnobs::KnobInt("Volume", &volume, 0, 100, 1, "%d%", ImGuiKnobVariant_WiperDot, 100.f, ImGuiKnobFlags_AlwaysClamp)) {
        m_audio.setVolume(volume);
        setVolume(m_audio.volume());
      }
      ImGui::Separator();
      int pitch = m_audio.pitch();
      if (ImGuiKnobs::KnobInt("Pitch", &pitch, 50, 150, 1, "%d%", ImGuiKnobVariant_WiperDot, 100.f, ImGuiKnobFlags_AlwaysClamp)) {
        m_audio.setPitch(pitch);
        setPitch(m_audio.pitch());
      }
      ImGui::Separator();
      int pan = m_audio.pan();
      if (ImGuiKnobs::KnobInt("Pan", &pan, -100, 100, 1, "%d%", ImGuiKnobVariant_WiperDot, 100.f, ImGuiKnobFlags_AlwaysClamp)) {
        m_audio.setPan(pan);
        setPanning(m_audio.pan());
      }
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5));
      ImGui::BeginHorizontal("##audioRenderer_confirm", {-1, -1});
      {
        if (const auto ret = ImGui::ButtonGroup("##change_hp_dialog_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
        } else if (ret == 1) {
          m_confirmed = false;
        }
      }
      ImGui::EndHorizontal();
      ImGui::EndVertical();
    }
  }
  ImGui::EndHorizontal();
  return m_confirmed;
}
