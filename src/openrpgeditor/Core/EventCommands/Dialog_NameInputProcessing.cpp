#include "Dialog_NameInputProcessing.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Project.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_NameInputProcessing::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{200, 140} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {

    if (actor_picker) {
      auto [closed, confirmed] = actor_picker->draw();
      if (confirmed) {
        m_actor = actor_picker->selection();
        actor_picker.reset();
      }
    }

    ImGui::SeparatorText("Actor");
    ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
    ImGui::PushID("##nameinput_actor");
    if (ImGui::Button(Database::Instance->actorNameOrId(m_actor).c_str(),
                      ImVec2{(App::DPIHandler::scale_value(180)), 0})) {
      actor_picker = ObjectPicker<Actor>("Actor"sv, Database::Instance->actors.actorList(), 0);
    }
    ImGui::PopID();
    ImGui::SeparatorText("Max characters");
    ImGui::SetNextItemWidth(App::DPIHandler::scale_value(80));
    if (ImGui::InputInt("##inputnumber_digits", &m_maxChar)) {
      if (m_maxChar > 16) {
        m_maxChar = 16;
      } else if (m_maxChar < 1)
        m_maxChar = 1;
    }

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->actorId = m_actor;
      command->maxChar = m_maxChar;
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
