#include "Dialog_ShowText.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Project.hpp"

std::tuple<bool, bool> Dialog_ShowText::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{551, 240} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {
    const auto buttonSize = ImVec2{144, 144} * App::DPIHandler::get_ui_scale();
    const auto buttonCenter = (buttonSize / 2);
    ImGui::BeginGroup();
    {
      ImGui::Text("Face:");
      auto cursorPos = ImGui::GetCursorPos();
      if (ImGui::ImageButton("##svbattler_image", m_buttonBack.get(),
                             ImVec2{80.f, 102.f} * App::DPIHandler::get_ui_scale())) {
        m_characterPicker.setCharacterInfo(m_faceImage, m_faceIndex);
        m_characterPicker.SetOpen(true);
      }
      if (m_faceSheet && m_faceSheet->texture()) {
        const auto faceRect = ImVec2{static_cast<float>(m_faceSheet->faceWidth()),
                                     static_cast<float>(m_faceSheet->faceHeight())} *
                              App::DPIHandler::get_ui_scale();
        ImGui::SetCursorPos(
            ((cursorPos + buttonCenter) - (faceRect / 2)) +
            (ImGui::GetStyle().ItemInnerSpacing - ImVec2{0.f, App::DPIHandler::scale_value(1.f)}));
        const auto rect = m_faceSheet->getFaceRect(m_faceIndex);
        ImVec2 uv0{rect.u0, rect.v0};
        ImVec2 uv1{rect.u1, rect.v1};
        ImGui::Image(m_faceSheet->texture().get(), faceRect, uv0, uv1);
      }
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup(); {
      ImGui::Text("Text:");
      char note[8192];
      strncpy(note, m_textLine.c_str(), IM_ARRAYSIZE(note));
      if (ImGui::InputTextMultiline("##showtext_multiline", note, IM_ARRAYSIZE(note), ImVec2{437.f, 107.f}))
      {
        m_textLine = note;
        if (splitString(m_textLine, '\n').size() > 1) {
          m_textLine = "";
        }
      };

      ImGui::EndGroup();
    }
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + App::DPIHandler::scale_value(100.f));
    ImGui::BeginGroup(); {
      ImGui::Text("Background:");
      ImGui::PushItemWidth((App::DPIHandler::scale_value(120)));
      if (ImGui::BeginCombo("##showtext_background", DecodeEnumName(magic_enum::enum_value<TextBackground>(m_background)).c_str())) {
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
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - App::DPIHandler::scale_value(3));
    ImGui::BeginGroup(); {
      ImGui::Text("Window Position:");
      ImGui::PushItemWidth((App::DPIHandler::scale_value(120)));
      if (ImGui::BeginCombo("##showtext_windowpos", DecodeEnumName(magic_enum::enum_value<TextWindowPosition>(m_position)).c_str())) {
        for (auto& bg : magic_enum::enum_values<TextWindowPosition>()) {
          bool is_selected = m_background == magic_enum::enum_index(bg).value();
          if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(bg)).c_str(), is_selected)) {
            m_background = magic_enum::enum_index(bg).value();
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::SetCursorPos(ImVec2{ImGui::GetCursorPosX() + App::DPIHandler::scale_value(70), ImGui::GetCursorPosY() + App::DPIHandler::scale_value(15)});
    if (ImGui::Button("Preview...", ImVec2{App::DPIHandler::scale_value(100), 0})) {
      // TODO
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + App::DPIHandler::scale_value(20));
    ImGui::BeginGroup(); {
      ImGui::Checkbox("Batch Entry", &m_batchEntry);
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + App::DPIHandler::scale_value(380));
    ImGui::BeginGroup(); {
      if (ImGui::Button("OK")) {
        m_confirmed = true;
        if (m_batchEntry) {
          // TODO: > 4 lines = new command until all lines are done
        }
        command->faceImage = m_faceImage;
        command->faceIndex = m_faceIndex;
        command->background = static_cast<TextBackground>(m_background);
        command->position = static_cast<TextWindowPosition>(m_position);
        command->textLine = m_textLine;
        for (auto str : splitString(command->textLine, '\n')) {
            command->text.push_back(std::make_shared<NextTextCommand>());
            command->text.back()->text = str;
        }
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
