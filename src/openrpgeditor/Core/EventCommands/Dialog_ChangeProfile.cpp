#include "Core/EventCommands/Dialog_ChangeProfile.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_ChangeProfile::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{300, 220} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open,
                             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {

    if (actor_picker) {
      auto [closed, confirmed] = actor_picker->draw();
      if (confirmed) {
        m_actor = actor_picker->selection();
        actor_picker.reset();
      }
      if (closed) {
        actor_picker.reset();
      }
    }

    ImGui::SeparatorText("Actor");

    // Actor Button
    ImGui::PushID("##nickname_selection_actor");
    if (ImGui::Button(Database::instance().actorName(m_actor).c_str(), ImVec2{(App::DPIHandler::scale_value(280)), 0})) {
      actor_picker = ObjectPicker("Actor"sv, Database::instance().actors.actorList(), m_actor);
    }
    ImGui::PopID();

    ImGui::SeparatorText("Profile");
    ImGui::SetNextItemWidth(App::DPIHandler::scale_value(280));
    ImGui::InputTextMultiline("##profile_input", &m_profile,
                              ImVec2{App::DPIHandler::scale_value(280), App::DPIHandler::scale_value(100)});

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->actor = m_actor;
      command->profile = m_profile;
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
