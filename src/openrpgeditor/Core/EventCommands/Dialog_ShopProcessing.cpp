#include "Core/EventCommands/Dialog_ShopProcessing.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

std::tuple<bool, bool> Dialog_ShopProcessing::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{551, 400} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open,
                             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {
    if (goodsDialog) {
      auto [closed, confirmed] = goodsDialog->draw();
      if (confirmed) {
        if (m_selection_type == 1) {
          APP_INFO("Running selection type 1");
          m_goods.push_back(std::static_pointer_cast<ShopProcessingGoodCommand>(goodsDialog->getCommand()));
        } else if (m_selection_type == 2) {
          APP_INFO("Running selection type 2");
          std::shared_ptr<ShopProcessingGoodCommand> shopCmd =
              std::static_pointer_cast<ShopProcessingGoodCommand>(goodsDialog->getCommand());
          APP_INFO(std::to_string(shopCmd->id) + "\n" + std::to_string(shopCmd->price) + "\n" +
                   std::to_string(static_cast<int>(shopCmd->type)) + "\n" +
                   std::to_string(static_cast<int>(shopCmd->priceType)));
          m_id = shopCmd->id;
          m_priceType = static_cast<int>(shopCmd->priceType);
          m_type = static_cast<int>(shopCmd->type);
          m_price = shopCmd->price;
        }
        goodsDialog.reset();
      }
    }

    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))) {
      if (m_goods_selection > 0) {
        m_goods.erase(m_goods.begin() + m_goods_selection);
      } else {
        m_id = m_goods.at(0)->id;
        m_price = m_goods.at(0)->price;
        m_priceType = static_cast<int>(m_goods.at(0)->priceType);
        m_type = static_cast<int>(m_goods.at(0)->type);
        m_goods.erase(m_goods.begin());
      }
    }

    if (ImGui::BeginTable("##shop_processing_main", 2,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX |
                              ImGuiTableFlags_ScrollY,
                          ImVec2{(App::DPIHandler::scale_value(530)), App::DPIHandler::scale_value(390)})) {

      ImGui::TableSetupScrollFreeze(1, 0);
      ImGui::TableSetupColumn("Merchandise", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn(("Price" + std::string(30, ' ')).c_str(), ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableHeadersRow();
      ImGui::TableNextRow();
      if (ImGui::TableNextColumn()) {
        std::string text = m_type == 2   ? Database::instance().armorName(m_id)
                           : m_type == 0 ? Database::instance().itemName(m_id)
                                         : Database::instance().weaponName(m_id);
        const bool isSelected = (m_goods_selection == 0);
        if (ImGui::SelectableWithBorder(text.c_str(), isSelected,
                                        ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns |
                                            ImGuiSelectableFlags_AllowDoubleClick)) {
          if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
            // This is a specific scenario where index 0 is used within the actual command, not the vector.
            // This is used to modify index 0 only.
            APP_INFO("price: " + std::to_string(m_price));
            m_selection_type = 2;
            if (!goodsDialog)
              goodsDialog.emplace("Goods Processing", m_id, m_price, m_type, m_priceType);

            goodsDialog->SetOpen(true);
          }
          m_goods_selection = 0;
          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }
      }
      if (ImGui::TableNextColumn()) {

        std::string text = m_priceType == 1 ? std::to_string(m_price)
                           : m_id == 0      ? ""
                           : m_type == 2    ? std::to_string(Database::instance().armors.armor(m_id)->price)
                           : m_type == 1    ? std::to_string(Database::instance().weapons.weapon(m_id)->price)
                                            : std::to_string(Database::instance().items.item(m_id)->price);
        ImGui::Text("%s", text.c_str());
      }

      for (int n = 0; n < m_goods.size(); n++) {
        ImGui::TableNextRow();
        int itemId = m_goods.at(n)->id;
        if (ImGui::TableNextColumn()) {
          std::string text = m_goods.at(n)->type == ShopType::Armor  ? Database::instance().armorName(itemId)
                             : m_goods.at(n)->type == ShopType::Item ? Database::instance().itemName(itemId)
                                                                     : Database::instance().weaponName(itemId);
          const bool isSelected = (m_goods_selection == n + 1);
          if (ImGui::SelectableWithBorder(text.c_str(), isSelected,
                                          ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns |
                                              ImGuiSelectableFlags_AllowDoubleClick)) {
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              // This is used to modify existing items only.
              m_selection_type = 0;
              if (!goodsDialog)
                goodsDialog.emplace("Goods Processing", m_goods.at(n));

              goodsDialog->SetOpen(true);
            }
            m_goods_selection = n + 1;
            if (isSelected)
              ImGui::SetItemDefaultFocus();
          }
        }
        if (ImGui::TableNextColumn()) {
          std::string text = m_goods.at(n)->priceType == PriceType::Specific ? std::to_string(m_goods.at(n)->price)
                             : m_goods.at(n)->type == ShopType::Armor
                                 ? std::to_string(Database::instance().armors.armor(itemId)->price)
                             : m_goods.at(n)->type == ShopType::Item
                                 ? std::to_string(Database::instance().items.item(itemId)->price)
                                 : std::to_string(Database::instance().weapons.weapon(itemId)->price);
          ImGui::Text("%s", text.c_str());
        }
      }

      const bool isSelected = m_goods_selection == m_goods.size() + 1;
      if (ImGui::SelectableWithBorder("##dummy_shopprocessing_selection", isSelected,
                                      ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns |
                                          ImGuiSelectableFlags_AllowDoubleClick)) {

        if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
          // This is used to create new nodes only.
          m_selection_type = 1;
          if (!goodsDialog)
            goodsDialog.emplace("Goods Processing");

          goodsDialog->SetOpen(true);
        }
        m_goods_selection = m_goods.size() + 1;
        if (isSelected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndTable();
    }
    ImGui::Checkbox("Purchase Only", &m_purchaseOnly);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + App::DPIHandler::scale_value(ImGui::GetWindowWidth() - 106));
    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        m_confirmed = true;
        command->goods = m_goods;
        command->id = m_id;
        command->purchaseOnly = m_purchaseOnly;
        command->priceType = static_cast<PriceType>(m_priceType);
        command->type = static_cast<ShopType>(m_type);
        command->price = m_price;
        ImGui::CloseCurrentPopup();
        SetOpen(false);
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        ImGui::CloseCurrentPopup();
        SetOpen(false);
      }
      ImGui::EndGroup();
    }
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
