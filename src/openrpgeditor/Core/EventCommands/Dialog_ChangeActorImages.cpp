#include "Core/EventCommands/Dialog_ChangeActorImages.hpp"
#include "Core/Application.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/Log.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ChangeActorImages::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{400, 225} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    if (const auto [closed, confirmed] = m_characterPicker.draw(); closed) {
      if (confirmed) {
        m_characterPicker.Accept();
        if (m_image_selection == 0) {
          m_faceIndex = m_characterPicker.character();
          m_facePicture = m_characterPicker.selectedSheet();
          m_faceSheet = FaceSheet(m_facePicture);
        } else if (m_image_selection == 1) {
          m_charIndex = m_characterPicker.character();
          m_charPicture = m_characterPicker.selectedSheet();
          m_characterSheet = CharacterSheet(m_charPicture);
        } else {
          m_battlerPicture = m_characterPicker.selectedSheet();
          m_battlerSheet = SideViewBattlerSheet(m_battlerPicture);
        }
      }
    }
    if (actor_picker) {
      auto [closed, confirmed] = actor_picker->draw();
      if (confirmed) {
        m_actor = actor_picker->selection();
        actor_picker.reset();
      }
    }
    ImGui::PushID("##actor_selection");
    if (ImGui::Button((std::format("{:04} ", m_actor) + Database::instance()->actorName(m_actor)).c_str(), {(App::DPIHandler::scale_value(160)), 0})) {

      actor_picker = ObjectPicker<Actor>("Actor"sv, Database::instance()->actors.actorList(), 0);
    }
    ImGui::PopID();
    ImGui::BeginGroup();
    {
      ImGui::SeparatorText("Images");
      const auto buttonSize = ImVec2{144, 144} * App::DPIHandler::get_ui_scale();
      const auto buttonCenter = (buttonSize / 2);
      ImGui::BeginGroup();
      {

        ImGui::Text("Face:");
        auto cursorPos = ImGui::GetCursorPos();
        if (ImGui::ImageButton("##svbattler_image", m_buttonBack, ImVec2{80.f, 102.f} * App::DPIHandler::get_ui_scale())) {
          m_image_selection = 0;
          m_characterPicker.setCharacterInfo(m_charPicture, m_actor);
          m_characterPicker.SetOpen(true);
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
        ImGui::Text("Character:");
        auto cursorPos = ImGui::GetCursorPos();
        if (ImGui::ImageButton("##char_image", m_buttonBack, ImVec2{80.f, 102.f} * App::DPIHandler::get_ui_scale())) {
          m_image_selection = 1;
          m_characterPicker.setCharacterInfo(m_facePicture, m_charIndex);
          m_characterPicker.SetOpen(true);
        }
        if (m_characterSheet && m_characterSheet->texture()) {
          if (m_characterSheet->characterWidth() < 72 || m_characterSheet->characterHeight() < 96) {
            ImGui::SetCursorPos(cursorPos + (ImVec2{m_characterSheet->characterWidth() / 2.f, m_characterSheet->characterHeight() / 2.f} * App::DPIHandler::get_ui_scale()));
          } else {
            ImGui::SetCursorPos(cursorPos);
          }

          const auto [min, max] = m_characterSheet->getRectForCharacter(m_charIndex);

          ImGui::Image(m_characterSheet->texture(),
                       ImVec2{static_cast<float>(m_characterSheet->characterWidth()), static_cast<float>(m_characterSheet->characterHeight())} * App::DPIHandler::get_ui_scale(), min, max);
        }
      }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        ImGui::Text("[SV] Battler:");
        auto cursorPos = ImGui::GetCursorPos();
        if (ImGui::ImageButton("##svbattler_image", m_buttonBack, ImVec2{80.f, 102.f} * App::DPIHandler::get_ui_scale())) {
          m_image_selection = 2;
          m_characterPicker.setCharacterInfo(m_battlerPicture, m_faceIndex);
          m_characterPicker.SetOpen(true);
        }
        if (m_battlerSheet && m_battlerSheet->texture()) {
          const auto battlerRect = ImVec2{static_cast<float>(m_battlerSheet->characterWidth()) * 2, static_cast<float>(m_battlerSheet->characterHeight()) * 2} * App::DPIHandler::get_ui_scale();
          ImGui::SetCursorPos((cursorPos + buttonCenter) - (battlerRect / 2));
          const auto rect = m_battlerSheet->getAction(SideViewActionType::StepForward);
          ImGui::Image(m_battlerSheet->texture(), battlerRect, rect.frames[1].min, rect.frames[1].max);
        }
      }
      ImGui::EndGroup();
    }
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.f);
    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        m_confirmed = true;
        command->actor = m_actor;
        command->charPicture = m_charPicture;
        command->charIndex = m_charIndex;
        command->facePicture = m_facePicture;
        command->faceIndex = m_faceIndex;
        command->battlerPicture = m_battlerPicture;
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
