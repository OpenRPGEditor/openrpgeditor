#include "Core/EventCommands/Dialog_SelectItem.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <tuple>

std::tuple<bool, bool> Dialog_SelectItem::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###SelectItem");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 30) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 7) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###SelectItem", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();
    ImGui::BeginVertical("##select_item_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox variableGroupBox(trNOOP("Variable"), "##select_item_variable_group_box", {-1, 0});
      if (variableGroupBox.begin()) {
        if (ImGui::EllipsesButton(Database::instance()->variableNameAndId(m_variable).c_str(), {-1, 0})) {
          m_variablePicker.emplace(VariableSwitchPicker::Type::Variable, Database::instance()->system.variables(), m_variable);
          m_variablePicker->setOpen(true);
        }
      }
      variableGroupBox.end();
      GroupBox itemTypeGroupBox(trNOOP("Item Type"), "##input_number_item_type_group_box", {-1, 0});
      if (itemTypeGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##input_number_item_type_selection", DecodeEnumName(magic_enum::enum_cast<ItemType>(m_type).value_or(ItemType::Key_Item)).c_str())) {
          for (const auto& itemType : magic_enum::enum_values<ItemType>()) {
            bool selected = m_type == static_cast<int>(itemType);
            if (ImGui::Selectable(DecodeEnumName(itemType).c_str(), selected)) {
              m_type = static_cast<int>(itemType);
            }
            if (selected) {
              ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndCombo();
        }
      }
      itemTypeGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##select_item_button_layout");
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##select_item_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->item = m_variable;
          m_command->type = static_cast<ItemType>(m_type);
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

void Dialog_SelectItem::drawPickers() {
  if (m_variablePicker) {
    if (const auto [closed, confirmed] = m_variablePicker->draw(); closed) {
      if (confirmed) {
        m_variable = m_variablePicker->selection();
      }
      m_variablePicker.reset();
    }
  }
}