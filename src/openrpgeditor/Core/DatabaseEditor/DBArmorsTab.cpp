#include "Core/DatabaseEditor/DBArmorsTab.hpp"
#include "Core/Application.hpp"
#include "Database/Armors.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"
#include "imgui_internal.h"

DBArmorsTab::DBArmorsTab(Armors& Armors, DatabaseEditor* parent) : IDBEditorTab(parent), m_armors(Armors) {
  m_selectedArmor = m_armors.armor(1);
  if (m_selectedArmor) {
    m_traitsEditor.setTraits(&m_selectedArmor->traits);
  }
}

void DBArmorsTab::draw() {
  if (!m_itemButtonTexture) {
    m_itemButtonTexture.emplace();
    m_itemButtonTexture->setSize(38, 38);
    if (!m_itemSheet) {
      m_itemSheet.emplace(*m_parent->getIconSheet());
    }
    const auto& [uv0, uv1] = m_itemSheet.value().rectForId(m_selectedArmor->iconIndex);
    const Point offset{static_cast<int>(uv0.x() * m_itemSheet.value().texture().width()), static_cast<int>(uv0.y() * m_itemSheet.value().texture().height())};
    m_itemButtonTexture->setTexturesToComposite({{m_itemSheet.value().texture(), {m_itemSheet.value().iconWidth(), m_itemSheet.value().iconHeight()}, offset}});
  }
  ImGui::BeginChild("#orpg_armors_editor");
  {
    ImGui::BeginChild("##orpg_armors_editor_armors", ImVec2{250.f, 0} * App::DPIHandler::get_ui_scale(), 0, ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Armors");
        ImGui::BeginChild("##orpg_armors_editor_armors_list", ImVec2{0, ImGui::GetContentRegionMax().y - (App::DPIHandler::scale_value(108))});
        {
          ImGui::BeginGroup();
          {
            for (auto& skill_ : m_armors.armors()) {
              if (skill_.id == 0) {
                continue;
              }

              char name[4096];
              snprintf(name, 4096, "%04i %s", skill_.id, skill_.name.c_str());
              if (ImGui::Selectable(name, &skill_ == m_selectedArmor) || (ImGui::IsItemFocused() && m_selectedArmor != &skill_)) {
                m_selectedArmor = &skill_;
                m_traitsEditor.setTraits(&m_selectedArmor->traits);
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Armors %i", m_armors.count());
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - (App::DPIHandler::scale_value(8)), 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxArmors = m_armors.count();
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_armors_editor_armors_armor_properties");
    {
      if (m_selectedArmor) {
        ImGui::BeginChild("##orpg_armors_armor_panel_left", ImVec2{ImGui::GetContentRegionMax().x / 2, 0.f});
        {
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("General Settings");
            // Name
            ImGui::BeginGroup();
            {
              char name[4096];
              strncpy(name, m_selectedArmor->name.c_str(), 4096);
              if (ImGui::LabelOverLineEdit("##orpg_armors_editor_name", "Name:", name, 4096, (ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(16))) {
                m_selectedArmor->name = name;
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            // Icon
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::TextUnformatted("Icon:");
              if (ImGui::ImageButtonEx(ImGui::GetID("##orpg_database_armors_image"), m_itemButtonTexture->get(), ImVec2(m_itemButtonTexture->size()) * App::DPIHandler::get_ui_scale(), {0.f, 0.f},
                                       {1.f, 1.f}, {}, {1.f, 1.f, 1.f, 1.f}, ImGuiButtonFlags_PressedOnDoubleClick)) {
                m_currentSheet = &m_itemSheet.value();
                // m_characterPicker->setCharacterInfo("", m_selectedItem->iconIndex);
                // m_characterPicker->setOpen(true);
              }
            }
            ImGui::EndGroup();
            // Description
            ImGui::BeginGroup();
            {
              char description[4096];
              strncpy(description, m_selectedArmor->description.c_str(), 4096);
              ImGui::Text("Description:");
              ImGui::InputTextMultiline("##orpg_database_armors_description", description, 4096, ImVec2{App::DPIHandler::scale_value(360), App::DPIHandler::scale_value(60)});
            }
            ImGui::EndGroup();
            // Weapon Type
            ImGui::BeginGroup();
            {
              ImGui::Text("Armor Type:");
              // TODO: Needs to be armor type
              // ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              // if (ImGui::BeginCombo("##orpg_database_items_itype", DecodeEnumName(magic_enum::enum_name(static_cast<ItemType>(m_selectedItem->itypeId))).c_str())) {
              //  int index{0};
              //  for (auto v : magic_enum::enum_values<ItemType>()) {
              //    if (ImGui::Selectable(DecodeEnumName(v).c_str(), static_cast<ItemType>(m_selectedItem->itypeId) == v)) {
              //      m_selectedItem->itypeId = static_cast<int>(v);
              //    }
              //  }
              //  ImGui::EndCombo();
              //}
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            // Price
            ImGui::BeginGroup();
            {
              ImGui::Text("Price:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_armors_price", &m_selectedArmor->price, 1, 100)) {
                if (m_selectedArmor->price < 0)
                  m_selectedArmor->price = 0;
                if (m_selectedArmor->price > 999999)
                  m_selectedArmor->price = 999999;
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Equipment Type:");
              // ImGui::SetNextItemWidth(App::DPIHandler::scale_value(160));
              ImGui::PushID("##orpg_database_armors_equipmenttype");
              // TODO: Equipment Type combobox
              ImGui::PopID();
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::EndGroup();
            ImGui::SeparatorText("Parameter Changes");
            ImGui::BeginGroup();
            {
              ImGui::Text("Attack:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_armor_attack", &m_selectedArmor->params.at(0), 1, 100)) {
                if (m_selectedArmor->params.at(0) < -999)
                  m_selectedArmor->params.at(0) = -999;
                if (m_selectedArmor->params.at(0) > 999)
                  m_selectedArmor->params.at(0) = 999;
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Defense:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_armor_defense", &m_selectedArmor->params.at(1), 1, 100)) {
                if (m_selectedArmor->params.at(1) < -999)
                  m_selectedArmor->params.at(1) = -999;
                if (m_selectedArmor->params.at(1) > 999)
                  m_selectedArmor->params.at(1) = 999;
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("M.Attack:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_armor_mattack", &m_selectedArmor->params.at(2), 1, 100)) {
                if (m_selectedArmor->params.at(2) < -999)
                  m_selectedArmor->params.at(2) = -999;
                if (m_selectedArmor->params.at(2) > 999)
                  m_selectedArmor->params.at(2) = 999;
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("M.Defense:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_armor_mdefense", &m_selectedArmor->params.at(3), 1, 100)) {
                if (m_selectedArmor->params.at(3) < -999)
                  m_selectedArmor->params.at(3) = -999;
                if (m_selectedArmor->params.at(3) > 999)
                  m_selectedArmor->params.at(3) = 999;
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Agility:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_armor_agility", &m_selectedArmor->params.at(4), 1, 100)) {
                if (m_selectedArmor->params.at(4) < -999)
                  m_selectedArmor->params.at(4) = -999;
                if (m_selectedArmor->params.at(4) > 999)
                  m_selectedArmor->params.at(4) = 999;
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Luck:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_armor_luck", &m_selectedArmor->params.at(5), 1, 100)) {
                if (m_selectedArmor->params.at(5) < -999)
                  m_selectedArmor->params.at(5) = -999;
                if (m_selectedArmor->params.at(5) > 999)
                  m_selectedArmor->params.at(5) = 999;
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Max HP:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_armor_maxhp", &m_selectedArmor->params.at(6), 1, 100)) {
                if (m_selectedArmor->params.at(6) < -9999)
                  m_selectedArmor->params.at(6) = -9999;
                if (m_selectedArmor->params.at(6) > 9999)
                  m_selectedArmor->params.at(6) = 9999;
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Max MP:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_armor_maxmp", &m_selectedArmor->params.at(7), 1, 100)) {
                if (m_selectedArmor->params.at(7) < -9999)
                  m_selectedArmor->params.at(7) = -9999;
                if (m_selectedArmor->params.at(7) > 9999)
                  m_selectedArmor->params.at(7) = 9999;
              }
            }
            ImGui::EndGroup();
          }
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##orpg_weapons_armor_panel_right");
        {

          m_traitsEditor.draw(m_parent);
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Note:");
            char note[8192];
            strncpy(note, m_selectedArmor->note.c_str(), IM_ARRAYSIZE(note));
            if (ImGui::InputTextMultiline("##orpg_database_armors_note", note, IM_ARRAYSIZE(note),
                                          ImVec2{ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(16), ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(16)})) {
              m_selectedArmor->note = note;
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
      }
      ImGui::EndChild();
    }
    ImGui::EndChild();

    if (m_changeIntDialogOpen) {
      if (ImGui::Begin("Change Max Armors", &m_changeIntDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text(
            "Specify an amount to resize the armor list to\n"
            "This cannot be undone!");
        ImGui::InputInt("##value_input", &m_editMaxArmors);
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
            int tmpId = m_selectedArmor->id;
            m_armors.resize(m_editMaxArmors);
            m_selectedArmor = m_armors.armor(tmpId);
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
  }
}