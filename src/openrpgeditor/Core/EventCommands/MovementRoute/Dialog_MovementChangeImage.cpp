#include "Core/EventCommands/MovementRoute/Dialog_MovementChangeImage.hpp"
#include "Core/Application.hpp"

#include "Core/Log.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_MovementChangeImage::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{115, 167}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    if (const auto [closed, confirmed] = m_characterPicker.draw(); closed) {
      if (closed) {
        if (confirmed) {
          m_characterPicker.accept();
          m_character = m_characterPicker.character();
          m_image = m_characterPicker.selectedSheet();
          m_characterSheet = CharacterSheet(m_image);
        }
      }
    }

    const auto buttonSize = ImVec2{144, 144};
    const auto buttonCenter = (buttonSize / 2);
    ImGui::BeginGroup();
    {
      auto cursorPos = ImGui::GetCursorPos();
      if (ImGui::ImageButton("##event_image", static_cast<ImTextureID>(m_buttonBack), ImVec2{80.f, 102.f})) {
        m_characterPicker.setCharacterInfo(m_image, m_character);
        m_characterPicker.setOpen(true);
      }
      if (m_characterSheet->texture()) {
        if (m_characterSheet->characterWidth() < 72 || m_characterSheet->characterHeight() < 96) {
          ImGui::SetCursorPos(cursorPos + (ImVec2{m_characterSheet->characterWidth() / 2.f, m_characterSheet->characterHeight() / 2.f}));
        } else {
          ImGui::SetCursorPos(cursorPos);
        }
        const auto [min, max] = m_characterSheet->getRectForCharacter(m_character);
        ImGui::Image(m_characterSheet->texture(),
                     ImVec2{static_cast<float>(m_characterSheet->characterWidth()), static_cast<float>(m_characterSheet->characterHeight())}, min, max);
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
        setOpen(false);
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        ImGui::CloseCurrentPopup();
        setOpen(false);
      }
      ImGui::EndGroup();
    }

    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
