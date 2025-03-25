#include "Core/DatabaseEditor/DBItemsTab.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Items.hpp"
#include "imgui.h"
#include "imgui_internal.h"

DBItemsTab::DBItemsTab(Items& items, DatabaseEditor* parent) : IDBEditorTab(parent), m_items(items) {
  m_selectedItem = m_items.item(1);
  if (m_selectedItem) {
    m_effectsEditor.setEffects(&m_selectedItem->effects());
  }
}

void DBItemsTab::draw() {
  if (!m_itemButtonTexture) {
    m_itemButtonTexture.emplace();
    m_itemButtonTexture->setSize(48, 48);
    if (!m_itemSheet) {
      m_itemSheet.emplace(*m_parent->getIconSheet());
    }
  }

  if (m_selectedItem && !m_itemButtonTexture->hasCompositeTextures()) {
    const auto& [uv0, uv1] = m_itemSheet.value().rectForId(m_selectedItem->iconIndex());
    const Point offset{static_cast<int>(uv0.x() * m_itemSheet.value().texture().width()), static_cast<int>(uv0.y() * m_itemSheet.value().texture().height())};
    m_itemButtonTexture->setTexturesToComposite({{m_itemSheet.value().texture(), {m_itemSheet.value().iconWidth(), m_itemSheet.value().iconHeight()}, offset}});
  }

  if (m_animationPicker) {
    if (const auto [closed, confirmed] = m_animationPicker->draw(); closed) {
      if (confirmed) {
        m_selectedItem->setAnimationId(m_animationPicker->selection());
      }
      m_animationPicker.reset();
    }
  }

  ImGui::BeginChild("#orpg_items_editor");
  {
    const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
    ImGui::BeginChild("##orpg_items_editor_items", ImVec2{calc.x + (ImGui::GetStyle().ItemSpacing.x * 2), 0}, 0, ImGuiWindowFlags_NoScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Items");
        ImGui::BeginChild("##orpg_items_editor_items_list", ImVec2{0, ImGui::GetContentRegionMax().y - ((calc.y + ImGui::GetStyle().ItemSpacing.y) * 4)});
        {
          ImGui::BeginGroup();
          {
            for (auto& skill_ : m_items.items()) {
              if (skill_.id() == 0) {
                continue;
              }

              char name[4096];
              snprintf(name, 4096, "%04i %s", skill_.id(), skill_.name().c_str());
              if (ImGui::Selectable(name, &skill_ == m_selectedItem) || (ImGui::IsItemFocused() && m_selectedItem != &skill_)) {
                if (m_selectedItem != &skill_) {ImGui::ClearActiveID();
                  m_selectedItem = &skill_;
                  m_effectsEditor.setEffects(&m_selectedItem->effects());
                  m_itemButtonTexture->clear();
                }
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Items %i", m_items.count());
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
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
              strncpy(name, m_selectedItem->name().c_str(), 4096);
              if (ImGui::LabelOverLineEdit("##orpg_items_editor_name", "Name:", name, 4096, ImGui::GetContentRegionMax().x / 2 - 16,nullptr, ImGuiInputTextFlags_None)) {
                m_selectedItem->setName(name);
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            // Icon
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::TextUnformatted("Icon:");
              if (ImGui::ImageButtonEx(ImGui::GetID("##orpg_database_items_image"), m_itemButtonTexture->get(), ImVec2(m_itemButtonTexture->size()), {0.f, 0.f}, {1.f, 1.f}, {}, {1.f, 1.f, 1.f, 1.f},
                                       ImGuiButtonFlags_PressedOnDoubleClick)) {
                m_currentSheet = &m_itemSheet.value();
                // m_characterPicker->setCharacterInfo("", m_selectedItem->iconIndex);
                // m_characterPicker->setOpen(true);
              }
              ImGui::SameLine();
              ImGui::SetCursorPos(ImVec2{ImGui::GetCursorPosX() + 10.f, ImGui::GetCursorPosY() - 6.f});
              ImGui::BeginGroup();
              {
                ImGui::Text("Consumable");
                bool consumable = m_selectedItem->consumable();
                if (ImGui::Checkbox("##orpg_database_items_consumable", &consumable)) {
                  m_selectedItem->setConsumable(consumable);
                }
              }
              ImGui::EndGroup();
            }
            ImGui::EndGroup();
            // Description
            ImGui::BeginGroup();
            {
              char description[4096];
              strncpy(description, m_selectedItem->description().c_str(), 4096);
              ImGui::Text("Description:");
              ImGui::InputTextMultiline("##orpg_database_items_description", description, 4096, ImVec2{360, 60});
            }
            ImGui::EndGroup();
            // Item Type, Price, Consumable
            ImGui::BeginGroup();
            {
              ImGui::Text("Item Type:");
              ImGui::SetNextItemWidth(170);
              if (ImGui::BeginCombo("##orpg_database_items_itype", DecodeEnumName(magic_enum::enum_name(static_cast<ItemType>(m_selectedItem->itypeId()))).c_str())) {
                for (auto v : magic_enum::enum_values<ItemType>()) {
                  if (ImGui::Selectable(DecodeEnumName(v).c_str(), static_cast<ItemType>(m_selectedItem->itypeId()) == v)) {
                    m_selectedItem->setItypeId(static_cast<int>(v));
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
              ImGui::SetNextItemWidth(170);
              int price = m_selectedItem->price();
              if (ImGui::InputInt("##orpg_database_items_price", &price, 1, 100)) {
                m_selectedItem->setPrice(std::clamp(price, 0, 999999));
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
              ImGui::SetNextItemWidth(170);
              if (ImGui::BeginCombo("##orpg_database_items_scopelist", DecodeEnumName(magic_enum::enum_name(m_selectedItem->scope())).c_str())) {
                int index{0};
                for (auto& dir : magic_enum::enum_values<Scope>()) {
                  if (const bool is_selected = m_selectedItem->scope() == static_cast<Scope>(magic_enum::enum_index(dir).value());
                      ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                    m_selectedItem->setScope(dir);
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
              ImGui::SetNextItemWidth(170);
              if (ImGui::BeginCombo("##orpg_database_items_occasionlist", DecodeEnumName(magic_enum::enum_name(m_selectedItem->occasion())).c_str())) {
                int index{0};
                for (auto& dir : magic_enum::enum_values<Occasion>()) {
                  if (const bool is_selected = m_selectedItem->occasion() == static_cast<Occasion>(magic_enum::enum_index(dir).value());
                      ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                    m_selectedItem->setOccasion(dir);
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
                ImGui::SetNextItemWidth(170);
                int speed = m_selectedItem->speed();
                if (ImGui::InputInt("##orpg_database_items_speed", &speed, 1, 100)) {
                  m_selectedItem->setSpeed(std::clamp(speed, -2000, 2000));
                }
              }
              ImGui::EndGroup();
              ImGui::SameLine();
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
              ImGui::BeginGroup();
              {
                ImGui::Text("Success:");
                ImGui::SetNextItemWidth(170);
                int successRate = m_selectedItem->successRate();
                if (ImGui::InputInt("##orpg_database_items_successRate", &successRate, 1, 100)) {
                  m_selectedItem->setSuccessRate(std::clamp(successRate, 0, 100));
                }
              }
              ImGui::EndGroup();
              ImGui::BeginGroup();
              {
                ImGui::Text("Repeat:");
                ImGui::SetNextItemWidth(170);
                int repeats = m_selectedItem->repeats();
                if (ImGui::InputInt("##orpg_database_items_repeats", &repeats, 1, 100)) {
                  m_selectedItem->setRepeats(std::clamp(repeats, 1, 9));
                }
              }
              ImGui::EndGroup();
              ImGui::SameLine();
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
              ImGui::BeginGroup();
              {
                ImGui::Text("TP Gain:");
                ImGui::SetNextItemWidth(170);
                int tpGain = m_selectedItem->tpGain();
                if (ImGui::InputInt("##orpg_database_items_tpGain", &tpGain, 1, 100)) {
                  m_selectedItem->setTpGain(std::clamp(tpGain, 0, 100));
                }
              }
              ImGui::EndGroup();
              ImGui::BeginGroup();
              {
                ImGui::Text("Hit Type:");
                ImGui::SetNextItemWidth(160);
                if (ImGui::BeginCombo("##orpg_database_items_hitType", DecodeEnumName(magic_enum::enum_name(m_selectedItem->hitType())).c_str())) {
                  int index{0};
                  for (auto& dir : magic_enum::enum_values<HitType>()) {
                    if (const bool is_selected = m_selectedItem->hitType() == static_cast<HitType>(magic_enum::enum_index(dir).value());
                        ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                      m_selectedItem->setHitType(dir);
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
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
              ImGui::BeginGroup();
              {
                ImGui::Text("Animation:");
                ImGui::SetNextItemWidth(160);
                // Animation Button
                ImGui::PushID("##orpg_database_items_animation");
                if (ImGui::Button(m_selectedItem->animationId() == -1  ? "Normal Attack"
                                  : m_selectedItem->animationId() == 0 ? "None"
                                                                       : Database::instance()->animationName(m_selectedItem->animationId()).c_str(),
                                  ImVec2{200 - 15, 0})) {
                  m_animationPicker = ObjectPicker("Animation"sv, Database::instance()->animations.animations(), m_selectedItem->animationId());
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
            strncpy(note, m_selectedItem->note().c_str(), IM_ARRAYSIZE(note));
            if (ImGui::InputTextMultiline("##orpg_database_items_note", note, IM_ARRAYSIZE(note), ImVec2{ImGui::GetContentRegionMax().x - 16, ImGui::GetContentRegionAvail().y - 16})) {
              m_selectedItem->setNote(note);
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
          const int tmpId = m_selectedItem->id();
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