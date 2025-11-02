#include "Editor/EventCommands/Dialog_NameInputProcessing.hpp"

#include "Database/Database.hpp"
#include "Editor/CommonUI/GroupBox.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <tuple>

void Dialog_NameInputProcessing::drawPickers() {
  if (m_actorPicker) {
    if (const auto [closed, confirmed] = m_actorPicker->draw(); closed) {
      if (confirmed) {
        m_actor = m_actorPicker->selection();
      }
      m_actorPicker.reset();
    }
  }
}
std::tuple<bool, bool> Dialog_NameInputProcessing::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###NameInputProcessing");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 30) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 7) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###NameInputProcessing", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();

    ImGui::BeginVertical("##name_input_processing_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox actorGroupBox(trNOOP("Actor"), "##name_input_processing_actor_group", {-1, 0});
      if (actorGroupBox.begin()) {
        if (ImGui::EllipsesButton(Database::instance()->actorNameAndId(m_actor).c_str(), ImVec2{-1, 0})) {
          m_actorPicker = ActorPicker( Database::instance()->actors->actorList(), m_actor);
          m_actorPicker->setOpen(true);
        }
      }
      actorGroupBox.end();
      GroupBox maxCharGroupBox(trNOOP("Max characters"), "##name_input_processing_max_char_group", {-1, 0});
      if (maxCharGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);
        if (ImGui::SpinInt("##name_input_processing_max_chars", &m_maxChar, 1, 1)) {
          m_maxChar = std::clamp(m_maxChar, 1, 16);
        }
      }
      maxCharGroupBox.end();

      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##name_input_processing_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##name_input_processing_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->actorId = m_actor;
          m_command->maxChar = m_maxChar;
          ImGui::CloseCurrentPopup();
          setOpen(false);
        } else if (ret == 1) {
          ImGui::CloseCurrentPopup();
          setOpen(false);
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();

    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
