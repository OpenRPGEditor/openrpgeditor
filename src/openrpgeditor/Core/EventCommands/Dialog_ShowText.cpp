#include "Core/EventCommands/Dialog_ShowText.hpp"

#include "Core/DPIHandler.hpp"
#include "Core/Log.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ShowText::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{551, 260} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    const auto buttonSize = ImVec2{144, 144} * App::DPIHandler::get_ui_scale();
    const auto buttonCenter = (buttonSize / 2);
    ImGui::BeginGroup();
    {
      ImGui::Text("Face:");
      auto cursorPos = ImGui::GetCursorPos();
      if (ImGui::ImageButton("##svbattler_image", m_buttonBack, ImVec2{80.f, 102.f} * App::DPIHandler::get_ui_scale())) {
        m_characterPicker.setCharacterInfo(m_faceImage, m_faceIndex);
        m_characterPicker.setOpen(true);
      }
      if (m_faceSheet && m_faceSheet->texture()) {
        const auto faceRect = ImVec2{static_cast<float>(m_faceSheet->faceWidth()), static_cast<float>(m_faceSheet->faceHeight())} * App::DPIHandler::get_ui_scale();
        ImGui::SetCursorPos(((cursorPos + buttonCenter) - (faceRect / 2)) + (ImGui::GetStyle().ItemInnerSpacing - ImVec2{0.f, App::DPIHandler::scale_value(1.f)}));
        const auto rect = m_faceSheet->getFaceRect(m_faceIndex);
        ImVec2 uv0{rect.u0, rect.v0};
        ImVec2 uv1{rect.u1, rect.v1};
        ImGui::Image(m_faceSheet->texture(), faceRect, uv0, uv1);
      }
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::Text("Text:");
      ImGui::InputTextMultiline("##showtext_multiline", m_textLine, 4096, ImVec2{437.f, 107.f});
      ImGui::EndGroup();
    }
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + App::DPIHandler::scale_value(100.f));
    ImGui::BeginGroup();
    {
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
    ImGui::BeginGroup();
    {
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
      ImGui::SameLine();
      if (ImGui::Button("Preview...", ImVec2{App::DPIHandler::scale_value(100), 0})) {
        // TODO
      }
      ImGui::EndGroup();
    }
    // ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - 78, ImGui::GetCursorPosY()));
    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->faceImage = m_faceImage;
      command->faceIndex = m_faceIndex;
      command->background = static_cast<TextBackground>(m_background);
      command->position = static_cast<TextWindowPosition>(m_position);
      command->textLine = m_textLine;
      std::vector<std::string> split = splitString(m_textLine, '\n');
      int index{0};
      for (auto str : split) {
        if (split.size() > 4) {
          if (textIndex == 0) {
            moreCommands.push_back(command);
          }
          moreCommands.back()->text.push_back(std::make_shared<NextTextCommand>());
          moreCommands.back()->text.back()->text = str;
          textIndex++;
          if (moreCommands.back()->text.size() > 3 && textIndex < split.size()) {
            moreCommands.push_back(std::make_shared<ShowTextCommand>());
            moreCommands.back()->faceImage = m_faceImage;
            moreCommands.back()->faceIndex = m_faceIndex;
            moreCommands.back()->background = static_cast<TextBackground>(m_background);
            moreCommands.back()->position = static_cast<TextWindowPosition>(m_position);
          }
        } else {
          if (index == 0) {
            command->text.clear();
          }

          command->text.push_back(std::make_shared<NextTextCommand>());
          command->text.back()->text = str;
        }
        index++;
      }
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }
    ImGui::EndPopup();
  }
  return std::make_tuple(!m_open, m_confirmed);
}
