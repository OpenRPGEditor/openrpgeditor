#include "Core/EventCommands/Dialog_RecoverAll.hpp"

#include "Database/Database.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_RecoverAll::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###RecoverAll");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 6) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###RecoverAll", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();
    ImGui::BeginVertical("##recover_all_main_layout", ImGui::GetContentRegionAvail());
    {
      GroupBox actorGroupBox(trNOOP("Actor"), "##recover_all_actor_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (actorGroupBox.begin()) {
        ImGui::BeginHorizontal("##recover_all_actor_layout");
        {
          ImGui::BeginVertical("##recover_all_actor_source_layout", {-1, -1});
          {
            ImGui::RadioButton(trNOOP("Fixed"), &m_source, 0);
            ImGui::RadioButton(trNOOP("Variable"), &m_source, 1);
          }
          ImGui::EndVertical();

          ImGui::BeginVertical("##recover_all_source_layout", {-1, -1});
          {
            // Actor Button
            ImGui::BeginDisabled(m_source != 0);
            ImGui::SetNextItemWidth(-1);
            std::string text = m_source != 0 ? "##recover_all_actor_empty" : Database::instance()->actorNameAndId(m_actor);
            ImGui::PushID("##recover_all_selection_actor");
            if (ImGui::EllipsesButton(text.c_str(), ImVec2{-1, 0})) {
              m_actorPicker = ActorPicker( Database::instance()->actors->actorList(), m_actor);
              m_actorPicker->setUseDummyEntry(trNOOP("Entire Party"));
              m_actorPicker->setOpen(true);
            }
            ImGui::PopID();
            ImGui::EndDisabled();

            // Variable Button
            ImGui::BeginDisabled(m_source != 1);
            ImGui::SetNextItemWidth(100);
            text = m_source != 1 ? "##recover_all_variable_empty" : Database::instance()->variableNameAndId(m_variable);
            ImGui::PushID("##recover_all_variable");
            if (ImGui::EllipsesButton(text.c_str(), ImVec2{-1, 0})) {
              m_variablePicker.emplace(VariableSwitchPicker::Type::Variable, Database::instance()->system->variables(), m_variable);
              m_variablePicker->setOpen(true);
            }
            ImGui::PopID();
            ImGui::EndDisabled();
          }
          ImGui::EndVertical();
        }
        ImGui::EndHorizontal();
      }
      actorGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5));
      ImGui::BeginHorizontal("##recover_all_buttons", {-1, -1});
      {
        ImGui::Spring();

        if (const auto ret = ImGui::ButtonGroup("##recover_all_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->comparison = static_cast<ActorComparisonSource>(m_source);
          if (m_command->comparison == ActorComparisonSource::Fixed) {
            m_command->value = m_actor;
          } else {
            m_command->value = m_variable;
          }
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

void Dialog_RecoverAll::drawPickers() {
  if (m_variablePicker) {
    if (const auto [closed, confirmed] = m_variablePicker->draw(); closed) {
      if (confirmed) {
        m_variable = m_variablePicker->selection();
      }
      m_variablePicker.reset();
    }
  }
  if (m_actorPicker) {
    auto [closed, confirmed] = m_actorPicker->draw();
    if (closed) {
      if (confirmed) {
        m_actor = m_actorPicker->selection();
      }
      m_actorPicker.reset();
    }
  }
}