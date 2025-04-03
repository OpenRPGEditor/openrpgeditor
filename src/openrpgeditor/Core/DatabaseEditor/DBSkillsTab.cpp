#include "Core/DatabaseEditor/DBSkillsTab.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Skills.hpp"

#include "imgui.h"
#include "imgui_internal.h"
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
  if (!m_iconButtonTexture) {
    m_iconButtonTexture.emplace();
    m_iconButtonTexture->setSize(56, 56);
  }
  if (m_selectedSkill && !m_iconButtonTexture->hasCompositeTextures()) {
    const auto* iconSheet = m_parent->getIconSheet();
    const auto& [uv0, uv1] = iconSheet->rectForId(m_selectedSkill->iconIndex());
    const Point offset{static_cast<int>(uv0.x() * iconSheet->texture().width()), static_cast<int>(uv0.y() * iconSheet->texture().height())};
    m_iconButtonTexture->setTexturesToComposite({{iconSheet->texture(), {iconSheet->iconWidth(), iconSheet->iconHeight()}, offset}});
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
              if (ImGui::Selectable(name, &skill_ == m_selectedSkill) || (ImGui::IsItemFocused() && m_selectedSkill != &skill_)) {
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
              if (ImGui::LabelOverLineEdit(std::format("##orpg_skills_editor_skills_skill_name_{}", m_selectedSkill->id()).c_str(), "Name:", name, 4096, ImGui::GetContentRegionMax().x / 2 - 16,
                                           nullptr, 0)) {
                m_selectedSkill->setName(name);
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            // Icon
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y);
            ImGui::BeginGroup();
            {
              ImGui::BeginGroup();
              {
                ImGui::TextUnformatted("Icon:");
                ImGui::Text("%s", std::to_string(m_selectedSkill->iconIndex()).c_str());
              }
              ImGui::EndGroup();
              ImGui::SameLine();
              // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().ItemSpacing.y);
              if (ImGui::ImageButtonEx(ImGui::GetID("##orpg_database_items_image"), m_iconButtonTexture->get(), ImVec2(m_iconButtonTexture->size()), {0.f, 0.f}, {1.f, 1.f}, {}, {1.f, 1.f, 1.f, 1.f},
                                       ImGuiButtonFlags_PressedOnDoubleClick)) {}
            }
            ImGui::EndGroup();
            // Description
            ImGui::BeginGroup();
            {
              char description[4096];
              strncpy(description, m_selectedSkill->description().c_str(), 4096);
              ImGui::TextUnformatted("Description:");
              if (ImGui::InputTextMultiline(std::format("##orpg_database_skills_description_{}", m_selectedSkill->id()).c_str(), description, 4096,
                                            ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, ImGui::CalcTextSize("A").y * 2 + ImGui::GetStyle().FramePadding.y * 2})) {
                m_selectedSkill->setDescription(description);
              }
            }
            ImGui::EndGroup();
            // Skill Type, MP/TP Cost
            ImGui::BeginGroup();
            {
              ImGui::BeginGroup();
              {
                ImGui::Text("Skill Type:");
                ImGui::SetNextItemWidth((ImGui::GetContentRegionAvail().x / 2) - ImGui::GetStyle().FramePadding.x);
                if (ImGui::BeginCombo(std::format("##orpg_database_skills_skilltype_{}", m_selectedSkill->id()).c_str(),
                                      m_selectedSkill->stypeId() == 0 ? "None" : Database::instance()->system.skillType(m_selectedSkill->stypeId()).c_str())) {
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
              ImGui::SameLine();
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y);
              ImGui::BeginGroup();
              {
                ImGui::Text("MP Cost:");
                ImGui::SetNextItemWidth((ImGui::GetContentRegionAvail().x / 2) - ImGui::GetStyle().FramePadding.x);
                int mpCost = m_selectedSkill->mpCost();
                if (ImGui::InputInt(std::format("##orpg_database_skills_manacost_{}", m_selectedSkill->id()).c_str(), &mpCost, 1, 100)) {
                  m_selectedSkill->setMpCost(std::clamp(mpCost, 0, 9999));
                }
              }
              ImGui::EndGroup();
              ImGui::SameLine();
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y);
              ImGui::BeginGroup();
              {
                ImGui::Text("TP Cost:");
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
                int tpCost = m_selectedSkill->tpCost();
                if (ImGui::InputInt(std::format("##orpg_database_skills_tcost_{}", m_selectedSkill->id()).c_str(), &tpCost, 1, 100)) {
                  m_selectedSkill->setTpCost(std::clamp(tpCost, 0, 999));
                }
              }
              ImGui::EndGroup();
            }
            ImGui::EndGroup();
            // Scope, Occasion
            ImGui::BeginGroup();
            {
              ImGui::BeginGroup();
              {
                ImGui::Text("Scope:");
                ImGui::SetNextItemWidth((ImGui::GetContentRegionAvail().x / 2) - ImGui::GetStyle().FramePadding.x);
                if (ImGui::BeginCombo(std::format("##orpg_database_skills_scopelist_{}", m_selectedSkill->id()).c_str(), DecodeEnumName(magic_enum::enum_name(m_selectedSkill->scope())).c_str())) {
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
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y);
              ImGui::BeginGroup();
              {
                ImGui::Text("Occasion:");
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
                if (ImGui::BeginCombo(std::format("##orpg_database_skills_occasionlist_{}", m_selectedSkill->id()).c_str(),
                                      DecodeEnumName(magic_enum::enum_name(m_selectedSkill->occasion())).c_str())) {
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
              ImGui::SetNextItemWidth((ImGui::GetContentRegionAvail().x / 4) - ImGui::GetStyle().FramePadding.x);
              int speed = m_selectedSkill->speed();
              if (ImGui::InputInt(std::format("##orpg_database_skills_speed_{}", m_selectedSkill->id()).c_str(), &speed, 1, 100)) {
                m_selectedSkill->setSpeed(std::clamp(speed, -2000, 2000));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y);
            ImGui::BeginGroup();
            {
              ImGui::Text("Success:");
              ImGui::SetNextItemWidth((ImGui::GetContentRegionAvail().x / 3) - ImGui::GetStyle().FramePadding.x);
              int successRate = m_selectedSkill->successRate();
              if (ImGui::InputInt(std::format("##orpg_database_skills_successRate_{}", m_selectedSkill->id()).c_str(), &successRate, 1, 100)) {
                m_selectedSkill->setSuccessRate(std::clamp(successRate, 0, 100));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y);
            ImGui::BeginGroup();
            {
              ImGui::Text("Repeat:");
              ImGui::SetNextItemWidth((ImGui::GetContentRegionAvail().x / 2) - ImGui::GetStyle().FramePadding.x);
              int repeats = m_selectedSkill->repeats();
              if (ImGui::InputInt("##orpg_database_skills_repeats", &repeats, 1, 100)) {
                m_selectedSkill->setRepeats(std::clamp(repeats, 1, 9));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y);
            ImGui::BeginGroup();
            {
              ImGui::Text("TP Gain:");
              ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
              int tpGain = m_selectedSkill->tpGain();
              if (ImGui::InputInt(std::format("##orpg_database_skills_tpGain_{}", m_selectedSkill->id()).c_str(), &tpGain, 1, 100)) {
                m_selectedSkill->setTpGain(std::clamp(tpGain, 0, 100));
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Hit Type:");
              ImGui::SetNextItemWidth((ImGui::GetContentRegionAvail().x / 2) - ImGui::GetStyle().FramePadding.x);
              if (ImGui::BeginCombo(std::format("##orpg_database_skills_hitType_{}", m_selectedSkill->id()).c_str(), DecodeEnumName(magic_enum::enum_name(m_selectedSkill->hitType())).c_str())) {
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
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y);
            ImGui::BeginGroup();
            {
              ImGui::Text("Animation:");
              // Animation Button
              ImGui::PushID(std::format("##orpg_database_skills_animation_{}", m_selectedSkill->id()).c_str());
              if (ImGui::Button(m_selectedSkill->animationId() == -1  ? "Normal Attack"
                                : m_selectedSkill->animationId() == 0 ? "None"
                                                                      : Database::instance()->animationName(m_selectedSkill->animationId()).c_str(),
                                ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
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

            auto textColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);
            textColor.w = 0.5f;
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::Text("(User Name)");
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - (ImGui::GetContentRegionAvail().x / 4) - ImGui::GetStyle().FramePadding.x);
            if (ImGui::InputText(std::format("##orpg_database_message_message1_{}", m_selectedSkill->id()).c_str(), message1, 4096)) {
              m_selectedSkill->setMessage1(message1);
            }
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
            if (ImGui::BeginCombo(std::format("##orpg_database_message_templateList_{}", m_selectedSkill->id()).c_str(), m_message_templateList.at(m_message_template).c_str())) {
              for (int index{0}; auto& msg : m_message_templateList) {
                if (msg == m_selectedSkill->message1() && m_message_template != index) {
                  m_message_template = index;
                }
                if (const bool is_selected = m_message_template == index; ImGui::Selectable(m_message_templateList.at(index).c_str(), is_selected)) {
                  m_message_template = index;
                  m_selectedSkill->setMessage1(msg);
                  if (is_selected)
                    ImGui::SetItemDefaultFocus();
                }
                index++;
              }
              ImGui::EndCombo();
            }
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
            if (ImGui::InputText(std::format("##orpg_database_message_message2_{}", m_selectedSkill->id()).c_str(), message2, 4096)) {
              m_selectedSkill->setMessage2(message2);
            }
          }
          ImGui::EndGroup();
          ImGui::SeparatorText("Required Weapon");
          ImGui::BeginGroup();
          {
            ImGui::BeginGroup();
            {
              ImGui::Text("Weapon Type 1");
              ImGui::SetNextItemWidth((ImGui::GetContentRegionAvail().x / 2) - ImGui::GetStyle().FramePadding.x);
              if (ImGui::BeginCombo(std::format("##orpg_database_reqweapon_1_{}", m_selectedSkill->id()).c_str(),
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
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y);
            ImGui::BeginGroup();
            {
              ImGui::Text("Weapon Type 2");
              ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
              if (ImGui::BeginCombo(std::format("##orpg_database_reqweapon_2_{}", m_selectedSkill->id()).c_str(),
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
        ImGui::BeginChild("##orpg_skills_skill_panel_right", {}, ImGuiChildFlags_Borders);
        {
          ImGui::BeginChild("##damage", {}, ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_Borders);
          {
            ImGui::SeparatorText("Damage");
            ImGui::BeginGroup();
            {
              ImGui::BeginGroup();
              {
                ImGui::Text("Type:");
                ImGui::SetNextItemWidth((ImGui::GetContentRegionAvail().x / 2) - ImGui::GetStyle().FramePadding.x);
                if (ImGui::BeginCombo(std::format("##orpg_database_skills_damage_type_{}", m_selectedSkill->id()).c_str(),
                                      DecodeEnumName(magic_enum::enum_name(m_selectedSkill->damage().type())).c_str())) {
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
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y);
              ImGui::BeginGroup();
              {
                ImGui::TextUnformatted(trNOOP("Element:"));
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
                if (ImGui::BeginCombo(std::format("##orpg_database_skills_damage_element_{}", m_selectedSkill->id()).c_str(),
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
                if (ImGui::InputInt(std::format("##orpg_database_skills_variance_{}", m_selectedSkill->id()).c_str(), &variance)) {
                  m_selectedSkill->damage().setVariance(std::clamp(variance, 0, 100));
                }
              }
              ImGui::EndGroup();
              ImGui::SameLine();
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y);
              ImGui::BeginGroup();
              {
                ImGui::Text("Critical Hits:");
                bool critical = m_selectedSkill->damage().critical();
                if (ImGui::Checkbox("##orpg_database_skills_crits", &critical)) {
                  m_selectedSkill->damage().setCritical(critical);
                }
              }
              ImGui::EndGroup();
            }
            ImGui::EndGroup();
          }
          ImGui::EndChild();
          // Begin effects drawing
          ImGui::BeginChild(std::format("##effects_{}", m_selectedSkill->id()).c_str(), {}, ImGuiChildFlags_ResizeY | ImGuiChildFlags_Borders);
          { m_effectsEditor.draw(m_parent); }
          ImGui::EndChild();

          ImGui::BeginChild("##note", {}, ImGuiChildFlags_Borders);
          {
            ImGui::SeparatorText("Note:");
            char note[8192];
            strncpy(note, m_selectedSkill->note().c_str(), IM_ARRAYSIZE(note));
            if (ImGui::InputTextMultiline(std::format("##orpg_database_skills_note_{}", m_selectedSkill->id()).c_str(), note, IM_ARRAYSIZE(note),
                                          ImVec2{ImGui::GetContentRegionMax().x - 16, ImGui::GetContentRegionAvail().y - 16})) {
              m_selectedSkill->setNote(note);
            }
          }
          ImGui::EndChild();
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
