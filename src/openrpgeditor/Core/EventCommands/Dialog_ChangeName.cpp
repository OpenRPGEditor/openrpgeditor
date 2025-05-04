#include "Core/EventCommands/Dialog_ChangeName.hpp"

#include "Database/Database.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_ChangeName::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ChangeName");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 7) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###ChangeName", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();

    ImGui::BeginVertical("##change_name_main_layout", ImGui::GetContentRegionAvail());
    {
      GroupBox actorGroupBox(trNOOP("Actor"), "##change_name_actor_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (actorGroupBox.begin()) {
        // Actor Button
        ImGui::PushID("##name_selection_actor");
        if (ImGui::Button(Database::instance()->actorNameAndId(m_actor).c_str(), ImVec2{-1, 0})) {
          m_actorPicker = ObjectPicker(trNOOP("Actor"), Database::instance()->actors.actorList(), m_actor);
          m_actorPicker->setOpen(true);
        }
        ImGui::PopID();
      }
      actorGroupBox.end();

      GroupBox nameGroupBox(trNOOP("Name"), "##change_name_name_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (nameGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);
        ImGui::InputText("##name_input", &m_actorName);
      }
      nameGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5));
      ImGui::BeginHorizontal("##change_name_buttons", {-1, 0});
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##change_name_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->actor = m_actor;
          m_command->name = m_actorName;
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

void Dialog_ChangeName::drawPickers() {
  if (m_actorPicker) {
    if (const auto [closed, confirmed] = m_actorPicker->draw(); closed) {
      if (confirmed) {
        m_actor = m_actorPicker->selection();
      }
      m_actorPicker.reset();
    }
  }
}