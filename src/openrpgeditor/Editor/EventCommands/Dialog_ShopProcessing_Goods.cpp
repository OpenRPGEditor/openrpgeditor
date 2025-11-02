#include "Editor/EventCommands/Dialog_ShopProcessing_Goods.hpp"

#include "Database/Database.hpp"
#include "Editor/CommonUI/GroupBox.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>
std::tuple<bool, bool> Dialog_ShopProcessing_Goods::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ShopProcessing_Goods");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 40) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 10) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###ShopProcessing_Goods", m_name).c_str(), &m_open, ImGuiWindowFlags_NoResize)) {
    drawPickers();
    ImGui::BeginVertical("##shop_processing_goods_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox merchandiseGroupBox(trNOOP("Merchandise"), "##shop_processing_goods_merchandise_group", {-1, 0});
      if (merchandiseGroupBox.begin()) {
        ImGui::BeginHorizontal("##shop_processing_goods_merchandise_group_main_layout", {-1, 0}, 0);
        {
          ImGui::BeginVertical("##shop_processing_goods_merchandise_group_left_layout", {-1, 0}, 0);
          {
            for (const auto type : magic_enum::enum_values<ShopType>()) {
              ImGui::RadioButton(DecodeEnumName(type).c_str(), &m_typeSelection, static_cast<int>(type));
            }
          }
          ImGui::EndVertical();
          ImGui::BeginVertical("##shop_processing_goods_merchandise_group_right_layout", {-1, 0}, 0);
          {
            ImGui::BeginDisabled(m_typeSelection != 0);
            {
              if (ImGui::EllipsesButton(m_typeSelection != 0 ? "##shop_processing_goods_merchandise_item_button" : Database::instance()->itemNameAndId(m_itemSelection).c_str(), ImVec2{-1, 0})) {
                m_itemPicker.emplace(Database::instance()->items->items(), m_itemSelection);
                m_itemPicker->setOpen(true);
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_typeSelection != 1);
            {
              if (ImGui::EllipsesButton(m_typeSelection != 1 ? "##shop_processing_goods_merchandise_weapon_button" : Database::instance()->weaponNameAndId(m_weaponSelection).c_str(), ImVec2{-1, 0})) {
                m_weaponPicker.emplace(Database::instance()->weapons->weapons(), m_weaponSelection);
                m_weaponPicker->setOpen(true);
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_typeSelection != 2);
            {
              if (ImGui::EllipsesButton(m_typeSelection != 2 ? "##shop_processing_goods_merchandise_armor_button" : Database::instance()->armorNameAndId(m_armorSelection).c_str(), ImVec2{-1, 0})) {
                m_armorPicker.emplace(Database::instance()->armors->armors(), m_armorSelection);
                m_armorPicker->setOpen(true);
              }
            }
            ImGui::EndDisabled();
          }
          ImGui::EndVertical();
        }
        ImGui::EndHorizontal();
      }
      merchandiseGroupBox.end();
      GroupBox priceGroupBox(trNOOP("Price"), "##shop_processing_goods_price_group", {-1, 0});
      if (priceGroupBox.begin()) {
        ImGui::BeginHorizontal("##shop_processing_goods_price_group_main_layout", {-1, 0}, 0);
        {
          ImGui::BeginVertical("##shop_processing_goods_price_group_left_layout", {-1, 0}, 0);
          {
            for (const auto type : magic_enum::enum_values<PriceType>()) {
              ImGui::RadioButton(DecodeEnumName(type).c_str(), &m_priceOperation, static_cast<int>(type));
            }
          }
          ImGui::EndVertical();
          ImGui::BeginVertical("##shop_processing_goods_price_group_right_layout", {-1, 0}, 0);
          {
            ImGui::Dummy({0, ImGui::GetFrameHeightWithSpacing()});
            ImGui::BeginChild("##price_input", {-1, 0}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_NavFlattened,
                              ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
            {
              ImGui::BeginDisabled(m_priceOperation != 1);
              ImGui::SetNextItemWidth(-1);
              if (ImGui::SpinInt("##shop_processing_price", &m_priceConstant, 1, 100, m_priceOperation != 1 ? "" : nullptr)) {
                m_priceConstant = std::clamp(m_priceConstant, 0, 999999);
              }
              ImGui::EndDisabled();
            }
            ImGui::EndChild();
          }
          ImGui::EndVertical();
        }
        ImGui::EndHorizontal();
      }
      priceGroupBox.end();

      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##shop_processing_goods_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##Shop_processing_goods_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;

          m_command->type = static_cast<ShopType>(m_typeSelection);
          m_command->priceType = static_cast<PriceType>(m_priceOperation);
          switch (m_command->type) {
          case ShopType::Armor:
            m_command->id = m_armorSelection;
            break;
          case ShopType::Weapon:
            m_command->id = m_weaponSelection;
            break;
          case ShopType::Item:
            m_command->id = m_itemSelection;
            break;
          }

          if (m_command->priceType == PriceType::Standard) {
            switch (m_command->type) {
            case ShopType::Armor:
              m_command->price = Database::instance()->armors->armor(m_command->id)->price();
              break;
            case ShopType::Weapon:
              m_command->price = Database::instance()->weapons->weapon(m_command->id)->price();
              break;
            case ShopType::Item:
              m_command->price = Database::instance()->items->item(m_command->id)->price();
              break;
            }
          } else {
            m_command->price = m_priceConstant;
          }
          ImGui::CloseCurrentPopup();
          setOpen(false);
        } else if (ret == 1) {
          ImGui::CloseCurrentPopup();
          setOpen(false);
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}

void Dialog_ShopProcessing_Goods::drawPickers() {
  if (m_armorPicker) {
    if (const auto [closed, confirmed] = m_armorPicker->draw(); closed) {
      if (confirmed) {
        m_armorSelection = m_armorPicker->selection();
      }
      m_armorPicker.reset();
    }
  }
  if (m_itemPicker) {
    if (const auto [closed, confirmed] = m_itemPicker->draw(); closed) {
      if (confirmed) {
        m_itemSelection = m_itemPicker->selection();
      }
      m_itemPicker.reset();
    }
  }
  if (m_weaponPicker) {
    if (const auto [closed, confirmed] = m_weaponPicker->draw(); closed) {
      if (confirmed) {
        m_weaponSelection = m_weaponPicker->selection();
      }
      m_weaponPicker.reset();
    }
  }
}