#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Dialog_ShowChoice.hpp"
#include <tuple>
#include "Core/DPIHandler.hpp"
#include "Core/Log.hpp"

std::tuple<bool, bool> Dialog_ShowChoice::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{551, 290} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginGroup();
    {
      ImGui::SeparatorText("Choices");
      ImGui::BeginGroup();
      {
        ImGui::Text("#1:");
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + App::DPIHandler::scale_value(7));
        ImGui::Text("#2:");
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + App::DPIHandler::scale_value(7));
        ImGui::Text("#3:");
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + App::DPIHandler::scale_value(7));
        ImGui::Text("#4:");
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + App::DPIHandler::scale_value(7));
        ImGui::Text("#5:");
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + App::DPIHandler::scale_value(7));
        ImGui::Text("#6:");
        ImGui::EndGroup();
      }
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        ImGui::InputText("##showchoice_text_1", &m_choice_1);
        ImGui::InputText("##showchoice_text_2", &m_choice_2);
        ImGui::InputText("##showchoice_text_3", &m_choice_3);
        ImGui::InputText("##showchoice_text_4", &m_choice_4);
        ImGui::InputText("##showchoice_text_5", &m_choice_5);
        ImGui::InputText("##showchoice_text_6", &m_choice_6);
        ImGui::EndGroup();
      }
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + App::DPIHandler::scale_value(20));
    ImGui::BeginGroup();
    {
      ImGui::Text("Background:");
      ImGui::PushItemWidth((App::DPIHandler::scale_value(120)));
      if (ImGui::BeginCombo("##showchoice_background",
                            DecodeEnumName(magic_enum::enum_value<TextBackground>(m_background)).c_str())) {
        for (auto& bg : magic_enum::enum_values<TextBackground>()) {
          bool is_selected = m_background == magic_enum::enum_index(bg).value();
          if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(bg)).c_str(), is_selected)) {
            m_background = magic_enum::enum_index(bg).value();
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      ImGui::Text("Window Position:");
      ImGui::PushItemWidth((App::DPIHandler::scale_value(120)));
      if (ImGui::BeginCombo("##showchoice_windowpos",
                            DecodeEnumName(magic_enum::enum_value<ChoiceWindowPosition>(m_position)).c_str())) {
        for (auto& bg : magic_enum::enum_values<ChoiceWindowPosition>()) {
          bool is_selected = m_background == magic_enum::enum_index(bg).value();
          if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(bg)).c_str(), is_selected)) {
            m_background = magic_enum::enum_index(bg).value();
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }

      ImGui::Text("Default:");
      ImGui::PushItemWidth((App::DPIHandler::scale_value(120)));
      if (ImGui::BeginCombo("##showchoice_default",
                            m_defaultType < 0 ? "None" : ("Choice #" + std::to_string(m_defaultType + 1)).c_str())) {

        for (int i{-1}; i < 6; i++) {
          bool is_selected = m_defaultType == i;
          if (i < 0) {
            if (ImGui::Selectable("None", is_selected)) {
              m_defaultType = -1;
              if (is_selected)
                ImGui::SetItemDefaultFocus();
            }
          } else {
            if (ImGui::Selectable(("Choice #" + std::to_string(i + 1)).c_str(), is_selected)) {
              m_defaultType = i;
              if (is_selected)
                ImGui::SetItemDefaultFocus();
            }
          }
        }
        ImGui::EndCombo();
      }

      ImGui::Text("Cancel:");
      ImGui::PushItemWidth((App::DPIHandler::scale_value(120)));
      std::string text = m_cancelType == -2   ? "Branch"
                         : m_cancelType == -1 ? "Disallow"
                                              : ("Choice #" + std::to_string(m_cancelType + 1));

      if (ImGui::BeginCombo("##showchoice_cancel", text.c_str())) {
        for (int i{-2}; i < 6; i++) {
          bool is_selected = m_cancelType == i;
          if (i < 0) {
            if (i == -2) {
              if (ImGui::Selectable("Branch", is_selected)) {
                m_cancelType = -2;
                if (is_selected)
                  ImGui::SetItemDefaultFocus();
              }
            } else {
              if (ImGui::Selectable("Disallow", is_selected)) {
                m_cancelType = -1;
                if (is_selected)
                  ImGui::SetItemDefaultFocus();
              }
            }
          } else {
            if (ImGui::Selectable(("Choice #" + std::to_string(i + 1)).c_str(), is_selected)) {
              m_cancelType = i;
              if (is_selected)
                ImGui::SetItemDefaultFocus();
            }
          }
        }
        ImGui::EndCombo();
      }
      ImGui::EndGroup();
    }
    ImGui::SetCursorPos(ImVec2{ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(80),
                               ImGui::GetContentRegionMax().y - App::DPIHandler::scale_value(20)});
    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        m_confirmed = true;

        /*
        if (!m_choice_1.empty()) { m_choices[0] = m_choice_1; }
        if (!m_choice_2.empty()) { m_choices[1] = m_choice_2; }
        if (!m_choice_3.empty()) { m_choices[2] = m_choice_3; }
        if (!m_choice_4.empty()) { m_choices[3] = m_choice_4; }
        if (!m_choice_5.empty()) { m_choices[4] = m_choice_5; }
        if (!m_choice_6.empty()) { m_choices[5] = m_choice_6; }
        */
        m_choices[0] = m_choice_1;
        m_choices[1] = m_choice_2;
        m_choices[2] = m_choice_3;
        m_choices[3] = m_choice_4;
        m_choices[4] = m_choice_5;
        m_choices[5] = m_choice_6;

        command->background = static_cast<TextBackground>(m_background);
        command->positionType = static_cast<ChoiceWindowPosition>(m_position);
        // command->choices = m_choices;

        command->choices.clear();
        for (auto& str : m_choices) {
          if (!str.empty())
            command->choices.push_back(str);
        }

        /*
        // Validation
        int index{0};
        int deletionIndex{-1};
        for (auto&str : m_choices) {
          if (str.empty()) {
            APP_INFO(std::to_string(index) + " is marked for deletion");
            deletionIndex = index;
            break;
          }
          index++;
        }
        if (!m_choices.back().empty())
          deletionIndex = -1;

        if (deletionIndex > -1) {
          APP_INFO("Erasing from " + std::to_string(deletionIndex));
          command->choices.erase(command->choices.begin() + deletionIndex, command->choices.end());
        }
         */
        command->cancelType = m_cancelType;
        command->defaultType = m_defaultType;

        ImGui::CloseCurrentPopup();
        SetOpen(false);
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        ImGui::CloseCurrentPopup();
        SetOpen(false);
      }
      ImGui::EndPopup();
      ImGui::EndGroup();
    }
  }

  return std::make_tuple(!m_open, m_confirmed);
}
