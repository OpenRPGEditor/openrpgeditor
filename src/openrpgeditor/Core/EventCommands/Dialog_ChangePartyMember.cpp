#include "Core/EventCommands/Dialog_ChangePartyMember.hpp"

#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ChangePartyMember::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{248, 158}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

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
    ImGui::SetNextItemWidth(100);
    ImGui::PushID("##changepartymember_actor_selection");
    if (ImGui::Button(Database::instance()->actorName(m_actor).c_str(), ImVec2{200 - 15, 0})) {
      actor_picker = ObjectPicker<Actor>("Actor"sv, Database::instance()->actors.actorList(), m_actor);
      actor_picker->setOpen(true);
    }
    ImGui::PopID();

    ImGui::SeparatorText("Operation");
    ImGui::RadioButton("Add", &m_operation, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Remove", &m_operation, 1);

    ImGui::Checkbox("Initialize", &m_isInit);

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->member = m_actor;
      command->operation = static_cast<PartyMemberOperation>(m_operation);
      command->initialize = m_isInit;
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
