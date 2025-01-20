#include "Core/CommonUI/SoundPicker.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"

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

        if (ImGui::SelectableWithBorder("None", m_selected == 0, ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
          m_selected = 0;
          m_audio.setName("");
          if (m_selected == 0)
            ImGui::SetItemDefaultFocus();
        }

        for (int n = 0; n < m_audios.size(); n++) {
          ImGui::TableNextColumn();
          const bool isSelected = (m_selected == n + 1);
          if (ImGui::SelectableWithBorder(std::format("{}##{}", m_audios.at(n), n).c_str(), isSelected,
                                          ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              playAudio((Database::instance()->basePath + "audio/se/" + m_audios.at(m_selected - 1) + ".ogg").c_str());
            }
            m_selected = n + 1;
            m_audio.setName(m_audios.at(m_selected - 1));
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
          playAudio((Database::instance()->basePath + "audio/se/" + m_audios.at(m_selected) + ".ogg").c_str());
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
