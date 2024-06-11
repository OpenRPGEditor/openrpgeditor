#include "Core/DatabaseEditor/DBActorsTab.hpp"
#include "Database/Actors.hpp"

#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/DatabaseEditor.hpp"
#include "Core/ImGuiUtils.hpp"
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/ImGuiParsedText.hpp"

DBActorsTab::DBActorsTab(Actors& actors, DatabaseEditor* parent) : IDBEditorTab(parent), m_actors(actors) {
  m_selectedActor = m_actors.actor(1);
  m_maxActors = m_actors.count();
}

void DBActorsTab::draw() {
  if (m_selectedActor != nullptr && !m_charaterSheet) {
    m_charaterSheet.emplace(m_selectedActor->characterName);
  }

  ImGui::BeginChild("##orpg_actors_editor");
  {
    ImGui::BeginChild("##orpg_actors_editor_actors", ImVec2{250.f, 0} * App::DPIHandler::get_scale(), 0, ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Actors");
        ImGui::BeginChild("##orpg_actors_editor_actor_list",
                          ImVec2{0, ImGui::GetContentRegionMax().y - (108 * App::DPIHandler::get_scale())});
        {
          ImGui::BeginGroup();
          {
            for (auto& actor : m_actors.m_actors) {
              if (actor.id == 0) {
                continue;
              }

              char name[4096];
              snprintf(name, 4096, "%04i %s", actor.id, actor.name.c_str());
              if (ImGui::Selectable(name, &actor == m_selectedActor) ||
                  (ImGui::IsItemFocused() && m_selectedActor != &actor)) {
                m_selectedActor = &actor;
                m_charaterSheet.emplace(m_selectedActor->characterName);
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Actors %i", m_maxActors);
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max",
                          ImVec2{ImGui::GetContentRegionMax().x - (8 * App::DPIHandler::get_scale()), 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxActors = m_maxActors;
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_actors_editor_actors_actor_properties");
    {
      if (m_selectedActor) {
        ImGui::BeginChild("##orpg_actors_actor_panel_left", ImVec2{ImGui::GetContentRegionMax().x / 2, 0.f});
        {
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("General Settings");
            char name[4096];
            strncpy(name, m_selectedActor->name.c_str(), 4096);
            if (ImGui::LabelOverLineEdit("##orpg_actors_editor_actors_actor_name", "Name:", name, 4096,
                                         (ImGui::GetContentRegionMax().x / 2) - 16)) {
              m_selectedActor->name = name;
            }
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
            strncpy(name, m_selectedActor->nickname.c_str(), 4096);
            if (ImGui::LabelOverLineEdit("##orpg_actors_editor_actors_actor_nickname", "Nickname:", name, 4096,
                                         (ImGui::GetContentRegionMax().x / 2) - 16)) {
              m_selectedActor->nickname = name;
            }
            ImGui::BeginGroup();
            {
              ImGui::Text("Class:");
              Classes classes = m_parent->classes();
              auto cls = classes.classType(m_selectedActor->classId);
              ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 16);
              char buf[4096];
              snprintf(buf, 4096, "%04i %s", cls->id, cls->name.c_str());
              if (ImGui::BeginCombo("##orpg_actors_editor_class_combo", buf)) {
                for (const auto& c : classes.classes()) {
                  snprintf(buf, 4096, "%04i %s", c.id, c.name.c_str());
                  if (ImGui::Selectable(buf, c.id == m_selectedActor->classId)) {
                    m_selectedActor->classId = c.id;
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
            ImGui::BeginGroup();
            {
              ImGui::Text("Initial Level:");
              ImGui::SetNextItemWidth((ImGui::GetCursorPosX() / 2) - 16);
              ImGui::InputInt("##orpg_actors_initial_level_edit", &m_selectedActor->initialLevel);
              m_selectedActor->initialLevel = std::clamp(m_selectedActor->initialLevel, 1, 99);
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
            ImGui::BeginGroup();
            {
              ImGui::Text("Max Level:");
              ImGui::SetNextItemWidth(((ImGui::GetContentRegionMax().x / 2) / 2) - 16);
              ImGui::InputInt("##orpg_actors_max_level_edit", &m_selectedActor->maxLevel);
              m_selectedActor->maxLevel = std::clamp(m_selectedActor->maxLevel, 1, 99);
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Profile:");
              char profile[8192];
              strncpy(profile, m_selectedActor->profile.c_str(), IM_ARRAYSIZE(profile));
              if (ImGui::InputTextMultiline("##orpg_actors_profile", profile, IM_ARRAYSIZE(profile),
                                            ImVec2{ImGui::GetContentRegionMax().x - 16, 0})) {
                m_selectedActor->profile = profile;
              }
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Images");
            ImGui::BeginGroup();
            {
              ImGui::Text("Face:");
              ImGui::ImageButton("##orpg_actors_face_image", m_buttonBack.get(), ImVec2{160, 160});
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            // ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2;
            ImGui::BeginGroup();
            {
              ImGui::Text("Character:");
              ImGui::ImageButton("##orpg_actors_character_image", m_buttonBack.get(), ImVec2{160, 160});
              if (m_charaterSheet && m_charaterSheet->texture()) {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                                     ((160 / 2) - (static_cast<float>((m_charaterSheet->characterWidth()) * 2) / 2)));
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() -
                                     ((108) + (static_cast<float>((m_charaterSheet->characterHeight()) * 2) / 2)));
                auto rect = m_charaterSheet->getRectForCharacter(m_selectedActor->characterIndex, 1);
                ImVec2 uv0{rect.uv0.u, rect.uv0.v};
                ImVec2 uv1{rect.uv1.u, rect.uv1.v};
                ImGui::Image(m_charaterSheet->texture().get(),
                             ImVec2{static_cast<float>(m_charaterSheet->characterWidth()) * 2,
                                    static_cast<float>(m_charaterSheet->characterHeight()) * 2},
                             uv0, uv1);
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            // ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
            ImGui::BeginGroup();
            {
              ImGui::Text("[SV] Battler:");
              ImGui::ImageButton("##orpg_actors_battler_image", m_buttonBack.get(), ImVec2{160, 160});
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 128);
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Initial Equipment");
            if (ImGui::BeginTable("##orpg_actors_actor_init_equip", 2,
                                  ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY |
                                      ImGuiTableFlags_ScrollY,
                                  ImVec2{ImGui::GetContentRegionMax().x - 15, ImGui::GetContentRegionAvail().y - 16})) {

              ImGui::TableSetupColumn("Type");
              ImGui::TableSetupColumn("Equipment Item");
              ImGui::TableHeadersRow();
              /* Weapon */
              ImGui::TableNextRow();
              if (ImGui::TableNextColumn()) {
                if (ImGui::Selectable(m_parent->equipType(1).c_str(), false,
                                      ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
                  if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                    m_showEquipEdit = true;
                    m_selectedEquip = 0;
                    m_chosenEquip =
                        m_selectedEquip < m_selectedActor->equips.size() ? m_selectedActor->equips[m_selectedEquip] : 0;
                  }
                }
              }
              if (ImGui::TableNextColumn()) {
                std::string label;
                if (!m_selectedActor->equips.empty()) {
                  auto weapon = m_parent->weapon(m_selectedActor->equips[0]);
                  label = weapon ? weapon->name : "None";
                } else {
                  label = "None";
                }
                ImGui::Text("%s", label.c_str());
              }
              for (int i = 2; i < m_parent->equipTypes().size(); ++i) {
                ImGui::TableNextRow();
                if (ImGui::TableNextColumn()) {
                  if (ImGui::Selectable(m_parent->equipType(i).c_str(), false,
                                        ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
                    if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                      m_showEquipEdit = true;
                      m_selectedEquip = i - 1;
                      m_chosenEquip = m_selectedEquip < m_selectedActor->equips.size()
                                          ? m_selectedActor->equips[m_selectedEquip]
                                          : 0;
                    }
                  }
                }
                if (ImGui::TableNextColumn()) {
                  std::string label;
                  const int armorId = i - 1;
                  if (armorId < m_selectedActor->equips.size()) {
                    auto armor = m_parent->armor(m_selectedActor->equips[armorId]);
                    label = armor ? armor->name : "None";
                  } else {
                    label = "None";
                  }
                  ImGui::Text("%s", label.c_str());
                }
              }
              ImGui::EndTable();
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##orpg_actors_actor_panel_right");
        {
          m_traitsEditor.draw(m_selectedActor->traits, m_parent);

          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Note:");
            char note[8192];
            strncpy(note, m_selectedActor->note.c_str(), IM_ARRAYSIZE(note));
            if (ImGui::InputTextMultiline(
                    "##orpg_actors_note", note, IM_ARRAYSIZE(note),
                    ImVec2{ImGui::GetContentRegionMax().x - (16 * ImGui::GetIO().FontGlobalScale),
                           ImGui::GetContentRegionAvail().y - (16 * ImGui::GetIO().FontGlobalScale)})) {
              m_selectedActor->note = note;
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
    if (ImGui::Begin("Change Max Actors", &m_changeIntDialogOpen,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal |
                         ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
      ImGui::Text(
          "Specify an amount to resize the actor list to\n"
          "This cannot be undone!");
      ImGui::InputInt("##value_input", &m_editMaxActors);
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
          int tmpId = m_selectedActor->id;
          m_maxActors = m_editMaxActors;
          m_actors.resize(m_maxActors);
          m_selectedActor = m_actors.actor(tmpId);
          m_changeIntDialogOpen = false;
          m_changeConfirmDialogOpen = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          m_editMaxActors = m_maxActors;
          m_changeIntDialogOpen = false;
          m_changeConfirmDialogOpen = false;
        }
      }
      ImGui::End();
    }
  }

  if (m_showEquipEdit) {
    if (ImGui::Begin("Select Equipment...", &m_showEquipEdit,
                     ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal |
                         ImGuiWindowFlags_NoCollapse)) {
      if (m_selectedEquip == 0) {
        ImGui::Text("Weapon:");
        const std::vector<Weapon>& weapons = m_parent->weapons();
        std::vector<Weapon> filtered;
        auto cls = m_parent->classes().classType(m_selectedActor->classId);
        if (cls && !cls->traits.empty()) {
          auto it = std::find_if(cls->traits.begin(), cls->traits.end(), [](const auto& c) {
            return c.code == TraitCode::Equip_Weapon; // Weapon
          });
          if (it != cls->traits.end()) {
            for (const auto& weapon : weapons) {
              if (weapon.wtypeId == it->dataId) {
                filtered.push_back(weapon);
              }
            }
          }
        }
        std::string label;
        if (!m_selectedActor->equips.empty()) {
          auto weapon = m_parent->weapon(m_chosenEquip);
          label = weapon ? weapon->name : "None";
        } else {
          label = "None";
        }

        if (ImGui::BeginCombo("##orpg_weapon_selection", label.c_str())) {
          if (ImGui::Selectable("None", m_chosenEquip == 0)) {
            m_chosenEquip = 0;
          }

          for (auto w : filtered) {
            std::string label = w.name.empty() ? "##orpg_weapon_name_invalid_" + std::to_string(w.id) : w.name;
            if (ImGui::Selectable(label.c_str(), m_chosenEquip == w.id)) {
              m_chosenEquip = w.id;
            }
          }
          ImGui::EndCombo();
        }
      } else {
        ImGui::Text("Armor:");
        std::vector<Armor> filtered;
        const std::vector<Armor>& armors = m_parent->armors();
        auto cls = m_parent->classes().classType(m_selectedActor->classId);
        if (cls && !cls->traits.empty()) {
          auto it = std::find_if(cls->traits.begin(), cls->traits.end(), [](const auto& c) {
            return c.code == TraitCode::Equip_Armor; // Armor
          });
          if (it != cls->traits.end()) {
            for (const auto& armor : armors) {
              if (armor.atypeId == it->dataId && armor.etypeId == m_selectedEquip + 1) {
                filtered.push_back(armor);
              }
            }
          }
        }
        std::string label;
        if (!m_selectedActor->equips.empty()) {
          auto armor = m_parent->armor(m_chosenEquip);
          label = armor ? armor->name : "None";
        } else {
          label = "None";
        }

        if (ImGui::BeginCombo("##orpg_armor_selection", label.c_str())) {
          if (ImGui::Selectable("None", m_chosenEquip == 0)) {
            m_chosenEquip = 0;
          }

          for (auto a : filtered) {
            std::string label = a.name.empty() ? "##orpg_armor_name_invalid_" + std::to_string(a.id) : a.name;
            if (ImGui::Selectable(label.c_str(), m_chosenEquip == a.id)) {
              m_chosenEquip = a.id;
            }
          }
          ImGui::EndCombo();
        }
      }
      ImGui::SameLine();
      if (ImGui::Button("OK")) {
        m_showEquipEdit = false;
        if (m_selectedEquip >= m_selectedActor->equips.size()) {
          m_selectedActor->equips.resize(m_selectedEquip + 1);
        }
        m_selectedActor->equips[m_selectedEquip] = m_chosenEquip;
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        m_showEquipEdit = false;
      }
      ImGui::End();
    }
  }
}