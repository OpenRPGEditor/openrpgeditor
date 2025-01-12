#include "Core/EventCommands/Dialog_RecoverAll.hpp"

#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_RecoverAll::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{402, 117}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    if (picker) {
      auto [closed, confirmed] = picker->draw();
      if (closed) {
        if (confirmed) {
          m_variable = picker->selection();
        }
        picker.reset();
      }
    }
    if (actor_picker) {
      auto [closed, confirmed] = actor_picker->draw();
      if (closed) {
        if (confirmed) {
          m_actor = actor_picker->selection();
        }
        actor_picker.reset();
      }
    }

    ImGui::SeparatorText("Actor");

    ImGui::BeginGroup();
    {
      ImGui::RadioButton("Fixed", &m_Source, 0);
      ImGui::RadioButton("Variable", &m_Source, 1);
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      // Actor Button
      ImGui::BeginDisabled(m_Source != 0);
      ImGui::SetNextItemWidth(100);
      std::string text = m_Source != 0 ? "##commonevent_switch_empty" : Database::instance()->actorName(m_actor);
      ImGui::PushID("##recoverall_selection_actor");
      if (ImGui::Button(text.c_str(), ImVec2{200 - 15, 0})) {
        actor_picker = ObjectPicker<Actor>("Actor"sv, Database::instance()->actors.actorList(), m_actor);
        actor_picker->setUseDummyEntry("Entire Party");
        actor_picker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      // Variable Button
      ImGui::BeginDisabled(m_Source != 1);
      ImGui::SetNextItemWidth(100);
      text = m_Source != 1 ? "##commonevent_switch_empty" : Database::instance()->variableName(m_variable);
      ImGui::PushID("##controlvariable_gamedata");
      if (ImGui::Button(text.c_str(), ImVec2{200 - 15, 0})) {
        picker.emplace("Variables", Database::instance()->system.variables(), m_variable);
        picker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      ImGui::EndGroup();
    }

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->comparison = static_cast<ActorComparisonSource>(m_Source);
      if (command->comparison == ActorComparisonSource::Fixed) {
        command->value = m_actor;
      } else {
        command->value = m_variable;
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
