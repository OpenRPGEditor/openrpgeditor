#include "Dialog_ChangeDefeatME.hpp"
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/ImGuiUtils.hpp"
using namespace std::string_view_literals;
std::tuple<bool, bool> Dialog_ChangeDefeatME::draw() {

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
                          ImVec2{(ImGui::GetContentRegionMax().x / 2) + 175,
                                 ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(16)})) {

      ImGui::TableSetupScrollFreeze(1, 0);
      ImGui::TableSetupColumn("File");
      ImGui::TableNextRow();
      for (int n = 0; n < m_audios.size(); n++) {
        ImGui::TableNextColumn();
        const bool isSelected = (m_selected == n);
        if (ImGui::SelectableWithBorder(m_audios.at(n).c_str(), isSelected,
                                        ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns |
                                            ImGuiSelectableFlags_AllowDoubleClick)) {
          if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
            playAudio((Database::Instance->basePath + "audio/bgm/" + m_audios.at(m_selected) + ".ogg").c_str());
          }
          m_selected = n;
          m_audio.name = m_audios.at(m_selected);
          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndTable();
    }

    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      if (ImGui::Button("Play", ImVec2(100, 0))) {
        playAudio((Database::Instance->basePath + "audio/bgm/" + m_audios.at(m_selected) + ".ogg").c_str());
      }
      if (ImGui::Button("Stop", ImVec2(100, 0))) {
        stopAudio();
      }

      ImGui::SeparatorText("Volume");
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
      if (ImGui::DragInt("##playbgm_audio.volume", &m_audio.volume, 0.5f, 0, 100)) {
        setVolume(m_audio.volume);
      }
      ImGui::SeparatorText("Pitch");
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
      if (ImGui::DragInt("##playbgm_audio.pitch", &m_audio.pitch, 0.5f, 0, 100)) {
        setPitch(m_audio.pitch);
      }
      ImGui::SeparatorText("Pan");
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
      if (ImGui::DragInt("##playbgm_audio.pan", &m_audio.pan, 0.5f, -100, 100)) {
        setPanning(m_audio.pan);
      }
    }

    ImGui::EndGroup();

    ImGui::BeginGroup();
    {
      ImGui::SetCursorPos(ImVec2(
          (ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(90)) - ImGui::GetStyle().FramePadding.x,
          (ImGui::GetContentRegionMax().y - App::DPIHandler::scale_value(30)) - ImGui::GetStyle().FramePadding.y));

      if (ImGui::Button("OK")) {
        m_confirmed = true;
        command->me = m_audio;
        ImGui::CloseCurrentPopup();
        SetOpen(false);
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        SetOpen(false);
      }
    }
    ImGui::EndGroup();

    ImGui::EndPopup();
  }
  return std::make_tuple(!m_open, m_confirmed);
}
