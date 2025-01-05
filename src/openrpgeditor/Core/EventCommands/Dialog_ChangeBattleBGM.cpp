#include "Core/EventCommands/Dialog_ChangeBattleBGM.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Log.hpp"
#include "imgui.h"
using namespace std::string_view_literals;
std::tuple<bool, bool> Dialog_ChangeBattleBGM::draw() {

  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{610, 380}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize)) {
    // Audio List
    ImVec2 tablePos = ImGui::GetCursorPos();

    ImGui::BeginGroup();
    {
      if (ImGui::BeginTable("##changebattlebgm_audio_contents", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY, ImVec2{500, 500})) {

        ImGui::TableSetupScrollFreeze(1, 0);
        ImGui::TableSetupColumn("File");
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        if (ImGui::SelectableWithBorder("None", m_selected == 0, ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
          m_selected = 0;
          m_audio.setName("");
          ImGui::SetItemDefaultFocus();
        }

        for (int n = 0; n < m_audios.size(); n++) {
          ImGui::TableNextColumn();
          const bool isSelected = (m_selected == n + 1);
          if (ImGui::SelectableWithBorder(m_audios.at(n).c_str(), isSelected, ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              playAudio(m_audios.at(m_selected - 1));
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
          playAudio(m_audios.at(m_selected - 1));
      }
      if (ImGui::Button("Stop", ImVec2(100, 0))) {
        stopAudio();
      }

      ImGui::SeparatorText("Volume");
      ImGui::SetNextItemWidth(100);
      int volume = m_audio.volume();
      if (ImGui::DragInt("##changebattlebgm_audio.volume", &volume, 0.5f, 0, 100)) {
        m_audio.setVolume(volume);
        setVolume(m_audio.volume());
      }
      ImGui::SeparatorText("Pitch");
      ImGui::SetNextItemWidth(100);
      int pitch = m_audio.pitch();
      if (ImGui::DragInt("##changebattlebgm_audio.pitch", &pitch, 0.5f, 0, 100)) {
        m_audio.setPitch(pitch);
        setPitch(m_audio.pitch());
      }
      ImGui::SeparatorText("Pan");
      ImGui::SetNextItemWidth(100);
      int pan = m_audio.pan();
      if (ImGui::DragInt("##changebattlebgm_audio.pan", &pan, 0.5f, -100, 100)) {
        m_audio.setPan(pan);
        setPanning(m_audio.pan());
      }
    }
    ImGui::EndGroup();

    // ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - 80, ImGui::GetContentRegionMax().y - 20));
    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->bgm = m_audio;
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      setOpen(false);
    }
    ImGui::EndPopup();
  }
  return std::make_tuple(!m_open, m_confirmed);
}
