#include "Core/CommonUI/SoundPicker.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include <IconsFontAwesome6.h>

bool SoundPicker::draw() {
  if (m_open) {
    ImGui::OpenPopup("Audio");
  }

  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{610, 380}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal("Audio", &m_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginGroup();
    {
      // Audio List
      if (ImGui::BeginTable("##se_audio_contents", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY, ImVec2{500, 500})) {

        ImGui::TableSetupScrollFreeze(1, 0);
        ImGui::TableSetupColumn("File");
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        ImGui::BeginDisabled(m_audioDir.value().isParentDirectory());
        if (ImGui::Selectable("\u21B0 ..", false, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SelectOnClick)) {
          if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
            m_audioDir.value().moveUp();
            m_audios = m_audioDir->getDirectoryContents();
            m_folders = m_audioDir.value().getDirectories();
            m_selectedFolder = -1;
          }
        }
        ImGui::EndDisabled();
        for (int i = 0; i < m_folders.size(); ++i) {
          const auto& folderName = std::format("{} {}", ICON_FA_FOLDER_OPEN, m_folders[i]);
          if (ImGui::Selectable(folderName.c_str(), m_selectedFolder == i, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SelectOnClick)) {
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
          if (ImGui::SelectableWithBorder(std::format("{}##{}", m_audios.at(n), n).c_str(), isSelected,
                                          ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {

            m_selected = n + 1;
            m_audio.setName(m_audioDir.value().isParentDirectory() ? m_audios.at(m_selected - 1) : m_audioDir.value().pathPrefix + '/' + m_audios.at(m_selected - 1));
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              playAudio(m_audio.name().c_str());
            }
            if (isSelected)
              ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndTable();
      }
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      if (ImGui::Button("Play", ImVec2(100, 0))) {
        if (m_selected > 0)
          playAudio(m_audio.name().c_str());
      }
      if (ImGui::Button("Stop", ImVec2(100, 0))) {
        stopAudio();
      }

      ImGui::SeparatorText("Volume");
      ImGui::SetNextItemWidth(100);
      int volume = m_audio.volume();
      if (ImGui::DragInt("##playse_audio.volume", &volume, 0.5f, 0, 100)) {
        m_audio.setVolume(volume);
        setVolume(m_audio.volume());
      }
      ImGui::SeparatorText("Pitch");
      ImGui::SetNextItemWidth(100);
      int pitch = m_audio.pitch();
      if (ImGui::DragInt("##playse_audio.pitch", &pitch, 0.5f, 0, 100)) {
        m_audio.setPitch(pitch);
        setPitch(m_audio.pitch());
      }
      ImGui::SeparatorText("Pan");
      ImGui::SetNextItemWidth(100);
      int pan = m_audio.pan();
      if (ImGui::DragInt("##playse_audio.pan", &pan, 0.5f, -100, 100)) {
        m_audio.setPan(pan);
        setPanning(m_audio.pan());
      }
    }

    ImGui::EndGroup();
    if (ImGui::Button("OK")) {
      m_toneInvalid = m_modified = true;
      m_open = false;
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      m_open = false;
    }
    ImGui::EndPopup();
  }
  return m_modified;
}
