#include "Core/EventCommands/Dialog_ShowChoice.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Log.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include <tuple>

std::tuple<bool, bool> Dialog_ShowChoice::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ShowChoice");
  }
  const auto maxSize = ImVec2{ImGui::CalcTextSize("#").x * 64 + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 12) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###ShowChoice", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginVertical("##show_choice_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox choicesGroup(trNOOP("Choices"), "##show_choice_inner_layout", {-1, 0});
      if (choicesGroup.begin()) {
        for (int i = 0; auto& choice : m_choices) {
          ImGui::BeginHorizontal(std::format("##show_choice_choice_{}", i).c_str(), {-1, 0});
          {
            ImGui::Text("#%i:", i + 1);
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText(std::format("##show_choice_choice_{}", i).c_str(), &choice);
          }
          ImGui::EndHorizontal();
          ++i;
        }
      }
      choicesGroup.end();
      ImGui::BeginHorizontal("##show_choice_configuration_layout", {-1, 0}, 0);
      {
        ImGui::Spring(0.5f);
        GroupBox windowBackgroundGroup(trNOOP("Background"), "##show_choice_window_background", {ImGui::GetContentRegionAvail().x * .25f, 0.f});
        if (windowBackgroundGroup.begin()) {
          ImGui::SetNextItemWidth(-1);
          if (ImGui::BeginCombo("##show_choice_window_position_combo", DecodeEnumName(magic_enum::enum_value<TextBackground>(m_background)).c_str())) {
            for (auto& background : magic_enum::enum_values<TextBackground>()) {
              bool selected = m_background == static_cast<int>(background);
              if (ImGui::Selectable(DecodeEnumName(background).c_str(), selected)) {
                m_background = static_cast<int>(background);
              }
              if (selected)
                ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
          }
        }
        windowBackgroundGroup.end();
        ImGui::Spring(0.333f);
        GroupBox windowPositionGroup(trNOOP("Window Position"), "##show_choice_window_position", {ImGui::GetContentRegionAvail().x * .333f, 0.f});
        if (windowPositionGroup.begin()) {
          ImGui::SetNextItemWidth(-1);
          if (ImGui::BeginCombo("##show_choice_window_position_combo", DecodeEnumName(magic_enum::enum_value<ChoiceWindowPosition>(m_position)).c_str())) {
            for (auto& position : magic_enum::enum_values<ChoiceWindowPosition>()) {
              bool selected = m_position == static_cast<int>(position);
              if (ImGui::Selectable(DecodeEnumName(position).c_str(), selected)) {
                m_position = static_cast<int>(position);
              }
              if (selected)
                ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
          }
        }
        windowPositionGroup.end();
        ImGui::Spring(0.333f);
        GroupBox defaultChoiceGroup(trNOOP("Default"), "##show_choice_default_choice", {ImGui::GetContentRegionAvail().x * .5f, 0.f});
        if (defaultChoiceGroup.begin()) {
          ImGui::SetNextItemWidth(-1);
          if (ImGui::BeginCombo("##show_choice_window_default_choice_combo", (m_defaultType < 0 ? trNOOP("None") : trFormat("Choice #{}", m_defaultType + 1)).c_str())) {
            for (int i{-1}; i < 6; i++) {
              bool selected = m_defaultType == i;
              if (i < 0) {
                if (ImGui::Selectable("None", selected)) {
                  m_defaultType = -1;
                }
              } else {
                if (ImGui::Selectable(trFormat("Choice #{}", i + 1).c_str(), selected)) {
                  m_defaultType = i;
                }
              }
              if (selected) {
                ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
        }
        defaultChoiceGroup.end();

        ImGui::Spring(0.333f);
        GroupBox cancelChoiceGroup(trNOOP("Cancel"), "##show_choice_cancel_choice", {-1, 0.f});
        if (cancelChoiceGroup.begin()) {
          ImGui::SetNextItemWidth(-1);
          if (ImGui::BeginCombo("##show_choice_cancel_choice_combo", (m_cancelType == -2   ? trNOOP("Branch")
                                                                      : m_cancelType == -1 ? trNOOP("Disallow")
                                                                                           : trFormat("Choice #{}", m_cancelType + 1))
                                                                         .c_str())) {
            for (int i{-2}; i < 6; i++) {
              bool selected = m_cancelType == i;
              if (i < 0) {
                if (i == -2) {
                  if (ImGui::Selectable(trNOOP("Branch"), selected)) {
                    m_cancelType = -2;
                  }
                } else {
                  if (ImGui::Selectable(trNOOP("Disallow"), selected)) {
                    m_cancelType = -1;
                  }
                }
              } else {
                if (ImGui::Selectable(trFormat("Choice #{}", i + 1).c_str(), selected)) {
                  m_cancelType = i;
                }
              }
              if (selected) {
                ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
        }
        cancelChoiceGroup.end();

        ImGui::Spring(0.5f);
      }
      ImGui::EndHorizontal();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##show_choice_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##show_choice_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->choices.clear();
          for (int i = 0; i < m_choices.size(); ++i) {
            if (m_choices[i].empty()) {
              continue;
            }
            m_command->choices.push_back(m_choices[i]);
          }

          if (m_command->choices.empty()) {
            m_command->choices.push_back("");
          }
          m_command->positionType = static_cast<ChoiceWindowPosition>(m_position);
          m_command->background = static_cast<TextBackground>(m_background);
          m_command->cancelType = m_cancelType;
          m_command->defaultType = m_defaultType;
          setOpen(false);
        } else if (ret == 1) {
          setOpen(false);
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
    ImGui::EndPopup();
  }

  return {!m_open, m_confirmed};
}
