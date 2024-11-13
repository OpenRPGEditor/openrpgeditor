#include "Core/EventCommands/MovementRoute/Dialog_MovementChangeImage.hpp"
#include <tuple>
#include "imgui.h"
#include "Core/Application.hpp"
#include "Core/Log.hpp"
#include "Core/DPIHandler.hpp"

std::tuple<bool, bool> Dialog_MovementChangeImage::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{115, 167} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open,
                             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {

    if (const auto [closed, confirmed] = m_characterPicker.draw(); closed) {
      if (confirmed) {
        m_characterPicker.Accept();
        m_character = m_characterPicker.character();
        m_image = m_characterPicker.selectedSheet();
        m_characterSheet = CharacterSheet(m_image);
      }
    }

    const auto buttonSize = ImVec2{144, 144} * App::DPIHandler::get_ui_scale();
    const auto buttonCenter = (buttonSize / 2);
    ImGui::BeginGroup();
    {
      auto cursorPos = ImGui::GetCursorPos();
      if (ImGui::ImageButton("##event_image", m_buttonBack.get(),
                             ImVec2{80.f, 102.f} * App::DPIHandler::get_ui_scale())) {
        m_characterPicker.setCharacterInfo(m_image, m_character);
        m_characterPicker.SetOpen(true);
      }
      if (m_characterSheet->texture()) {
        if (m_characterSheet->characterWidth() < 72 || m_characterSheet->characterHeight() < 96) {
          ImGui::SetCursorPos(
              cursorPos + (ImVec2{m_characterSheet->characterWidth() / 2.f, m_characterSheet->characterHeight() / 2.f} *
                           App::DPIHandler::get_ui_scale()));
        } else {
          ImGui::SetCursorPos(cursorPos);
        }
        const auto [uv0, uv1] = m_characterSheet->getRectForCharacter(m_character);
        ImGui::Image(m_characterSheet->texture().get(),
                     ImVec2{static_cast<float>(m_characterSheet->characterWidth()),
                            static_cast<float>(m_characterSheet->characterHeight())} *
                         App::DPIHandler::get_ui_scale(),
                     ImVec2{uv0.u, uv0.v}, ImVec2{uv1.u, uv1.v});
      }
    }
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.f);
    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        m_confirmed = true;
        command->image = m_image;
        command->character = m_character;
        ImGui::CloseCurrentPopup();
        SetOpen(false);
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        ImGui::CloseCurrentPopup();
        SetOpen(false);
      }
      ImGui::EndGroup();
    }

    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
