#include "Core/DatabaseEditor/DBActorsTab.hpp"
#include "Database/Actors.hpp"

#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/DatabaseEditor.hpp"
#include "Core/Graphics/SideViewBattlerSheet.hpp"
#include "Core/ImGuiExt/ImGuiParsedText.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"

DBActorsTab::DBActorsTab(Actors& actors, DatabaseEditor* parent) : IDBEditorTab(parent), m_actors(actors) {
  m_selectedActor = m_actors.actor(1);
  if (m_selectedActor) {
    m_traitsEditor.setTraits(&m_selectedActor->traits);
  }
}

void DBActorsTab::draw() {
  if (m_selectedActor != nullptr && !m_characterSheet && !m_battlerSheet && !m_faceSheet) {
    m_faceSheet.emplace(m_selectedActor->faceName);
    m_characterSheet.emplace(m_selectedActor->characterName);
    m_battlerSheet.emplace(m_selectedActor->battlerName);
  }

  ImGui::BeginChild("##orpg_actors_editor");
  {
    ImGui::BeginChild("##orpg_actors_editor_actors", ImVec2{250.f, 0} * App::DPIHandler::get_ui_scale(), 0, ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Actors");
        ImGui::BeginChild("##orpg_actors_editor_actor_list", ImVec2{0, ImGui::GetContentRegionMax().y - (App::DPIHandler::scale_value(108))});
        {
          ImGui::BeginGroup();
          {
            for (auto& actor : m_actors.m_actors) {
              if (actor.id == 0) {
                continue;
              }

              char name[4096];
              snprintf(name, 4096, "%04i %s", actor.id, actor.name.c_str());
              if (ImGui::Selectable(name, &actor == m_selectedActor) || (ImGui::IsItemFocused() && m_selectedActor != &actor)) {
                m_selectedActor = &actor;
                m_faceSheet.emplace(m_selectedActor->faceName);
                m_characterSheet.emplace(m_selectedActor->characterName);
                m_battlerSheet.emplace(m_selectedActor->battlerName);
                m_traitsEditor.setTraits(&m_selectedActor->traits);
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Actors %i", m_actors.count());
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - (App::DPIHandler::scale_value(8)), 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxActors = m_actors.count();
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
            if (ImGui::LabelOverLineEdit("##orpg_actors_editor_actors_actor_name", "Name:", name, 4096, (ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(16))) {
              m_selectedActor->name = name;
            }
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
            strncpy(name, m_selectedActor->nickname.c_str(), 4096);
            if (ImGui::LabelOverLineEdit("##orpg_actors_editor_actors_actor_nickname", "Nickname:", name, 4096, (ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(16))) {
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
                    m_selectedActor->m_isValid = true;
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
              ImGui::SetNextItemWidth((ImGui::GetCursorPosX() / 2) - App::DPIHandler::scale_value(16));
              ImGui::InputInt("##orpg_actors_initial_level_edit", &m_selectedActor->initialLevel);
              m_selectedActor->initialLevel = std::clamp(m_selectedActor->initialLevel, 1, 99);
              m_selectedActor->m_isValid = true;
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
            ImGui::BeginGroup();
            {
              ImGui::Text("Max Level:");
              ImGui::SetNextItemWidth(((ImGui::GetContentRegionMax().x / 2) / 2) - App::DPIHandler::scale_value(16));
              ImGui::InputInt("##orpg_actors_max_level_edit", &m_selectedActor->maxLevel);
              m_selectedActor->maxLevel = std::clamp(m_selectedActor->maxLevel, 1, 99);
              m_selectedActor->m_isValid = true;
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Profile:");
              char profile[8192];
              strncpy(profile, m_selectedActor->profile.c_str(), IM_ARRAYSIZE(profile));
              if (ImGui::InputTextMultiline("##orpg_actors_profile", profile, IM_ARRAYSIZE(profile), ImVec2{ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(16), 0})) {
                m_selectedActor->profile = profile;
                m_selectedActor->m_isValid = true;
              }
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::BeginChild("##orpg_actors_editor_actor_images", ImVec2(), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_HorizontalScrollbar);
          {
            ImGui::SeparatorText("Images");
            const auto buttonSize = ImVec2{144, 144} * App::DPIHandler::get_ui_scale();
            const auto buttonCenter = (buttonSize / 2);
            ImGui::BeginGroup();
            {
              ImGui::Text("Face:");
              auto cursorPos = ImGui::GetCursorPos();
              ImGui::ImageButton("##orpg_actors_face_image", m_buttonBack, buttonSize);
              if (m_faceSheet && m_faceSheet->texture()) {
                const auto faceRect = ImVec2{static_cast<float>(m_faceSheet->faceWidth()), static_cast<float>(m_faceSheet->faceHeight())} * App::DPIHandler::get_ui_scale();
                ImGui::SetCursorPos(((cursorPos + buttonCenter) - (faceRect / 2)) + (ImGui::GetStyle().ItemInnerSpacing - ImVec2{0.f, App::DPIHandler::scale_value(1.f)}));
                const auto rect = m_faceSheet->getFaceRect(m_selectedActor->faceIndex);
                ImVec2 uv0{rect.u0, rect.v0};
                ImVec2 uv1{rect.u1, rect.v1};
                ImGui::Image(m_faceSheet->texture(), faceRect, uv0, uv1);
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            {
              ImGui::Text("Character:");
              auto cursorPos = ImGui::GetCursorPos();
              ImGui::ImageButton("##orpg_actors_character_image", m_buttonBack, buttonSize);
              if (m_characterSheet && m_characterSheet->texture()) {
                const auto characterRect = ImVec2{std::ceil(static_cast<float>(m_characterSheet->characterWidth() * 1.75)), std::ceil(static_cast<float>(m_characterSheet->characterHeight() * 1.75))} *
                                           App::DPIHandler::get_ui_scale();
                ImGui::SetCursorPos((cursorPos + buttonCenter) - (characterRect / 1.75));
                const auto [min, max] = m_characterSheet->getRectForCharacter(m_selectedActor->characterIndex, 1);
                ImGui::Image(m_characterSheet->texture(), characterRect, min, max);
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            {
              ImGui::Text("[SV] Battler:");
              auto cursorPos = ImGui::GetCursorPos();
              ImGui::ImageButton("##orpg_actors_battler_image", m_buttonBack, buttonSize);
              if (m_battlerSheet && m_battlerSheet->texture()) {
                const auto battlerRect = ImVec2{static_cast<float>(m_battlerSheet->characterWidth()) * 2, static_cast<float>(m_battlerSheet->characterHeight()) * 2} * App::DPIHandler::get_ui_scale();
                ImGui::SetCursorPos((cursorPos + buttonCenter) - (battlerRect / 2));
                const auto rect = m_battlerSheet->getAction(SideViewActionType::StepForward);
                ImGui::Image(m_battlerSheet->texture(), battlerRect, rect.frames[1].min, rect.frames[1].max);
              }
            }
            ImGui::EndGroup();
          }
          ImGui::EndChild();
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Initial Equipment");
            if (ImGui::BeginTable("##orpg_actors_actor_init_equip", 2,
                                  ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                                  ImVec2{ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(15), ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(16)})) {

              ImGui::TableSetupColumn("Type");
              ImGui::TableSetupColumn("Equipment Item");
              ImGui::TableHeadersRow();
              const auto& equipTypes = Database::instance()->system.equipTypes;

              for (int i = 0; i < equipTypes.size() - 1; i++) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                int etypeId = Database::instance()->slotIdToEquipId(m_selectedActor->id, i);
                auto etypeName = equipTypes[etypeId];
                int dataId = i < m_selectedActor->equips.size() ? m_selectedActor->equips[i] : 0;
                if (!checkEquipable(etypeId, dataId)) {
                  dataId = 0;
                }

                if (ImGui::Selectable(etypeName.c_str(), m_selectedEquip == i, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
                  m_selectedEquip = i;
                  if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                    m_showEquipEdit = true;
                    m_chosenEquip = m_selectedEquip < m_selectedActor->equips.size() ? m_selectedActor->equips[m_selectedEquip] : 0;
                  }
                }

                ImGui::TableNextColumn();
                ImGui::Text("%s", itemDisplayName(etypeId <= 1, dataId).c_str());
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
          m_traitsEditor.draw(m_parent);

          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Note:");
            char note[8192];
            strncpy(note, m_selectedActor->note.c_str(), IM_ARRAYSIZE(note));
            if (ImGui::InputTextMultiline("##orpg_actors_note", note, IM_ARRAYSIZE(note),
                                          ImVec2{ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(16), ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(16)})) {
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
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
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
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text("Are you sure?");
        if (ImGui::Button("Yes")) {
          int tmpId = m_selectedActor->id;
          m_actors.resize(m_editMaxActors);
          m_selectedActor = m_actors.actor(tmpId);
          m_changeIntDialogOpen = false;
          m_changeConfirmDialogOpen = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          m_changeIntDialogOpen = false;
          m_changeConfirmDialogOpen = false;
        }
      }
      ImGui::End();
    }
  }

  if (m_showEquipEdit) {
    if (ImGui::Begin("Select Equipment...", &m_showEquipEdit, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoCollapse)) {
      ImGui::SameLine();
      if (ImGui::Button("OK")) {
        m_showEquipEdit = false;
        if (m_selectedEquip >= m_selectedActor->equips.size()) {
          m_selectedActor->equips.resize(m_selectedEquip + 1);
        }
        m_selectedActor->equips[m_selectedEquip] = m_chosenEquip;
        m_selectedActor->m_isValid = true;
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        m_showEquipEdit = false;
      }
      ImGui::End();
    }
  }
}

bool DBActorsTab::checkEquipable(const int etypeId, const int dataId) const {
  if (dataId <= 0 || Database::instance()->isEquipTypeSealed(m_selectedActor->id, etypeId)) {
    return false;
  }

  if (etypeId <= 1) {
    if (const auto& weapon = Database::instance()->weapons.weapon(dataId); weapon && Database::instance()->isEquipWeaponTypeOk(m_selectedActor->id, weapon->wtypeId)) {
      return true;
    }
  } else if (const auto& armor = Database::instance()->armors.armor(dataId); armor && armor->etypeId == etypeId && Database::instance()->isEquipArmorTypeOk(m_selectedActor->id, armor->atypeId)) {
    return true;
  }

  return false;
}

std::string DBActorsTab::itemDisplayName(const bool isWeapon, const int dataId) {
  if (dataId <= 0) {
    return "None";
  }
  if (isWeapon) {
    return Database::instance()->weaponNameOrId(dataId);
  }

  return Database::instance()->armorNameOrId(dataId);
}
