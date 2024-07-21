#include "Dialog_ShopProcessing.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Project.hpp"
#include "Core/ImGuiUtils.hpp"

std::tuple<bool, bool> Dialog_ShopProcessing::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{551, 400} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {
    if (ImGui::BeginTable("##shop_processing_main", 2,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX |
                              ImGuiTableFlags_ScrollY,
                          ImVec2{(ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(100)) , ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(100)})) {

      ImGui::TableSetupScrollFreeze(1, 0);
      ImGui::TableSetupColumn("Merchandise");
      ImGui::TableSetupColumn("Price", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableHeadersRow();
      for (int n = 0; n < m_goods.size(); n++) {
        ImGui::TableNextRow();
        if (ImGui::TableNextColumn()) {
          int itemId = m_goods.at(n)->id;
          std::string text = m_goods.at(n)->type == ShopType::Armor ?  Database::Instance->armorName(itemId)
                             : m_goods.at(n)->type == ShopType::Item ? Database::Instance->itemName(itemId)
                                                                     :                                         Database::Instance->weaponName(itemId);
          const bool isSelected = (m_goods_selection == n + 1);
          if (ImGui::SelectableWithBorder(text.c_str(), isSelected,
                                          ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns |
                                              ImGuiSelectableFlags_AllowDoubleClick)) {
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              // Open Sub menu
            }
            m_goods_selection = n + 1;
            if (isSelected)
              ImGui::SetItemDefaultFocus();
          }
        }

      }
      ImGui::EndTable();
    }
    ImGui::SetCursorPos(ImVec2{ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(80),
                               ImGui::GetContentRegionMax().y - App::DPIHandler::scale_value(20)});
    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        m_confirmed = true;
        ImGui::CloseCurrentPopup();
        SetOpen(false);
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        ImGui::CloseCurrentPopup();
        SetOpen(false);
      }
      ImGui::EndPopup();
      ImGui::EndGroup();
    }
  }

  return std::make_tuple(!m_open, m_confirmed);
}
