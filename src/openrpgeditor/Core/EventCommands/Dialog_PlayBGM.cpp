#include "Dialog_PlayBGM.hpp"
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/ImGuiUtils.hpp"
#include "Core/Log.hpp"
#include "Core/Project.hpp"
using namespace std::string_view_literals;
std::tuple<bool, bool> Dialog_PlayBGM::draw() {

  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.f));
  ImGui::SetNextWindowSize(ImVec2{610, 380} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
    // Audio List
    ImVec2 tablePos = ImGui::GetCursorPos();
    if (ImGui::BeginTable("##bgm_audio_contents", 1,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX |
                              ImGuiTableFlags_ScrollY,
                          ImVec2{(ImGui::GetContentRegionMax().x / 2)+ 175, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(16)})) {

      ImGui::TableSetupScrollFreeze(1, 0);
      ImGui::TableSetupColumn("File");
      const int totalPadding = static_cast<int>(std::floor(std::log10(m_audios.size())));
      ImGui::TableNextRow();
      for (int n = 0; n < m_audios.size(); n++) {
        ImGui::TableNextColumn();
        const bool isSelected = (m_selected == n);
        if (ImGui::SelectableWithBorder(m_audios.at(n).c_str(),
                                          isSelected,
                                          ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns |
                                              ImGuiSelectableFlags_AllowDoubleClick)) {
          if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
            playAudio((Database::Instance->basePath + "audio/bgm/" + m_audios.at(m_selected) + ".ogg").c_str());
          }
          m_selected = n;
          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndTable();
    }
    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - 100, tablePos.y + 5 - App::DPIHandler::scale_value(16)));
    if (ImGui::Button("Play", ImVec2(100, 0))) {
            playAudio((Database::Instance->basePath + "audio/bgm/" + m_audios.at(m_selected) + ".ogg").c_str());
    }
    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - 100, tablePos.y + 25 - App::DPIHandler::scale_value(16)));
    if (ImGui::Button("Stop", ImVec2(100, 0))) {
            stopAudio();
    }

    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - 100, tablePos.y + 45 - App::DPIHandler::scale_value(16)));
    ImGui::SeparatorText("Volume");
    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - 100, tablePos.y + 65 - App::DPIHandler::scale_value(16)));
    ImGui::SetNextItemWidth(100);
    if (ImGui::DragInt("##playbgm_volume", &m_volume, 0.5f, 0, 100)) {
      setVolume(m_volume);
    }
    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - 100, tablePos.y + 85 - App::DPIHandler::scale_value(16)));
    ImGui::SeparatorText("Pitch");
    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - 100, tablePos.y + 105 - App::DPIHandler::scale_value(16)));
    ImGui::SetNextItemWidth(100);
    if (ImGui::DragInt("##playbgm_pitch", &m_pitch, 0.5f, 0, 100)) {
      setPitch(m_pitch);
    }
    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - 100, tablePos.y + 125 - App::DPIHandler::scale_value(16)));
    ImGui::SeparatorText("Pan");
    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - 100, tablePos.y + 145 - App::DPIHandler::scale_value(16)));
    ImGui::SetNextItemWidth(100);
    if (ImGui::DragInt("##playbgm_pan", &m_pan, 0.5f, -100, 100)) {
      setPanning(m_pan);
    }
  }

  ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - 80, ImGui::GetContentRegionMax().y - 20));
  if (ImGui::Button("OK")) {
    command->audio.name = m_audios.at(m_selected);
    command->audio.volume = m_volume;
    command->audio.pitch = m_pitch;
    command->audio.pan = m_pan;
    m_confirmed = true;
    ImGui::CloseCurrentPopup();
    SetOpen(false);
  }
  ImGui::SameLine();
  if (ImGui::Button("Cancel")) {
    SetOpen(false);
  }
  ImGui::EndPopup();
  return std::make_tuple(!m_open, m_confirmed);
}
