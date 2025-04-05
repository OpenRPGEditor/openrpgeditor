#include "Core/EventCommands/Dialog_NameInputProcessing.hpp"

#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_NameInputProcessing::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{200, 140}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    if (m_actorPicker) {
      auto [closed, confirmed] = m_actorPicker->draw();
      if (closed) {
        if (confirmed) {
          m_actor = m_actorPicker->selection();
        }
        m_actorPicker.reset();
      }
    }

    ImGui::SeparatorText("Actor");
    ImGui::SetNextItemWidth(100);
    ImGui::PushID("##nameinput_actor");
    if (ImGui::Button(Database::instance()->actorNameOrId(m_actor).c_str(), ImVec2{180, 0})) {
      m_actorPicker = ObjectPicker<Actor>("Actor"sv, Database::instance()->actors.actorList(), m_actor);
      m_actorPicker->setOpen(true);
    }
    ImGui::PopID();
    ImGui::SeparatorText("Max characters");
    ImGui::SetNextItemWidth(80);
    if (ImGui::InputInt("##inputnumber_digits", &m_maxChar)) {
      if (m_maxChar > 16) {
        m_maxChar = 16;
      } else if (m_maxChar < 1)
        m_maxChar = 1;
    }

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      m_command->actorId = m_actor;
      m_command->maxChar = m_maxChar;
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
