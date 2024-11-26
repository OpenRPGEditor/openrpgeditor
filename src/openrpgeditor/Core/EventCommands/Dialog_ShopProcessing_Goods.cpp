#include "Core/EventCommands/Dialog_ShopProcessing_Goods.hpp"

#include "Core/DPIHandler.hpp"
#include "Core/Log.hpp"
#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ShopProcessing_Goods::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{402, 265} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    if (armor_picker) {
      auto [closed, confirmed] = armor_picker->draw();
      if (closed) {
        if (confirmed) {
          m_armor_selection = armor_picker->selection();
        }
        armor_picker.reset();
      }
    }
    if (item_picker) {
      auto [closed, confirmed] = item_picker->draw();
      if (closed) {
        if (confirmed) {
          m_item_selection = item_picker->selection();
        }
        item_picker.reset();
      }
    }
    if (weapon_picker) {
      auto [closed, confirmed] = weapon_picker->draw();
      if (closed) {
        if (confirmed) {
          m_weapon_selection = weapon_picker->selection();
        }
        weapon_picker.reset();
      }
    }

    ImGui::SeparatorText("Merchandise");

    ImGui::BeginGroup();
    {
      ImGui::RadioButton("Item", &m_type_selection, 0);
      ImGui::RadioButton("Weapon", &m_type_selection, 1);
      ImGui::RadioButton("Armor", &m_type_selection, 2);
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      // Items
      ImGui::BeginDisabled(m_type_selection != 0);
      ImGui::PushID("##merchandise_items");
      if (ImGui::Button(m_type_selection != 0 ? "" : Database::instance()->itemNameOrId(m_item_selection).c_str(), ImVec2{(App::DPIHandler::scale_value(180)), 0})) {
        item_picker = ObjectPicker<Item>("Items"sv, Database::instance()->items.items(), m_item_selection);
        item_picker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      // Weapon
      ImGui::BeginDisabled(m_type_selection != 1);
      ImGui::PushID("##merchandise_weapon");
      if (ImGui::Button(m_type_selection != 1 ? "" : Database::instance()->weaponNameOrId(m_weapon_selection).c_str(), ImVec2{(App::DPIHandler::scale_value(180)), 0})) {
        weapon_picker = ObjectPicker<Weapon>("Weapons"sv, Database::instance()->weapons.weapons(), m_weapon_selection);
        weapon_picker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      // Armor
      ImGui::BeginDisabled(m_type_selection != 2);
      ImGui::PushID("##merchandise_armor");
      if (ImGui::Button(m_type_selection != 2 ? "" : Database::instance()->armorNameOrId(m_armor_selection).c_str(), ImVec2{(App::DPIHandler::scale_value(180)), 0})) {
        armor_picker = ObjectPicker<Armor>("Armors"sv, Database::instance()->armors.armors(), m_armor_selection);
        armor_picker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();
      ImGui::EndGroup();
    }

    ImGui::SeparatorText("Price");
    ImVec2 cursorPos;
    ImGui::BeginGroup();
    {
      ImGui::RadioButton("Standard", &m_price_operation, 0);
      cursorPos = ImGui::GetCursorPos();
      ImGui::RadioButton("Specify", &m_price_operation, 1);
      ImGui::EndGroup();
    }
    ImGui::SetCursorPos(ImVec2{cursorPos.x + 90, cursorPos.y});
    ImGui::BeginDisabled(m_price_operation != 1);
    ImGui::SetNextItemWidth(App::DPIHandler::scale_value(200));
    if (ImGui::InputInt("##merchandise_price_constant", &m_price_constant)) {
      if (m_price_constant > 999999) {
        m_price_constant = 999999;
      }
      if (m_price_constant < 0) {
        m_price_constant = 0;
      }
    }
    ImGui::EndDisabled();

    ImGui::SetCursorPos(ImVec2{ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(105), ImGui::GetContentRegionMax().y - App::DPIHandler::scale_value(25)});
    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        m_confirmed = true;
        command->type = static_cast<ShopType>(m_type_selection);
        command->priceType = static_cast<PriceType>(m_price_operation);

        if (command->type == ShopType::Armor) {
          command->id = m_armor_selection;
        } else if (command->type == ShopType::Weapon) {
          command->id = m_weapon_selection;
        } else if (command->type == ShopType::Item) {
          command->id = m_item_selection;
        }
        if (command->priceType == PriceType::Standard) {
          if (command->type == ShopType::Armor) {
            command->price = Database::instance()->armors.armor(command->id)->price;
          } else if (command->type == ShopType::Weapon) {
            command->price = Database::instance()->weapons.weapon(command->id)->price;
          } else if (command->type == ShopType::Item) {
            command->price = Database::instance()->items.item(command->id)->price;
          }
        } else {
          command->price = m_price_constant;
        }
        ImGui::CloseCurrentPopup();
        setOpen(false);
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        ImGui::CloseCurrentPopup();
        setOpen(false);
      }
      ImGui::EndGroup();
    }
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
