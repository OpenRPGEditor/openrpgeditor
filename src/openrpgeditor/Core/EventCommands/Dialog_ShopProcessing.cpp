#include "Core/EventCommands/Dialog_ShopProcessing.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"
#include <imgui.h>
#include <imgui_internal.h>

std::tuple<bool, bool> Dialog_ShopProcessing::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ShopProcessing");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 60) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 10) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###ShopProcessing", m_name).c_str(), &m_open, ImGuiWindowFlags_NoResize)) {
    drawPickers();
    ImGui::BeginVertical("##shop_processing_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && !m_goods.empty() && m_selection >= 0) {
        m_goods.erase(m_goods.begin() + m_selection);
        if (m_selection >= m_goods.size()) {
          m_selection = std::max<int>(-1, m_goods.size() - 1);
        }
      }

      if (ImGui::BeginTable("##shop_processing_main", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                            ImVec2{-1, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()})) {
        ImGui::TableSetupScrollFreeze(2, 1);
        ImGui::TableSetupColumn(trNOOP("Merchandise"), ImGuiTableColumnFlags_WidthStretch, 0.5f);
        ImGui::TableSetupColumn(trNOOP("Price"), ImGuiTableColumnFlags_WidthStretch, 0.5f);
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();

        for (int n = 0; n < m_goods.size(); n++) {
          ImGui::TableNextRow();
          const int itemId = m_goods.at(n)->id;
          if (ImGui::TableNextColumn()) {
            std::string text = m_goods.at(n)->type == ShopType::Armor  ? Database::instance()->armorName(itemId)
                               : m_goods.at(n)->type == ShopType::Item ? Database::instance()->itemName(itemId)
                                                                       : Database::instance()->weaponName(itemId);
            const auto selected = (m_selection == n);
            if (ImGui::SelectableWithBorder(std::format("{}##{}", text, n).c_str(), selected,
                                            ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
              if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                m_goodsDialog.emplace(trNOOP("Goods Processing"), m_goods.at(n));
                m_goodsDialog->setOpen(true);
              }
              m_selection = n;
            }
            if (selected) {
              ImGui::SetItemDefaultFocus();
            }
          }
          if (ImGui::TableNextColumn()) {
            std::string text = m_goods.at(n)->priceType == PriceType::Specific ? std::to_string(m_goods.at(n)->price)
                                                                               : (m_goods.at(n)->type == ShopType::Armor  ? std::to_string(Database::instance()->armors.armor(itemId)->price())
                                                                                  : m_goods.at(n)->type == ShopType::Item ? std::to_string(Database::instance()->items.item(itemId)->price())
                                                                                                                          : std::to_string(Database::instance()->weapons.weapon(itemId)->price()));
            ImGui::Text("%s", text.c_str());
          }
        }

        ImGui::TableNextRow();
        if (ImGui::TableNextColumn()) {
          const auto selected = m_selection == m_goods.size();
          if (ImGui::SelectableWithBorder("##dummy_shop_processing_selection", selected,
                                          ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {

            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_goodsDialog.emplace(trNOOP("Goods Processing"));
              m_goodsDialog->setOpen(true);
            }
            m_selection = -1;
          }
          if (selected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::TableNextColumn();
        ImGui::EndTable();
      }
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##shop_processing_button_layout", {-1, 0}, 0);
      {
        ImGui::Checkbox(trNOOP("Purchase Only"), &m_purchaseOnly);
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##shop_processing_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          // If we don't have any goods we don't want to create a command, this differs from MV/MZ on purpose so we don't create an invalid shop by accident.
          if (!m_goods.empty()) {
            m_confirmed = true;
            // Grab and erase the first good
            const auto firstGood = std::dynamic_pointer_cast<ShopProcessingGoodCommand>(m_goods.front()->clone());
            m_goods.erase(m_goods.begin());
            m_command->id = firstGood->id;
            m_command->purchaseOnly = m_purchaseOnly;
            m_command->priceType = firstGood->priceType;
            m_command->type = firstGood->type;
            m_command->price = firstGood->price;
            m_command->goods = m_goods;
          } else {
            m_confirmed = false;
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

void Dialog_ShopProcessing::drawPickers() {
  if (m_goodsDialog) {
    if (const auto [closed, confirmed] = m_goodsDialog->draw(); closed) {
      if (confirmed) {
        if (m_selection == -1) {
          m_goods.emplace_back(std::dynamic_pointer_cast<ShopProcessingGoodCommand>(m_goodsDialog->getCommand()->clone()));
          m_selection = std::max<int>(0, m_goods.size() - 1);
        }
      }
      m_goodsDialog.reset();
    }
  }
}