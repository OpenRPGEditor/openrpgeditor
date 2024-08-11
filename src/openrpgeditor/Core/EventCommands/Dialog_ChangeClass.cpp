#include "Dialog_ChangeClass.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_ChangeClass::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{183, 160} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {

    if (actor_picker) {
      auto [closed, confirmed] = actor_picker->draw();
      if (confirmed) {
        m_actor = actor_picker->selection();
        actor_picker.reset();
      }
    }
    if (class_picker) {
      auto [closed, confirmed] = class_picker->draw();
      if (confirmed) {
        m_class = class_picker->selection();
        class_picker.reset();
      }
    }

    ImGui::SeparatorText("Actor");

    // Actor Button
    ImGui::PushID("##change_class_actor");
    if (ImGui::Button(Database::Instance->actorName(m_actor).c_str(), ImVec2{200 - (15 * App::DPIHandler::get_ui_scale()), 0})) {
      actor_picker = ObjectPicker("Actor"sv, Database::Instance->actors.actorList(), 0);
    }
    ImGui::PopID();

    ImGui::SeparatorText("Class");

    // Actor Button
    ImGui::PushID("##change_class_classid");
    if (ImGui::Button(Database::Instance->className(m_class).c_str(), ImVec2{200 - (15 * App::DPIHandler::get_ui_scale()), 0})) {
      class_picker = ObjectPicker<Class>("Class"sv, Database::Instance->classes.classes(), 0);
    }
    ImGui::PopID();

    // saveLevel checkbox
    ImGui::Checkbox("Save Level", &m_saveLevel);

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->actor = m_actor;
      command->classId = m_class;
      command->saveLevel = m_saveLevel;
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
