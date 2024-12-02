#include "Core/DatabaseEditor/DBItemsTab.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Items.hpp"
#include "imgui.h"
#include "imgui_internal.h"

DBItemsTab::DBItemsTab(Items& Items, DatabaseEditor* parent) : IDBEditorTab(parent), m_items(Items) {
  m_selectedItem = m_items.item(1);
  if (m_selectedItem) {
    m_effectsEditor.setEffects(&m_selectedItem->effects);
  }
}

void DBItemsTab::draw() {
  if (!m_itemButtonTexture) {
    // m_itemButtonTexture.emplace();
    // m_itemButtonTexture->setSize(38, 38);
    // if (!m_itemSheet) {
    //   // m_itemSheet.emplace(m_parent->getIconSheet());
    // }
    // const auto& [uv0, uv1] = m_itemSheet->getRectForCharacter(m_selectedItem->iconIndex, 1);
    // const Point offset{static_cast<int>(uv0.x() * m_itemSheet->texture().width()), static_cast<int>(uv0.y() * m_itemSheet->texture().height())};
    // m_itemButtonTexture->setTexturesToComposite({{m_itemSheet->texture(), {m_itemSheet->characterWidth(), m_itemSheet->characterHeight()}, offset}});
  }
  if (m_animationPicker) {
    const auto [closed, confirmed] = m_animationPicker->draw();
    if (closed) {
      if (confirmed) {
        m_selectedItem->animationId = m_animationPicker->selection();
      }
      m_animationPicker.reset();
    }
  }

  ImGui::BeginChild("#orpg_items_editor");
  {
    ImGui::BeginChild("##orpg_items_editor_items", ImVec2{250.f, 0} * App::DPIHandler::get_ui_scale(), 0, ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Items");
        ImGui::BeginChild("##orpg_items_editor_items_list", ImVec2{0, ImGui::GetContentRegionMax().y - (App::DPIHandler::scale_value(108))});
        {
          ImGui::BeginGroup();
          {
            for (auto& skill_ : m_items.items()) {
              if (skill_.id == 0) {
                continue;
              }

              char name[4096];
              snprintf(name, 4096, "%04i %s", skill_.id, skill_.name.c_str());
              if (ImGui::Selectable(name, &skill_ == m_selectedItem) || (ImGui::IsItemFocused() && m_selectedItem != &skill_)) {
                m_selectedItem = &skill_;
                // m_traitsEditor.setTraits(&m_selectedClass->traits);
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Items %i", m_items.count());
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - (App::DPIHandler::scale_value(8)), 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxItems = m_items.count();
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_items_editor_items_item_properties");
    {
      if (m_selectedItem) {
        ImGui::BeginChild("##orpg_items_item_panel_left", ImVec2{ImGui::GetContentRegionMax().x / 2, 0.f});
        {
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("General Settings");
            // Name
            ImGui::BeginGroup();
            {
              char name[4096];
              strncpy(name, m_selectedItem->name.c_str(), 4096);
              if (ImGui::LabelOverLineEdit("##orpg_items_editor_name", "Name:", name, 4096, (ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(16))) {
                m_selectedItem->name = name;
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            // Icon
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::TextUnformatted("Icon:");
              // if (ImGui::ImageButtonEx(ImGui::GetID("##orpg_database_items_image"), m_itemButtonTexture->get(), ImVec2(m_itemButtonTexture->size()) * App::DPIHandler::get_ui_scale(), {0.f, 0.f},
              //                          {1.f, 1.f}, {}, {1.f, 1.f, 1.f, 1.f}, ImGuiButtonFlags_PressedOnDoubleClick)) {
              //   m_currentSheet = &m_itemSheet.value();
              //   m_characterPicker->setCharacterInfo("", m_selectedItem->iconIndex);
              //   m_characterPicker->setOpen(true);
              // }
              ImGui::SameLine();
              ImGui::SetCursorPos(ImVec2{ImGui::GetCursorPosX() + 10.f, ImGui::GetCursorPosY() - 6.f});
              ImGui::BeginGroup();
              {
                ImGui::Text("Consumable");
                ImGui::Checkbox("##orpg_database_items_consumable", &m_selectedItem->consumable);
              }
              ImGui::EndGroup();
            }
            ImGui::EndGroup();
            // Description
            ImGui::BeginGroup();
            {
              char description[4096];
              strncpy(description, m_selectedItem->description.c_str(), 4096);
              ImGui::Text("Description:");
              ImGui::InputTextMultiline("##orpg_database_items_description", description, 4096, ImVec2{App::DPIHandler::scale_value(360), App::DPIHandler::scale_value(60)});
            }
            ImGui::EndGroup();
            // Item Type, Price, Consumable
            ImGui::BeginGroup();
            {
              ImGui::Text("Item Type:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::BeginCombo("##orpg_database_items_itype", DecodeEnumName(magic_enum::enum_name(static_cast<ItemType>(m_selectedItem->itypeId))).c_str())) {
                int index{0};
                for (auto v : magic_enum::enum_values<ItemType>()) {
                  if (ImGui::Selectable(DecodeEnumName(v).c_str(), static_cast<ItemType>(m_selectedItem->itypeId) == v)) {
                    m_selectedItem->itypeId = static_cast<int>(v);
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Price:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_items_price", &m_selectedItem->price, 1, 100)) {
                if (m_selectedItem->price < 0)
                  m_selectedItem->price = 0;
                if (m_selectedItem->price > 999999)
                  m_selectedItem->price = 999999;
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
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::BeginCombo("##orpg_database_items_scopelist", DecodeEnumName(magic_enum::enum_name(m_selectedItem->scope)).c_str())) {
                int index{0};
                for (auto& dir : magic_enum::enum_values<Scope>()) {
                  bool is_selected = m_selectedItem->scope == static_cast<Scope>(magic_enum::enum_index(dir).value());
                  if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                    m_selectedItem->scope = static_cast<Scope>(magic_enum::enum_index(dir).value());
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
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::BeginCombo("##orpg_database_items_occasionlist", DecodeEnumName(magic_enum::enum_name(m_selectedItem->occasion)).c_str())) {
                int index{0};
                for (auto& dir : magic_enum::enum_values<Occasion>()) {
                  bool is_selected = m_selectedItem->occasion == static_cast<Occasion>(magic_enum::enum_index(dir).value());
                  if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                    m_selectedItem->occasion = static_cast<Occasion>(magic_enum::enum_index(dir).value());
                    if (is_selected)
                      ImGui::SetItemDefaultFocus();
                  }
                  index++;
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndGroup();
            ImGui::SeparatorText("Invocation");
            ImGui::BeginGroup();
            {
              ImGui::BeginGroup();
              {
                ImGui::Text("Speed:");
                ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
                if (ImGui::InputInt("##orpg_database_items_speed", &m_selectedItem->speed, 1, 100)) {
                  if (m_selectedItem->speed < -2000)
                    m_selectedItem->speed = -2000;
                  if (m_selectedItem->speed > 2000)
                    m_selectedItem->speed = 2000;
                }
              }
              ImGui::EndGroup();
              ImGui::SameLine();
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
              ImGui::BeginGroup();
              {
                ImGui::Text("Success:");
                ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
                if (ImGui::InputInt("##orpg_database_items_successRate", &m_selectedItem->successRate, 1, 100)) {
                  if (m_selectedItem->successRate < 0)
                    m_selectedItem->successRate = 0;
                  if (m_selectedItem->successRate > 100)
                    m_selectedItem->successRate = 100;
                }
              }
              ImGui::EndGroup();
              ImGui::BeginGroup();
              {
                ImGui::Text("Repeat:");
                ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
                if (ImGui::InputInt("##orpg_database_items_repeats", &m_selectedItem->repeats, 1, 100)) {
                  if (m_selectedItem->repeats < 1)
                    m_selectedItem->repeats = 1;
                  if (m_selectedItem->repeats > 9)
                    m_selectedItem->repeats = 9;
                }
              }
              ImGui::EndGroup();
              ImGui::SameLine();
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
              ImGui::BeginGroup();
              {
                ImGui::Text("TP Gain:");
                ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
                if (ImGui::InputInt("##orpg_database_items_tpGain", &m_selectedItem->tpGain, 1, 100)) {
                  if (m_selectedItem->tpGain < 0)
                    m_selectedItem->tpGain = 0;
                  if (m_selectedItem->tpGain > 100)
                    m_selectedItem->tpGain = 100;
                }
              }
              ImGui::EndGroup();
              ImGui::BeginGroup();
              {
                ImGui::Text("Hit Type:");
                ImGui::SetNextItemWidth(App::DPIHandler::scale_value(160));
                if (ImGui::BeginCombo("##orpg_database_items_hitType", DecodeEnumName(magic_enum::enum_name(m_selectedItem->hitType)).c_str())) {
                  int index{0};
                  for (auto& dir : magic_enum::enum_values<ItemType>()) {
                    bool is_selected = m_selectedItem->hitType == static_cast<ItemType>(magic_enum::enum_index(dir).value());
                    if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                      m_selectedItem->hitType = static_cast<ItemType>(magic_enum::enum_index(dir).value());
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
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4.f);
              ImGui::BeginGroup();
              {
                ImGui::Text("Animation:");
                ImGui::SetNextItemWidth(App::DPIHandler::scale_value(160));
                // Animation Button
                ImGui::PushID("##orpg_database_items_animation");
                if (ImGui::Button(m_selectedItem->animationId == -1  ? "Normal Attack"
                                  : m_selectedItem->animationId == 0 ? "None"
                                                                     : Database::instance()->animationName(m_selectedItem->animationId).c_str(),
                                  ImVec2{200 - (15 * App::DPIHandler::get_ui_scale()), 0})) {
                  m_animationPicker = ObjectPicker("Animation"sv, Database::instance()->animations.animations(), m_selectedItem->animationId);
                  m_animationPicker->setOpen(true);
                }
                ImGui::PopID();
              }
              ImGui::EndGroup();
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("####orpg_items_item_panel_right");
        {
          m_effectsEditor.draw(m_parent);
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Note:");
            char note[8192];
            strncpy(note, m_selectedItem->note.c_str(), IM_ARRAYSIZE(note));
            if (ImGui::InputTextMultiline("##orpg_database_items_note", note, IM_ARRAYSIZE(note),
                                          ImVec2{ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(16), ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(16)})) {
              m_selectedItem->note = note;
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
    if (ImGui::Begin("Change Max Items", &m_changeIntDialogOpen,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
      ImGui::Text(
          "Specify an amount to resize the item list to\n"
          "This cannot be undone!");
      ImGui::InputInt("##value_input", &m_editMaxItems);
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
          int tmpId = m_selectedItem->id;
          m_items.resize(m_editMaxItems);
          m_selectedItem = m_items.item(tmpId);
          m_changeIntDialogOpen = false;
          m_changeConfirmDialogOpen = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          m_editMaxItems = m_items.count();
          m_changeIntDialogOpen = false;
          m_changeConfirmDialogOpen = false;
        }
      }
      ImGui::End();
    }
  }
}