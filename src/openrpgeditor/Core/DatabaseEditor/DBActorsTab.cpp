#include "Core/DatabaseEditor/DBActorsTab.hpp"
#include "Database/Class.hpp"

#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor.hpp"
#include "Core/Graphics/SideViewBattlerSheet.hpp"
#include "Core/ImGuiExt/ImGuiParsedText.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"
#include "imgui_internal.h"

DBActorsTab::DBActorsTab(Actors& actors, DatabaseEditor* parent) : IDBEditorTab(parent), m_actors(actors) {
  m_selectedActor = m_actors.actor(1);
  if (m_selectedActor) {
    m_traitsEditor.setTraits(&m_selectedActor->traits());
  }
}

void DBActorsTab::draw() {
  if (m_selectedActor != nullptr && !m_characterSheet && !m_battlerSheet && !m_faceSheet) {
    m_faceSheet.emplace(m_selectedActor->faceName());
    m_characterSheet.emplace(m_selectedActor->characterName());
    m_battlerSheet.emplace(m_selectedActor->battlerName());
  }

  if (!m_faceButton) {
    m_faceButton.emplace();
    m_faceButton->setSize(144, 144);
  }

  if (!m_actorButton) {
    m_actorButton.emplace();
    m_actorButton->setSize(144, 144);
  }
  if (!m_battlerButton) {
    m_battlerButton.emplace();
    m_battlerButton->setSize(144, 144);
  }

  if (m_selectedActor) {
    if (!m_faceButton->hasCompositeTextures()) {
      const auto [min, max] = m_faceSheet->getFaceRect(m_selectedActor->faceIndex());
      m_faceButton->setTexturesToComposite({{m_faceSheet->texture(),
                                             {m_faceSheet->faceWidth(), m_faceSheet->faceWidth()},
                                             {static_cast<int>(min.x() * m_faceSheet->texture().width()), static_cast<int>(min.y() * m_faceSheet->texture().height())}}});
    }
    if (!m_actorButton->hasCompositeTextures()) {
      const auto [min, max] = m_characterSheet->getRectForCharacter(m_selectedActor->characterIndex(), 1);
      m_actorButton->setTexturesToComposite({{m_characterSheet->texture(),
                                              {m_characterSheet->characterWidth(), m_characterSheet->characterHeight()},
                                              {static_cast<int>(min.x() * m_characterSheet->texture().width()), static_cast<int>(min.y() * m_characterSheet->texture().height())}}});
    }
    if (!m_battlerButton->hasCompositeTextures()) {
      const auto [_, frames] = m_battlerSheet->getAction(SideViewActionType::StepForward);
      m_battlerButton->setTexturesToComposite({{m_battlerSheet->texture(),
                                                {m_battlerSheet->characterWidth(), m_battlerSheet->characterHeight()},
                                                {static_cast<int>(frames[1].min.x() * m_battlerSheet->texture().width()), static_cast<int>(frames[1].min.y() * m_battlerSheet->texture().height())}}});
    }
  }

  ImGui::BeginChild("##orpg_actors_editor");
  {

    const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
    ImGui::BeginChild("##orpg_actors_editor_actors", ImVec2{calc.x + (ImGui::GetStyle().ItemSpacing.x * 2), 0}, 0, ImGuiWindowFlags_NoScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Actors");
        ImGui::BeginChild("##orpg_actors_editor_actor_list", ImVec2{0, ImGui::GetContentRegionMax().y - ((calc.y + ImGui::GetStyle().ItemSpacing.y) * 4)});
        {
          ImGui::BeginGroup();
          {
            for (auto& actor : m_actors.m_actors) {
              if (actor.id() == 0) {
                continue;
              }

              if (ImGui::Selectable(std::format("{}{}", Database::instance()->actorNameAndId(actor.id()), actor.isModified() ? "*" : "").c_str(), &actor == m_selectedActor,
                                    ImGuiSelectableFlags_SelectOnNav)) {
                if (m_selectedActor != &actor) {
                  m_selectedActor = &actor;
                  m_faceSheet.emplace(m_selectedActor->faceName());
                  m_characterSheet.emplace(m_selectedActor->characterName());
                  m_battlerSheet.emplace(m_selectedActor->battlerName());
                  m_traitsEditor.setTraits(&m_selectedActor->traits());
                  m_faceButton->clear();
                  m_actorButton->clear();
                  m_battlerButton->clear();
                }
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Actors %i", m_actors.count());
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
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
            strncpy(name, m_selectedActor->name().c_str(), 4096);
            ImGui::LabelOverLineEdit("##orpg_actors_editor_actors_actor_name", "Name:", name, 4096, ImGui::GetContentRegionMax().x / 2 - 16);
            if (ImGui::IsItemDeactivatedAfterEdit()) {
              m_selectedActor->setName(name);
            }
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
            strncpy(name, m_selectedActor->nickname().c_str(), 4096);
            ImGui::LabelOverLineEdit("##orpg_actors_editor_actors_actor_nickname", "Nickname:", name, 4096, ImGui::GetContentRegionMax().x / 2 - 16);
            if (ImGui::IsItemDeactivatedAfterEdit()) {
              m_selectedActor->setNickname(name);
            }
            ImGui::BeginGroup();
            {
              ImGui::Text("Class:");
              Classes classes = m_parent->classes();
              const auto cls = classes.classType(m_selectedActor->classId());
              ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x / 2 - 16);
              if (ImGui::BeginCombo("##orpg_actors_editor_class_combo", Database::instance()->classNameAndId(cls->id()).c_str())) {
                for (const auto& c : classes.classes()) {
                  if (c.id() == 0) {
                    continue;
                  }
                  if (ImGui::Selectable(Database::instance()->classNameAndId(c.id()).c_str(), c.id() == m_selectedActor->classId())) {
                    m_selectedActor->setClassId(c.id());
                    m_selectedActor->setValid(true);
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
              ImGui::SetNextItemWidth(ImGui::GetCursorPosX() / 2 - 16);
              int initialLevel = m_selectedActor->initialLevel();
              ImGui::InputInt("##orpg_actors_initial_level_edit", &initialLevel);
              if (ImGui::IsItemDeactivatedAfterEdit()) {
                m_selectedActor->setInitialLevel(std::clamp(initialLevel, 1, 99));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
            ImGui::BeginGroup();
            {
              ImGui::Text("Max Level:");
              ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x / 2 / 2 - 16);
              int maxLevel = m_selectedActor->maxLevel();
              ImGui::InputInt("##orpg_actors_max_level_edit", &maxLevel);
              if (ImGui::IsItemDeactivatedAfterEdit()) {
                m_selectedActor->setMaxLevel(std::clamp(maxLevel, 1, 99));
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Profile:");
              char profile[8192];
              strncpy(profile, m_selectedActor->profile().c_str(), IM_ARRAYSIZE(profile));
              if (ImGui::InputTextMultiline("##orpg_actors_profile", profile, IM_ARRAYSIZE(profile), ImVec2{ImGui::GetContentRegionMax().x - 16, 0})) {
                m_selectedActor->setProfile(profile);
              }
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::BeginChild("##orpg_actors_editor_actor_images", ImVec2(), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_HorizontalScrollbar);
          {
            ImGui::SeparatorText("Images");
            ImGui::BeginGroup();
            {
              ImGui::Text("Face:");
              if (ImGui::ImageButtonEx(ImGui::GetID("##orpg_actors_face_image"), m_faceButton->get(), ImVec2{m_faceButton->size()}, {0.f, 0.f}, {1.f, 1.f}, {}, {1.f, 1.f, 1.f, 1.f},
                                       ImGuiButtonFlags_PressedOnDoubleClick)) {}
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            {
              ImGui::Text("Character:");
              if (ImGui::ImageButtonEx(ImGui::GetID("##orpg_actors_character_image"), m_actorButton->get(), ImVec2{m_actorButton->size()}, {0.f, 0.f}, {1.f, 1.f}, {}, {1.f, 1.f, 1.f, 1.f},
                                       ImGuiButtonFlags_PressedOnDoubleClick)) {}
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            {
              ImGui::Text("[SV] Battler:");
              if (ImGui::ImageButtonEx(ImGui::GetID("##orpg_actors_battler_image"), m_battlerButton->get(), ImVec2{m_battlerButton->size()}, {0.f, 0.f}, {1.f, 1.f}, {}, {1.f, 1.f, 1.f, 1.f},
                                       ImGuiButtonFlags_PressedOnDoubleClick)) {}
            }
            ImGui::EndGroup();
          }
          ImGui::EndChild();
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Initial Equipment");
            if (ImGui::BeginTable("##orpg_actors_actor_init_equip", 2,
                                  ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                                  ImVec2{ImGui::GetContentRegionMax().x - 15, ImGui::GetContentRegionAvail().y - 16})) {

              ImGui::TableSetupColumn("Type");
              ImGui::TableSetupColumn("Equipment Item");
              ImGui::TableHeadersRow();
              const auto& equipTypes = Database::instance()->system.equipTypes;

              for (int i = 0; i < equipTypes.size() - 1; i++) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                const int etypeId = Database::instance()->slotIdToEquipId(m_selectedActor->id(), i);
                auto etypeName = equipTypes[etypeId];
                int dataId = i < m_selectedActor->equips().size() ? m_selectedActor->equips()[i] : 0;
                if (!checkEquipable(etypeId, dataId)) {
                  dataId = 0;
                }

                if (ImGui::Selectable(etypeName.c_str(), m_selectedEquip == i, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
                  m_selectedEquip = i;
                  if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                    m_showEquipEdit = true;
                    m_chosenEquip = m_selectedEquip < m_selectedActor->equips().size() ? m_selectedActor->equips()[m_selectedEquip] : 0;
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
            strncpy(note, m_selectedActor->note().c_str(), IM_ARRAYSIZE(note));
            if (ImGui::InputTextMultiline("##orpg_actors_note", note, IM_ARRAYSIZE(note), ImVec2{ImGui::GetContentRegionMax().x - 16, ImGui::GetContentRegionAvail().y - 16})) {
              m_selectedActor->setNote(note);
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
          const int tmpId = m_selectedActor->id();
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
        if (m_selectedEquip >= m_selectedActor->equips().size()) {
          m_selectedActor->addEquip(m_chosenEquip);
        } else {
          m_selectedActor->setEquip(m_selectedEquip, m_chosenEquip);
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          m_showEquipEdit = false;
        }
        ImGui::End();
      }
    }
  }
}

bool DBActorsTab::checkEquipable(const int etypeId, const int dataId) const {
  if (dataId <= 0 || Database::instance()->isEquipTypeSealed(m_selectedActor->id(), etypeId)) {
    return false;
  }

  if (etypeId <= 1) {
    if (const auto& weapon = Database::instance()->weapons.weapon(dataId); weapon && Database::instance()->isEquipWeaponTypeOk(m_selectedActor->id(), weapon->wtypeId())) {
      return true;
    }
  } else if (const auto& armor = Database::instance()->armors.armor(dataId);
             armor && armor->etypeId() == etypeId && Database::instance()->isEquipArmorTypeOk(m_selectedActor->id(), armor->atypeId())) {
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
