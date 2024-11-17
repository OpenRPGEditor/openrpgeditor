#include "Core/EventCommands/Dialog_ChangeSkill.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_ChangeSkill::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{240, 210} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {

    if (actor_picker) {
      auto [closed, confirmed] = actor_picker->draw();
      if (confirmed) {
        m_value = actor_picker->selection();
        actor_picker.reset();
      }
    }
    if (skill_picker) {
      auto [closed, confirmed] = skill_picker->draw();
      if (confirmed) {
        m_skill = skill_picker->selection();
        skill_picker.reset();
      }
    }

    if (picker) {
      auto [closed, confirmed] = picker->draw();
      if (confirmed) {
        m_var = picker->selection();
        picker.reset();
      }
    }
    ImGui::SeparatorText("Actor");
    ImGui::BeginGroup(); {
      ImGui::RadioButton("Fixed", &m_comparison, 0);
      ImGui::RadioButton("Variable", &m_comparison, 1);
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup(); {
      ImGui::BeginDisabled(m_comparison != 0);
      ImGui::PushID("##changeskill_actor");
      if (ImGui::Button(
              m_comparison == 0 ? (std::format("{:04} ", m_value) + Database::instance().actorNameOrId(m_value)).c_str() : "",
              ImVec2{200 - (15 * App::DPIHandler::get_ui_scale()), 0})) {

        actor_picker = ObjectPicker<Actor>("Actor"sv, Database::instance().actors.actorList(), 0);
      }
      ImGui::PopID();
      ImGui::EndDisabled();


      ImGui::BeginDisabled(m_comparison != 1);
      ImGui::PushID("##changeskill_var");
      if (ImGui::Button(
              m_comparison == 1 ? (std::format("{:04} ", m_var) + Database::instance().variableNameOrId(m_var)).c_str() : "",
              ImVec2{200 - (15 * App::DPIHandler::get_ui_scale()), 0})) {
        picker.emplace("Variables", Database::instance().system.variables);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      ImGui::EndGroup();
    }

    ImGui::SeparatorText("Operation");
    ImGui::RadioButton("Learn", &m_skillOp, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Forget", &m_skillOp, 1);

    ImGui::SeparatorText("Skill");
    ImGui::PushID("##changeskill_skill_selection");
    if (ImGui::Button(
            (std::format("{:04} ", m_skill) + Database::instance().skillNameOrId(m_skill)).c_str(),
            ImVec2{(App::DPIHandler::scale_value(225)), 0})) {
      skill_picker = ObjectPicker<Skill>("Skill"sv, Database::instance().skills.skills(), 0);
    }
    ImGui::PopID();

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->comparison = static_cast<ActorComparisonSource>(m_comparison);
      command->skill = m_skill;
      command->skillOp = static_cast<SkillOperation>(m_skillOp);
      if (command->comparison == ActorComparisonSource::Fixed) {
        command->value = m_value;
      }
      else {
        command->value = m_var;
      }
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
