#include "Core/EventCommands/Dialog_ChangeNickname.hpp"

#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ChangeNickname::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{183, 135}, ImGuiCond_Appearing);
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

    // Actor Button
    ImGui::PushID("##nickname_selection_actor");
    if (ImGui::Button(Database::instance()->actorName(m_actor).c_str(), ImVec2{200 - 15, 0})) {
      actor_picker = ObjectPicker<Actor>("Actor"sv, Database::instance()->actors.actorList(), m_actor);
      actor_picker->setOpen(true);
    }
    ImGui::PopID();

    ImGui::SeparatorText("Nickname");
    ImGui::SetNextItemWidth(185);
    ImGui::InputText("##nickname_input", &m_nickname);

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->actor = m_actor;
      command->nick = m_nickname;
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
