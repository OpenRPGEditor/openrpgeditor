#include "Core/EventCommands/Dialog_ChangeEquipment.hpp"

#include "angelscript.h"
#include "Database/Database.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_ChangeEquipment::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ChangeEquipment");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 12) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###ChangeEquipment", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    drawPickers();
    ImGui::BeginVertical("##change_equipment_main_layout", ImGui::GetContentRegionAvail());
    {
      GroupBox actorGroupBox(trNOOP("Actor"), "##change_equipment_actor_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (actorGroupBox.begin()) {
        ImGui::PushID("##change_equip_actor_selection");
        if (ImGui::Button(Database::instance()->actorNameAndId(m_actor).c_str(), ImVec2{-1, 0})) {
          m_actorPicker = ObjectPicker(trNOOP("Actor"), Database::instance()->actors.actorList(), m_actor);
          m_actorPicker->setOpen(true);
        }
        ImGui::PopID();
      }
      actorGroupBox.end();
      GroupBox equipTypeGroupBox(trNOOP("Equipment Type"), "##change_equipment_equip_type_group", {-1, 0}, nullptr,
                                 ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (equipTypeGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##equip_type_list", m_equipmentTypes[m_equipmentType].c_str())) {
          for (int i = 0; const auto& name : m_equipmentTypes) {
            if (ImGui::Selectable(name.c_str(), i == m_equipmentType)) {
              m_equipmentType = i;
              makeEquipableItemList();
            }
            ++i;
          }
          ImGui::EndCombo();
        }
      }
      equipTypeGroupBox.end();
      GroupBox equipGroupBox(trNOOP("Equipment Item"), "##change_equipment_equip_item_group", {-1, 0}, nullptr,
                             ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (equipGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);
        const int etypeId = Database::instance()->slotIdToEquipId(m_actor, m_equipmentType);
        if (ImGui::BeginCombo("##equip_item_list", Database::instance()->itemDisplayName(etypeId <= 1, m_equipmentItem).c_str())) {
          for (const auto item : m_equipableItems) {
            if (ImGui::Selectable(Database::instance()->itemDisplayName(etypeId <= 1, item).c_str(), item == m_equipmentItem)) {
              m_equipmentItem = item;
            }
          }
          ImGui::EndCombo();
        }
      }
      equipGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5));
      ImGui::BeginHorizontal("##change_level_dialog_buttons", {-1, -1});
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##change_level_dialog_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->actorId = m_actor;
          m_command->equipType = m_equipmentType;
          m_command->equipment = m_equipmentItem;
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

void Dialog_ChangeEquipment::makeEquipableItemList() {
  m_equipableItems.clear();
  m_equipableItems.emplace_back(0);

  const int etypeId = Database::instance()->slotIdToEquipId(m_actor, m_equipmentType);
  if (Database::instance()->isEquipTypeSealed(m_actor, etypeId)) {
    return;
  }
  if (etypeId <= 1) {
    for (int i = 1; i < Database::instance()->weapons.weapons().size(); i++) {
      if (Database::instance()->isEquipWeaponTypeOk(m_actor, Database::instance()->weapons.weapon(i)->wtypeId())) {
        m_equipableItems.emplace_back(i);
      }
    }
    return;
  }
  for (int i = 1; i < Database::instance()->armors.armors().size(); i++) {
    if (const auto& armor = Database::instance()->armors.armor(i); armor->etypeId() == etypeId && Database::instance()->isEquipArmorTypeOk(m_actor, armor->atypeId())) {
      m_equipableItems.emplace_back(i);
    }
  }
}

void Dialog_ChangeEquipment::makeEquipTypeList() {
  m_equipmentTypes.clear();
  if (!Database::instance()->actors.actor(m_actor)) {
    return;
  }

  for (int i = 0; i < Database::instance()->system.equipTypes().size() - 1; ++i) {
    const std::string name = Database::instance()->instance()->equipTypeName(Database::instance()->slotIdToEquipId(m_actor, i));
    m_equipmentTypes.emplace_back(name);
  }
  makeEquipableItemList();
}

void Dialog_ChangeEquipment::drawPickers() {
  if (!m_actorPicker) {
    return;
  }
  if (const auto [closed, confirmed] = m_actorPicker->draw(); closed) {
    if (confirmed) {
      m_actor = m_actorPicker->selection();
      makeEquipTypeList();
    }
    m_actorPicker.reset();
  }
}