#include "Core/EventCommands/Dialog_ShowAnimation.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_ShowAnimation::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(windowSize * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open,
                             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {

    if (animation_picker) {
      auto [closed, confirmed] = animation_picker->draw();
      if (confirmed) {
        m_animation = animation_picker->selection();
        animation_picker.reset();
      }
    }
    ImGui::SeparatorText("Character");
    ImGui::PushItemWidth((App::DPIHandler::scale_value(160)));
    if (ImGui::BeginCombo("##showanim_character", Database::Instance->eventNameOrId(m_character).c_str())) {

      if (ImGui::Selectable("Player", m_character == -1)) {
        m_character = -1;
        ImGui::SetItemDefaultFocus();
      }
      if (ImGui::Selectable("This Event", m_character == 0)) {
        m_character = 0;
        ImGui::SetItemDefaultFocus();
      }

      for (auto& dataSource : Database::Instance->mapInfos.currentMap()->map()->events) {
        if (!dataSource.has_value())
          continue;

        bool is_selected = (m_character == dataSource->id);
        if (ImGui::Selectable(("EV" + std::format("{:03} ", dataSource->id)).c_str(), is_selected)) {
          m_character = dataSource->id;
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    // Animation Button
    ImGui::PushID("##showanim_animation_select");
    if (ImGui::Button(Database::Instance->animationName(m_animation).c_str(),
                      ImVec2{200 - (15 * App::DPIHandler::get_ui_scale()), 0})) {
      animation_picker = ObjectPicker<Animation>("Animation"sv, Database::Instance->animations.animations(), 0);
    }
    ImGui::PopID();
    // Wait for completion
    ImGui::Checkbox("Wait for Completion", &m_waitCompletion);

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->character = m_character;
      command->animation = m_animation;
      command->waitForCompletion = m_waitCompletion;
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    windowSize = ImVec2{ImGui::GetContentRegionMax().x, ImGui::GetContentRegionMax().y};
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
