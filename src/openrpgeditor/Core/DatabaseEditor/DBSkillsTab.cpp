#include "Core/DatabaseEditor/DBSkillsTab.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Skills.hpp"

#include "imgui_internal.h"
#include "imgui.h"
DBSkillsTab::DBSkillsTab(Skills& skills, DatabaseEditor* parent) : IDBEditorTab(parent), m_skills(skills) {
  m_selectedSkill = m_skills.skill(1);
  if (m_selectedSkill) {
    m_effectsEditor.setEffects(&m_selectedSkill->effects());
  }
  m_maxSkills = m_skills.count();
}

void DBSkillsTab::draw() {
  if (m_animationPicker) {
    if (const auto [closed, confirmed] = m_animationPicker->draw(); closed) {
      if (confirmed) {
        m_selectedSkill->setAnimationId(m_animationPicker->selection());
      }
      m_animationPicker.reset();
    }
  }
  ImGui::BeginChild("#orpg_skills_editor");
  {
    const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
    ImGui::BeginChild("##orpg_skills_editor_skills", ImVec2{calc.x + (ImGui::GetStyle().ItemSpacing.x * 2), 0}, ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX, ImGuiWindowFlags_NoScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Skills");
        ImGui::BeginChild("##orpg_skills_editor_skills_list", ImVec2{0, ImGui::GetContentRegionMax().y - ((calc.y + ImGui::GetStyle().ItemSpacing.y) * 4)}, ImGuiChildFlags_Borders);
        {
          ImGui::BeginGroup();
          {
            for (auto& skill_ : m_skills.skills()) {
              if (skill_.id() == 0) {
                continue;
              }

              char name[4096];
              snprintf(name, 4096, "%04i %s", skill_.id(), skill_.name().c_str());
              if (ImGui::Selectable(name, &skill_ == m_selectedSkill) || (ImGui::IsItemFocused() && m_selectedSkill != &skill_)) {ImGui::ClearActiveID();
                m_selectedSkill = &skill_;
                m_effectsEditor.setEffects(&m_selectedSkill->effects());
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Skills %i", m_maxSkills);
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxSkills = m_maxSkills;
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_skills_editor_skills_skill_properties", {}, ImGuiChildFlags_Borders);
    {
      if (m_selectedSkill) {
        ImGui::BeginChild("##orpg_skills_skill_panel_left", ImVec2{ImGui::GetContentRegionMax().x / 2, 0.f}, ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
        {
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("General Settings");
            // Name
            ImGui::BeginGroup();
            {
              char name[4096];
              strncpy(name, m_selectedSkill->name().c_str(), 4096);
              if (ImGui::LabelOverLineEdit("##orpg_skills_editor_skills_skill_name", "Name:", name, 4096, ImGui::GetContentRegionMax().x / 2 - 16, nullptr,
                                         ImGuiInputTextFlags_None)) {
                m_selectedSkill->setName(name);
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            // Icon
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Icon:");
              // ImGui::SameLine();
              const auto* iconSheet = m_parent->getIconSheet();
              auto [min, max] = iconSheet->rectForId(m_selectedSkill->iconIndex());
              ImGui::Image(iconSheet->texture(), ImVec2{static_cast<float>(iconSheet->iconWidth()), static_cast<float>(iconSheet->iconHeight())}, min,
                           max); // Show icon image
              ImGui::Text("%s", std::to_string(m_selectedSkill->iconIndex()).c_str());
            }
            ImGui::EndGroup();
            // Description
            ImGui::BeginGroup();
            {
              char description[4096];
              strncpy(description, m_selectedSkill->description().c_str(), 4096);
              ImGui::TextUnformatted("Description:");
              ImGui::InputTextMultiline("##orpg_database_skills_description", description, 4096, ImVec2{360, 60});
            }
            ImGui::EndGroup();
            // Skill Type, MP/TP Cost
            ImGui::BeginGroup();
            {
              ImGui::BeginGroup();
              {
                ImGui::Text("Skill Type:");
                ImGui::SetNextItemWidth(160);
                if (ImGui::BeginCombo("##orpg_database_skills_skilltype", m_selectedSkill->stypeId() == 0 ? "None" : Database::instance()->system.skillType(m_selectedSkill->stypeId()).c_str())) {
                  int index{0};
                  for (auto& dataSource : Database::instance()->system.skillTypes()) {
                    const bool is_selected = m_selectedSkill->stypeId() == index;
                    if (index == 0) {
                      if (ImGui::Selectable("None", is_selected)) {
                        m_selectedSkill->setStypeId(index);
                        if (is_selected)
                          ImGui::SetItemDefaultFocus();
                      }
                    } else {
                      if (ImGui::Selectable(dataSource.c_str(), is_selected)) {
                        m_selectedSkill->setStypeId(index);
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
              ImGui::BeginGroup();
              {
                ImGui::Text("MP Cost:");
                ImGui::SetNextItemWidth(150);
                int mpCost = m_selectedSkill->mpCost();
                if (ImGui::InputInt("##orpg_database_skills_manacost", &mpCost, 1, 100)) {
                  m_selectedSkill->setMpCost(std::clamp(mpCost, 0, 9999));
                }
              }
              ImGui::EndGroup();
              ImGui::SameLine();
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
              ImGui::BeginGroup();
              {
                ImGui::Text("TP Cost:");
                ImGui::SetNextItemWidth(150);
                int tpCost = m_selectedSkill->tpCost();
                if (ImGui::InputInt("##orpg_database_skills_tcost", &tpCost, 1, 100)) {
                  m_selectedSkill->setTpCost(std::clamp(tpCost, 0, 999));
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
                ImGui::SetNextItemWidth(160);
                if (ImGui::BeginCombo("##orpg_database_skills_scopelist", DecodeEnumName(magic_enum::enum_name(m_selectedSkill->scope())).c_str())) {
                  int index{0};
                  for (auto& dir : magic_enum::enum_values<Scope>()) {
                    if (const bool is_selected = m_selectedSkill->scope() == static_cast<Scope>(magic_enum::enum_index(dir).value());
                        ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                      m_selectedSkill->setScope(static_cast<Scope>(magic_enum::enum_index(dir).value()));
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
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
              ImGui::BeginGroup();
              {
                ImGui::Text("Occasion:");
                ImGui::SetNextItemWidth(160);
                if (ImGui::BeginCombo("##orpg_database_skills_occasionlist", DecodeEnumName(magic_enum::enum_name(m_selectedSkill->occasion())).c_str())) {
                  int index{0};
                  for (auto& dir : magic_enum::enum_values<Occasion>()) {
                    if (const bool is_selected = m_selectedSkill->occasion() == static_cast<Occasion>(magic_enum::enum_index(dir).value());
                        ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                      m_selectedSkill->setOccasion(dir);
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
              ImGui::SetNextItemWidth(170);
              int speed = m_selectedSkill->speed();
              if (ImGui::InputInt("##orpg_database_skills_speed", &speed, 1, 100)) {
                m_selectedSkill->setSpeed(std::clamp(speed, -2000, 2000));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Success:");
              ImGui::SetNextItemWidth(170);
              int successRate = m_selectedSkill->successRate();
              if (ImGui::InputInt("##orpg_database_skills_successRate", &successRate, 1, 100)) {
                m_selectedSkill->setSuccessRate(std::clamp(successRate, 0, 100));
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Repeat:");
              ImGui::SetNextItemWidth(170);
              int repeats = m_selectedSkill->repeats();
              if (ImGui::InputInt("##orpg_database_skills_repeats", &repeats, 1, 100)) {
                m_selectedSkill->setRepeats(std::clamp(repeats, 1, 9));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("TP Gain:");
              ImGui::SetNextItemWidth(170);
              int tpGain = m_selectedSkill->tpGain();
              if (ImGui::InputInt("##orpg_database_skills_tpGain", &tpGain, 1, 100)) {
                m_selectedSkill->setTpGain(std::clamp(tpGain, 0, 100));
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Hit Type:");
              ImGui::SetNextItemWidth(170);
              if (ImGui::BeginCombo("##orpg_database_skills_hitType", DecodeEnumName(magic_enum::enum_name(m_selectedSkill->hitType())).c_str())) {
                for (auto& dir : magic_enum::enum_values<HitType>()) {
                  if (const bool is_selected = m_selectedSkill->hitType() == static_cast<HitType>(magic_enum::enum_index(dir).value());
                      ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                    m_selectedSkill->setHitType(dir);
                    if (is_selected)
                      ImGui::SetItemDefaultFocus();
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            // Animation
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Animation:");
              ImGui::SetNextItemWidth(160);
              // Animation Button
              ImGui::PushID("##orpg_database_skills_animation");
              if (ImGui::Button(m_selectedSkill->animationId() == -1  ? "Normal Attack"
                                : m_selectedSkill->animationId() == 0 ? "None"
                                                                      : Database::instance()->animationName(m_selectedSkill->animationId()).c_str(),
                                ImVec2{200 - 15, 0})) {
                m_animationPicker = ObjectPicker("Animation"sv, Database::instance()->animations.animations(), m_selectedSkill->animationId());
                m_animationPicker->setOpen(true);
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
            strncpy(message1, m_selectedSkill->message1().c_str(), 4096);
            char message2[4096];
            strncpy(message2, m_selectedSkill->message2().c_str(), 4096);

            ImGui::Text("(User Name)");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(450);
            ImGui::InputText("##orpg_database_message_message1", message1, 4096);
            ImGui::SetNextItemWidth(534);
            ImGui::InputText("##orpg_database_message_message2", message2, 4096);
            // ImGui::EndGroup();

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 80.f);
            ImGui::SetNextItemWidth(400);
            if (ImGui::BeginCombo("##orpg_database_message_templateList", m_message_templateList.at(m_message_template).c_str())) {
              for (int index{0}; auto& _ : m_message_templateList) {
                if (const bool is_selected = m_message_template == index; ImGui::Selectable(m_message_templateList.at(index).c_str(), is_selected)) {
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
              ImGui::SetNextItemWidth(170);
              if (ImGui::BeginCombo("##orpg_database_reqweapon_1",
                                    m_selectedSkill->requiredWtypeId1() == 0 ? "None" : Database::instance()->system.weaponType(m_selectedSkill->requiredWtypeId1()).c_str())) {
                for (int index{0}; auto& _ : Database::instance()->system.weaponTypes()) {
                  const bool is_selected = m_selectedSkill->requiredWtypeId1() == index;
                  if (index == 0) {
                    if (ImGui::Selectable("None", is_selected)) {
                      m_selectedSkill->setRequiredWtypeId1(index);
                      if (is_selected)
                        ImGui::SetItemDefaultFocus();
                    }
                  } else {
                    if (ImGui::Selectable(Database::instance()->system.weaponType(index).c_str(), is_selected)) {
                      m_selectedSkill->setRequiredWtypeId1(index);
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
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Weapon Type 2");
              ImGui::SetNextItemWidth(170);
              if (ImGui::BeginCombo("##orpg_database_reqweapon_2",
                                    m_selectedSkill->requiredWtypeId2() == 0 ? "None" : Database::instance()->system.weaponType(m_selectedSkill->requiredWtypeId2()).c_str())) {
                int index{0};
                for (auto& _ : Database::instance()->system.weaponTypes()) {
                  const bool is_selected = m_selectedSkill->requiredWtypeId2() == index;
                  if (index == 0) {
                    if (ImGui::Selectable("None", is_selected)) {
                      m_selectedSkill->setRequiredWtypeId2(index);
                      if (is_selected)
                        ImGui::SetItemDefaultFocus();
                    }
                  } else {
                    if (ImGui::Selectable(Database::instance()->system.weaponType(index).c_str(), is_selected)) {
                      m_selectedSkill->setRequiredWtypeId2(index);
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
        ImGui::BeginChild("####orpg_skills_skill_panel_right", {}, ImGuiChildFlags_Borders);
        {
          ImGui::SeparatorText("Damage");
          ImGui::BeginGroup();
          {
            ImGui::BeginGroup();
            {
              ImGui::BeginGroup();
              {
                ImGui::Text("Type:");
                ImGui::SetNextItemWidth((ImGui::GetContentRegionAvail().x / 2) - ImGui::GetStyle().FramePadding.x);
                if (ImGui::BeginCombo("##orpg_database_skills_damage_type", DecodeEnumName(magic_enum::enum_name(m_selectedSkill->damage().type())).c_str())) {
                  int index{0};
                  for (auto& dir : magic_enum::enum_values<DamageType>()) {
                    if (const bool isSelected = m_selectedSkill->damage().type() == dir; ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), isSelected)) {
                      m_selectedSkill->damage().setType(dir);
                      if (isSelected)
                        ImGui::SetItemDefaultFocus();
                    }
                    index++;
                  }
                  ImGui::EndCombo();
                }
              }
              ImGui::EndGroup();
              ImGui::SameLine();
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
              ImGui::BeginGroup();
              {
                ImGui::TextUnformatted(trNOOP("Element:"));
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
                if (ImGui::BeginCombo("##orpg_database_skills_damage_element",
                                      m_selectedSkill->damage().elementId() == -1 ? trNOOP("Normal Attack")
                                      : m_selectedSkill->damage().elementId() == 0
                                          ? trNOOP("None")
                                          : (Database::instance()->elementNameOrId(m_selectedSkill->damage().elementId()) + "##" + std::to_string(m_selectedSkill->damage().elementId())).c_str())) {
                  int index{-1};
                  for (auto& elem : Database::instance()->system.elements()) {
                    const bool isSelected = m_selectedSkill->damage().elementId() == index;
                    if (index == -1) {
                      if (ImGui::Selectable(trNOOP("Normal Attack"), isSelected)) {
                        m_selectedSkill->damage().setElementId(index);
                      }
                    } else if (index == 0) {
                      if (ImGui::Selectable(trNOOP("None"), isSelected)) {
                        m_selectedSkill->damage().setElementId(index);
                      }
                    } else {
                      if (ImGui::Selectable(std::format("{}##{}", elem, index).c_str(), isSelected)) {
                        m_selectedSkill->damage().setElementId(index);
                      }
                    }
                    if (isSelected) {
                      ImGui::SetItemDefaultFocus();
                    }
                    index++;
                  }
                  ImGui::EndCombo();
                }
              }
              ImGui::EndGroup();
              char formula[4096];
              strncpy(formula, m_selectedSkill->damage().formula().c_str(), 4096);
              ImGui::LabelOverLineEdit("##orpg_database_skills_formula", trNOOP("Formula"), formula, 4096, ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, nullptr,
                                         ImGuiInputTextFlags_None);
              ImGui::BeginGroup();
              {
                ImGui::TextUnformatted("Variance:");
                int variance = m_selectedSkill->damage().variance();
                if (ImGui::InputInt("##orpg_database_skills_variance", &variance)) {
                  m_selectedSkill->damage().setVariance(std::clamp(variance, 0, 100));
                }
              }
              ImGui::EndGroup();
              ImGui::SameLine();
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
              ImGui::BeginGroup();
              {
                ImGui::Text("Critical Hits:");
                bool critical = m_selectedSkill->damage().critical();
                if (ImGui::Checkbox("##orpg_database_skills_crits", &critical)) {
                  m_selectedSkill->damage().setCritical(critical);
                }
              }
              ImGui::EndGroup();

              // Begin effects drawing
              m_effectsEditor.draw(m_parent);

              ImGui::BeginGroup();
              {
                ImGui::SeparatorText("Note:");
                char note[8192];
                strncpy(note, m_selectedSkill->note().c_str(), IM_ARRAYSIZE(note));
                if (ImGui::InputTextMultiline("##orpg_database_skills_note", note, IM_ARRAYSIZE(note), ImVec2{ImGui::GetContentRegionMax().x - 16, ImGui::GetContentRegionAvail().y - 16})) {
                  m_selectedSkill->setNote(note);
                }
              }
              ImGui::EndGroup();
            }
            ImGui::EndGroup();
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
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
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
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text("Are you sure?");
        if (ImGui::Button("Yes")) {
          const int tmpId = m_selectedSkill->id();
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
