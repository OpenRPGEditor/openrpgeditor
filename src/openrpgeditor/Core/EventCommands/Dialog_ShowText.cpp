#include "Core/EventCommands/Dialog_ShowText.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Log.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"

#include <iostream>
#include <tuple>

std::tuple<bool, bool> Dialog_ShowText::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  const auto imageSize = ImGui::GetDPIScaledSize(FaceSheet::faceWidth(), FaceSheet::faceHeight());
  m_faceButtonTexture.setSize(imageSize.x, imageSize.y);

  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto minimumWidth = ImGui::CalcTextSize("#").x * 60;
  const auto minimumWindowSize = ImVec2{imageSize.x + minimumWidth + (ImGui::GetStyle().FramePadding.x * 2), imageSize.y + (ImGui::GetTextLineHeight() * 9) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSizeConstraints(minimumWindowSize, {FLT_MAX, FLT_MAX});
  ImGui::SetNextWindowSize(minimumWindowSize, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar)) {
    drawPickers();
    if (!m_faceSheet && !m_faceImage.empty()) {
      m_faceSheet.emplace(m_faceImage);
      auto offset = m_faceSheet->getFaceRect(m_faceIndex).min;
      offset.xr() *= m_faceSheet->texture().width();
      offset.yr() *= m_faceSheet->texture().height();
      m_faceButtonTexture.setTexturesToComposite({
          {
              m_faceSheet->texture(),
              Point{m_faceSheet->faceWidth(), m_faceSheet->faceHeight()},
              offset.toPoint(),
              true,
          },
      });
    }
    ImGui::BeginVertical("##show_text_layout", ImGui::GetContentRegionAvail(), 0);
    {
      ImGui::BeginHorizontal("##show_text_layout_upper", {-1, -1}, 0);
      {
        GroupBox faceGroupBox(trNOOP("Face"), "##face_group");
        if (faceGroupBox.begin()) {
          if (ImGui::ImageButton("##face_button", m_faceButtonTexture.get(), imageSize)) {
            m_facePicker.emplace(m_faceImage, m_faceIndex);
            m_facePicker->setOpen(true);
          }
        }
        faceGroupBox.end();

        ImGui::BeginVertical("##show_text_layout_text", {-1, -1}, 0);
        {
          GroupBox textGroupBox(trNOOP("Text"), "##text_group", {-1, 0});
          if (textGroupBox.begin()) {
            ImGui::InputTextMultiline("##show_text_multiline", &m_textLine, ImVec2{ImGui::GetContentRegionAvail().x, (ImGui::GetTextLineHeight() * 4) + (ImGui::GetStyle().FramePadding.y * 2)});
          }
          textGroupBox.end();
          ImGui::BeginHorizontal("##show_text_window_settings", {-1, -1}, 0);
          {
            GroupBox backgroundGroup(trNOOP("Background"), "##show_text_background", {ImGui::GetContentRegionAvail().x * .5f, 0});
            if (backgroundGroup.begin()) {
              ImGui::SetNextItemWidth(-1);
              if (ImGui::BeginCombo("##showtext_background", DecodeEnumName(m_background).c_str())) {
                for (auto& bg : magic_enum::enum_values<TextBackground>()) {
                  if (ImGui::Selectable(DecodeEnumName(bg).c_str(), m_background == bg)) {
                    m_background = bg;
                    if (m_background == bg)
                      ImGui::SetItemDefaultFocus();
                  }
                }
                ImGui::EndCombo();
              }
            }
            backgroundGroup.end();
            GroupBox positionGroup(trNOOP("Window Position"), "##show_text_window_position", {ImGui::GetContentRegionAvail().x, 0});
            if (positionGroup.begin()) {
              ImGui::SetNextItemWidth(-1);
              if (ImGui::BeginCombo("##show_text_windowpos", DecodeEnumName(m_position).c_str())) {
                for (auto& pos : magic_enum::enum_values<TextWindowPosition>()) {
                  if (ImGui::Selectable(DecodeEnumName(pos).c_str(), m_position == pos)) {
                    m_position = pos;
                    if (m_position == pos)
                      ImGui::SetItemDefaultFocus();
                  }
                }
                ImGui::EndCombo();
              }
            }
            positionGroup.end();
          }
          ImGui::EndHorizontal();
        }
        ImGui::EndVertical();
      }
      ImGui::EndHorizontal();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##show_text_buttons", {-1, -1}, 0);
      {
        if (ImGui::Checkbox(trNOOP("Batch Entry"), &m_batchEntry)) {
          if (!m_batchEntry && std::ranges::count(m_textLine, '\n') >= 4) {
            int newlineCount = 0;
            size_t pos = 0;
            while ((pos = m_textLine.find('\n', pos)) != std::string::npos) {
              newlineCount++;
              if (newlineCount == 4) {
                m_textLine.erase(pos, m_textLine.length() - pos);
                break;
              }
              pos++;
            }
          }
        }
        ImGui::Spring();
        auto ret = ImGui::ButtonGroup("##show_text_buttons", {trNOOP("Preview..."), trNOOP("OK"), trNOOP("Cancel")}, false, {trNOOP("UNIMPLEMENTED")});
        if ((ImGui::IsKeyDown(ImGuiKey_Enter) || ImGui::IsKeyDown(ImGuiKey_KeypadEnter)) && !m_batchEntry && std::ranges::count(m_textLine, '\n') >= 4) {
          ret = 1;
        }
        if (ret == 0) {
          // TODO: Implement text preview
        } else if (ret == 1) {
          // OK
          m_confirmed = true;
          m_command->faceImage = m_faceImage;
          m_command->faceIndex = m_faceIndex;
          m_command->background = m_background;
          m_command->position = m_position;
          m_command->textLine = m_textLine;
          auto split = splitString(m_textLine, '\n');
          while (split.size() > 1 && split[split.size() - 1].empty()) {
            split.pop_back();
          }
          int index{0};
          int textIndex{0};
          for (const auto& str : split) {
            if (split.size() > 4) {
              if (textIndex == 0) {
                m_moreCommands.push_back(m_command);
              }
              m_moreCommands.back()->text.push_back(std::make_shared<NextTextCommand>());
              m_moreCommands.back()->text.back()->text = str;
              textIndex++;
              if (m_moreCommands.back()->text.size() >= 4 && textIndex < split.size()) {
                m_moreCommands.push_back(std::make_shared<ShowTextCommand>());
                m_moreCommands.back()->faceImage = m_faceImage;
                m_moreCommands.back()->faceIndex = m_faceIndex;
                m_moreCommands.back()->background = m_background;
                m_moreCommands.back()->position = m_position;
              }
            } else {
              if (index == 0) {
                m_command->text.clear();
              }

              m_command->text.push_back(std::make_shared<NextTextCommand>());
              m_command->text.back()->text = str;
            }
            index++;
          }
          ImGui::CloseCurrentPopup();
          setOpen(false);
        } else if (ret == 2) { // Cancel
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

void Dialog_ShowText::drawPickers() {
  if (m_facePicker) {
    if (const auto [closed, confirmed] = m_facePicker->draw(); closed) {
      if (confirmed) {
        m_faceImage = m_facePicker->selectedSheet();
        m_faceSheet.reset();
        m_faceIndex = m_facePicker->faceIndex();
      }
      m_facePicker.reset();
    }
  }
}
