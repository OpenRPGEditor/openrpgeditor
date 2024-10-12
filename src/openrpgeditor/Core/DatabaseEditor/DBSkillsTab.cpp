#include "Core/DatabaseEditor/DBSkillsTab.hpp"
#include "Database/Skills.hpp"
#include "Core/Application.hpp"
#include "imgui.h"
#include "Core/ImGuiUtils.hpp"
DBSkillsTab::DBSkillsTab(Skills& skills, DatabaseEditor* parent) : IDBEditorTab(parent), m_skills(skills) {
  m_selectedSkill = m_skills.skill(1);
  if (m_selectedSkill) {
    // m_traitsEditor.setTraits(&m_selectedClass->traits);
  }
  m_maxSkills = m_skills.count();
}

void DBSkillsTab::draw() {
  if (animation_picker) {
    auto [closed, confirmed] = animation_picker->draw();
    if (confirmed) {
      m_selectedSkill->animationId = animation_picker->selection();
      animation_picker.reset();
    }
  }
  ImGui::BeginChild("#orpg_skills_editor");
  {
    ImGui::BeginChild("##orpg_skills_editor_skills", ImVec2{250.f, 0} * App::DPIHandler::get_ui_scale(), 0,
                      ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Skills");
        ImGui::BeginChild("##orpg_skills_editor_skills_list",
                          ImVec2{0, ImGui::GetContentRegionMax().y - (App::DPIHandler::scale_value(108))});
        {
          ImGui::BeginGroup();
          {
            for (auto& skill_ : m_skills.skills()) {
              if (skill_.id == 0) {
                continue;
              }

              char name[4096];
              snprintf(name, 4096, "%04i %s", skill_.id, skill_.name.c_str());
              if (ImGui::Selectable(name, &skill_ == m_selectedSkill) ||
                  (ImGui::IsItemFocused() && m_selectedSkill != &skill_)) {
                m_selectedSkill = &skill_;
                // m_traitsEditor.setTraits(&m_selectedClass->traits);
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Skills %i", m_maxSkills);
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max",
                          ImVec2{ImGui::GetContentRegionMax().x - (App::DPIHandler::scale_value(8)), 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxSkills = m_maxSkills;
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_skills_editor_skills_skill_properties");
    {
      if (m_selectedSkill) {
        ImGui::BeginChild("##orpg_skills_skill_panel_left", ImVec2{ImGui::GetContentRegionMax().x / 2, 0.f});
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("General Settings");
            // Name
            ImGui::BeginGroup();
            {
              char name[4096];
              strncpy(name, m_selectedSkill->name.c_str(), 4096);
              if (ImGui::LabelOverLineEdit("##orpg_actors_editor_actors_actor_name", "Name:", name, 4096,
                                           (ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(16))) {
                m_selectedSkill->name = name;
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            // Icon
            ImGui::BeginGroup();
            {
              ImGui::Text("Icon:");
              // ImGui::SameLine();
              // ImGui::Image(); // Show icon image
              ImGui::Text("%s", std::to_string(m_selectedSkill->iconIndex).c_str());
            }
            ImGui::EndGroup();
            // Description
            ImGui::BeginGroup();
            {
              char description[4096];
              strncpy(description, m_selectedSkill->description.c_str(), 4096);
              ImGui::Text("Description:");
              ImGui::InputTextMultiline("##orpg_database_skills_description", description, 4096,
                                        ImVec2{App::DPIHandler::scale_value(500), App::DPIHandler::scale_value(60)});
            }
            ImGui::EndGroup();
            // Skill Type, MP/TP Cost
            ImGui::BeginGroup();
            {
              ImGui::BeginGroup();
              {
                ImGui::Text("Skill Type:");
                float cursorPosY = ImGui::GetCursorPosY();
                ImGui::SetNextItemWidth(App::DPIHandler::scale_value(200));
                if (ImGui::BeginCombo("##orpg_database_skills_skilltype",
                                      m_selectedSkill->stypeId == 0
                                          ? "None"
                                          : Database::Instance->system.skillType(m_selectedSkill->stypeId)->c_str())) {
                  int index{0};
                  for (auto& dataSource : Database::Instance->system.skillTypes) {
                    bool is_selected = (m_selectedSkill->stypeId == index);
                    if (index == 0) {
                      if (ImGui::Selectable("None", is_selected)) {
                        m_selectedSkill->stypeId = index;
                        if (is_selected)
                          ImGui::SetItemDefaultFocus();
                      }
                    } else {
                      if (ImGui::Selectable(dataSource.c_str(), is_selected)) {
                        m_selectedSkill->stypeId = index;
                        if (is_selected)
                          ImGui::SetItemDefaultFocus();
                      }
                    }

                    index++;
                  }
                  ImGui::EndCombo();
                }
              }
              ImGui::EndGroup();
              ImGui::SameLine();
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.f);
              ImGui::BeginGroup();
              {
                ImGui::Text("MP Cost:");
                ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
                if (ImGui::InputInt("##orpg_database_skills_manacost", &m_selectedSkill->mpCost, 1, 100)) {
                  if (m_selectedSkill->mpCost < 0)
                    m_selectedSkill->mpCost = 0;
                  if (m_selectedSkill->mpCost > 9999)
                    m_selectedSkill->mpCost = 9999;
                }
              }
              ImGui::EndGroup();
              ImGui::SameLine();
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.f);
              ImGui::BeginGroup();
              {
                ImGui::Text("TP Cost:");
                ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
                if (ImGui::InputInt("##orpg_database_skills_tcost", &m_selectedSkill->tpCost, 1, 100)) {
                  if (m_selectedSkill->tpCost < 0)
                    m_selectedSkill->tpCost = 0;
                  if (m_selectedSkill->tpCost > 999)
                    m_selectedSkill->tpCost = 999;
                }
              }
              ImGui::EndGroup();
              ImGui::SameLine();
            }
            ImGui::EndGroup();
            // Scope, Occasion
            ImGui::BeginGroup();
            {
              ImGui::BeginGroup();
              {
                ImGui::Text("Scope:");
                ImGui::SetNextItemWidth(App::DPIHandler::scale_value(200));
                if (ImGui::BeginCombo("##orpg_database_skills_scopelist",
                                      DecodeEnumName(magic_enum::enum_name(m_selectedSkill->scope)).c_str())) {
                  int index{0};
                  for (auto& dir : magic_enum::enum_values<Scope>()) {
                    bool is_selected =
                        m_selectedSkill->scope == static_cast<Scope>(magic_enum::enum_index(dir).value());
                    if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                      m_selectedSkill->scope = static_cast<Scope>(magic_enum::enum_index(dir).value());
                      if (is_selected)
                        ImGui::SetItemDefaultFocus();
                    }
                    index++;
                  }
                  ImGui::EndCombo();
                }
              }
              ImGui::EndGroup();
              ImGui::SameLine();
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.f);
              ImGui::BeginGroup();
              {
                ImGui::Text("Occasion:");
                ImGui::SetNextItemWidth(App::DPIHandler::scale_value(200));
                if (ImGui::BeginCombo("##orpg_database_skills_occasionlist",
                                      DecodeEnumName(magic_enum::enum_name(m_selectedSkill->occasion)).c_str())) {
                  int index{0};
                  for (auto& dir : magic_enum::enum_values<Occasion>()) {
                    bool is_selected =
                        m_selectedSkill->occasion == static_cast<Occasion>(magic_enum::enum_index(dir).value());
                    if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                      m_selectedSkill->occasion = static_cast<Occasion>(magic_enum::enum_index(dir).value());
                      if (is_selected)
                        ImGui::SetItemDefaultFocus();
                    }
                    index++;
                  }
                  ImGui::EndCombo();
                }
              }
              ImGui::EndGroup();
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          // Invocation

          ImGui::SeparatorText("Invocation");
          ImGui::BeginGroup();
          {
            ImGui::BeginGroup();
            {
              ImGui::Text("Speed:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
              if (ImGui::InputInt("##orpg_database_skills_speed", &m_selectedSkill->speed, 1, 100)) {
                if (m_selectedSkill->speed < -2000)
                  m_selectedSkill->speed = -2000;
                if (m_selectedSkill->speed > 2000)
                  m_selectedSkill->speed = 2000;
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Success:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
              if (ImGui::InputInt("##orpg_database_skills_successRate", &m_selectedSkill->successRate, 1, 100)) {
                if (m_selectedSkill->successRate < 0)
                  m_selectedSkill->successRate = 0;
                if (m_selectedSkill->successRate > 100)
                  m_selectedSkill->successRate = 100;
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Repeat:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
              if (ImGui::InputInt("##orpg_database_skills_repeats", &m_selectedSkill->repeats, 1, 100)) {
                if (m_selectedSkill->repeats < 1)
                  m_selectedSkill->repeats = 1;
                if (m_selectedSkill->repeats > 9)
                  m_selectedSkill->repeats = 9;
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("TP Gain:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
              if (ImGui::InputInt("##orpg_database_skills_tpGain", &m_selectedSkill->tpGain, 1, 100)) {
                if (m_selectedSkill->tpGain < 0)
                  m_selectedSkill->tpGain = 0;
                if (m_selectedSkill->tpGain > 100)
                  m_selectedSkill->tpGain = 100;
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Hit Type:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(200));
              if (ImGui::BeginCombo("##orpg_database_skills_hitType",
                                    DecodeEnumName(magic_enum::enum_name(m_selectedSkill->hitType)).c_str())) {
                int index{0};
                for (auto& dir : magic_enum::enum_values<HitType>()) {
                  bool is_selected =
                      m_selectedSkill->hitType == static_cast<HitType>(magic_enum::enum_index(dir).value());
                  if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                    m_selectedSkill->hitType = static_cast<HitType>(magic_enum::enum_index(dir).value());
                    if (is_selected)
                      ImGui::SetItemDefaultFocus();
                  }
                  index++;
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            // Animation
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Animation:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(200));
              // Animation Button
              ImGui::PushID("##orpg_database_skills_animation");
              if (ImGui::Button(m_selectedSkill->animationId == -1 ? "Normal Attack"
                                : m_selectedSkill->animationId == 0
                                    ? "None"
                                    : Database::Instance->animationName(m_selectedSkill->animationId).c_str(),
                                ImVec2{200 - (15 * App::DPIHandler::get_ui_scale()), 0})) {
                animation_picker =
                    ObjectPicker<Animation>("Animation"sv, Database::Instance->animations.animations(), 0);
              }
              ImGui::PopID();
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::SeparatorText("Message");
          ImGui::BeginGroup();
          {
            char message1[4096];
            strncpy(message1, m_selectedSkill->message1.c_str(), 4096);
            char message2[4096];
            strncpy(message2, m_selectedSkill->message2.c_str(), 4096);

            ImGui::Text("(User Name)");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(App::DPIHandler::scale_value(450));
            ImGui::InputText("##orpg_database_message_message1", message1, 4096);
            ImGui::SetNextItemWidth(App::DPIHandler::scale_value(534));
            ImGui::InputText("##orpg_database_message_message2", message2, 4096);
            // ImGui::EndGroup();

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 80.f);
            ImGui::SetNextItemWidth(App::DPIHandler::scale_value(400));
            if (ImGui::BeginCombo("##orpg_database_message_templateList",
                                  m_message_templateList.at(m_message_template).c_str())) {
              int index{0};
              for (auto& msg : m_message_templateList) {
                bool is_selected = m_message_template == index;
                if (ImGui::Selectable(m_message_templateList.at(index).c_str(), is_selected)) {
                  m_message_template = index;
                  if (is_selected)
                    ImGui::SetItemDefaultFocus();
                }
                index++;
              }
              ImGui::EndCombo();
            }
          }
          ImGui::EndGroup();
          ImGui::SeparatorText("Required Weapon");
          ImGui::BeginGroup();
          {
            ImGui::BeginGroup();
            {
              ImGui::Text("Weapon Type 1");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(200));
              if (ImGui::BeginCombo(
                      "##orpg_database_reqweapon_1",
                      m_selectedSkill->requiredWtypeId1 == 0
                          ? "None"
                          : Database::Instance->system.weaponType(m_selectedSkill->requiredWtypeId1)->c_str())) {
                int index{0};
                for (auto& msg : Database::Instance->system.weaponTypes) {
                  bool is_selected = m_selectedSkill->requiredWtypeId1 == index;
                  if (index == 0) {
                    if (ImGui::Selectable("None", is_selected)) {
                      m_selectedSkill->requiredWtypeId1 = index;
                      if (is_selected)
                        ImGui::SetItemDefaultFocus();
                    }
                  } else {
                    if (ImGui::Selectable(Database::Instance->system.weaponType(index)->c_str(), is_selected)) {
                      m_selectedSkill->requiredWtypeId1 = index;
                      if (is_selected)
                        ImGui::SetItemDefaultFocus();
                    }
                  }

                  index++;
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Weapon Type 2");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(200));
              if (ImGui::BeginCombo(
                      "##orpg_database_reqweapon_2",
                      m_selectedSkill->requiredWtypeId2 == 0
                          ? "None"
                          : Database::Instance->system.weaponType(m_selectedSkill->requiredWtypeId2)->c_str())) {
                int index{0};
                for (auto& msg : Database::Instance->system.weaponTypes) {
                  bool is_selected = m_selectedSkill->requiredWtypeId2 == index;
                  if (index == 0) {
                    if (ImGui::Selectable("None", is_selected)) {
                      m_selectedSkill->requiredWtypeId2 = index;
                      if (is_selected)
                        ImGui::SetItemDefaultFocus();
                    }
                  } else {
                    if (ImGui::Selectable(Database::Instance->system.weaponType(index)->c_str(), is_selected)) {
                      m_selectedSkill->requiredWtypeId2 = index;
                      if (is_selected)
                        ImGui::SetItemDefaultFocus();
                    }
                  }
                  index++;
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("####orpg_skills_skill_panel_right");
        {
          ImGui::SeparatorText("Damage");
          ImGui::BeginGroup();
          {
            ImGui::BeginGroup();
            {
              ImGui::BeginGroup();
              {
                ImGui::Text("Type:");
                ImGui::SetNextItemWidth(App::DPIHandler::scale_value(200));
                if (ImGui::BeginCombo("##orpg_database_skills_damage_type",
                                      DecodeEnumName(magic_enum::enum_name(m_selectedSkill->damage.type)).c_str())) {
                  int index{0};
                  for (auto& dir : magic_enum::enum_values<DamageType>()) {
                    bool is_selected =
                        m_selectedSkill->damage.type == static_cast<DamageType>(magic_enum::enum_index(dir).value());
                    if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                      m_selectedSkill->damage.type = static_cast<DamageType>(magic_enum::enum_index(dir).value());
                      if (is_selected)
                        ImGui::SetItemDefaultFocus();
                    }
                    index++;
                  }
                  ImGui::EndCombo();
                }
                ImGui::EndGroup();
              }
              ImGui::SameLine();
              ImGui::BeginGroup();
              {
                ImGui::Text("Element:");
                ImGui::SetNextItemWidth(App::DPIHandler::scale_value(200));
                if (ImGui::BeginCombo(
                        "##orpg_database_skills_damage_element",
                        m_selectedSkill->damage.elementId == -1 ? "Normal Attack"
                        : m_selectedSkill->damage.elementId == 0
                            ? "None"
                            : Database::Instance->system.element(m_selectedSkill->damage.elementId)->c_str())) {
                  int index{-1};
                  for (auto& msg : Database::Instance->system.elements) {
                    bool is_selected = m_selectedSkill->damage.elementId == index;
                    if (index == -1) {
                      if (ImGui::Selectable("Normal Attack", is_selected)) {
                        m_selectedSkill->damage.elementId = index;
                        if (is_selected)
                          ImGui::SetItemDefaultFocus();
                      }
                    } else if (index == 0) {
                      if (ImGui::Selectable("None", is_selected)) {
                        m_selectedSkill->damage.elementId = index;
                        if (is_selected)
                          ImGui::SetItemDefaultFocus();
                      }
                    } else {
                      if (ImGui::Selectable(
                              Database::Instance->system.element(m_selectedSkill->damage.elementId)->c_str(),
                              is_selected)) {
                        m_selectedSkill->damage.elementId = index;
                        if (is_selected)
                          ImGui::SetItemDefaultFocus();
                      }
                    }
                    index++;
                  }
                  ImGui::EndCombo();
                }
                ImGui::EndGroup();
              }
              char formula[4096];
              strncpy(formula, m_selectedSkill->damage.formula.c_str(), 4096);
              ImGui::LabelOverLineEdit("##orpg_database_skills_formula", "Formula", formula, 4096, 400.f);
              ImGui::BeginGroup();
              {
                // TODO: Needs to be formatted to use %
                ImGui::Text("Variance:");
                if (ImGui::InputInt("##orpg_database_skills_variance", &m_selectedSkill->damage.variance)) {
                  if (m_selectedSkill->damage.variance < 0)
                    m_selectedSkill->damage.variance = 0;
                  if (m_selectedSkill->damage.variance > 100)
                    m_selectedSkill->damage.variance = 100;
                }
                ImGui::EndGroup();
              }
              ImGui::SameLine();
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.f);
              ImGui::BeginGroup();
              {
                ImGui::Text("Critical Hits:");
                ImGui::Checkbox("##orpg_database_skills_crits", &m_selectedSkill->damage.critical);
              }
              ImGui::EndGroup();
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
      }
    }
    ImGui::EndChild();
  }
  ImGui::EndChild();
  if (m_changeIntDialogOpen) {
    if (ImGui::Begin("Change Max Classes", &m_changeIntDialogOpen,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal |
                         ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
      ImGui::Text(
          "Specify an amount to resize the skill list to\n"
          "This cannot be undone!");
      ImGui::InputInt("##value_input", &m_editMaxSkills);
      ImGui::SameLine();
      if (ImGui::Button("Accept")) {
        m_changeConfirmDialogOpen = true;
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        m_changeIntDialogOpen = false;
      }
    }
    ImGui::End();

    if (m_changeConfirmDialogOpen) {
      if (ImGui::Begin("Confirm Change", &m_changeConfirmDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoSavedSettings |
                           ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text("Are you sure?");
        if (ImGui::Button("Yes")) {
          int tmpId = m_selectedSkill->id;
          m_maxSkills = m_editMaxSkills;
          m_skills.resize(m_maxSkills);
          m_selectedSkill = m_skills.skill(tmpId);
          m_changeIntDialogOpen = false;
          m_changeConfirmDialogOpen = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          m_editMaxSkills = m_maxSkills;
          m_changeIntDialogOpen = false;
          m_changeConfirmDialogOpen = false;
        }
      }
      ImGui::End();
    }
  }
}
