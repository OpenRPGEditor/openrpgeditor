#include "Core/DatabaseEditor/DBTypesTab.hpp"

#include "Core/Application.hpp"
#include "Database/System.hpp"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

void DBTypesTab::setInitialBuffer(const std::vector<std::string>& list, std::string& buffer, const int selected) {
  if (list.empty()) {
    buffer.clear();
  }

  if (buffer.empty() && selected > 0 && selected < list.size()) {
    buffer = list[selected];
  }
}

void DBTypesTab::drawTypeTable(const std::string_view id, const std::string_view listId, const std::string_view label, const std::vector<std::string>& list, int& selected, const EditType editType,
                               std::string& buffer) {
  ImGui::PushFont(App::APP->getMonoFont());
  auto calc = ImGui::CalcTextSize("ABCDEFGKLMNOPQRSTUVWXYZABCDEFG");
  calc.y *= 16;
  calc.y += ImGui::GetStyle().ItemSpacing.y * 32;
  ImGui::PopFont();

  ImGui::BeginChild(id.data(), {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_NoResize);
  {
    ImGui::TextUnformatted(label.data());
    ImGui::PushFont(App::APP->getMonoFont());
    if (ImGui::BeginTable(listId.data(), 1, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY, calc)) {
      for (int i = 1; i < list.size(); ++i) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if (ImGui::Selectable(std::format("{:02} {}##_{}", i, list[i], listId).c_str(), i == selected, ImGuiSelectableFlags_SpanAllColumns)) {
          buffer = list[i];
          selected = i;
        }
      }
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && ImGui::IsWindowFocused() && selected > 0 && selected < list.size()) {
        if (editType == EditType::Element) {
          m_system->setElement(selected, "");
        }
        if (editType == EditType::Skill) {
          m_system->setSkillType(selected, "");
        }
        if (editType == EditType::Weapon) {
          m_system->setWeaponType(selected, "");
        }
        if (editType == EditType::Armor) {
          m_system->setArmorType(selected, "");
        }
        if (editType == EditType::Equipment) {
          m_system->setEquipType(selected, "");
        }
      }
      ImGui::EndTable();
    }
    ImGui::PopFont();

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_ChildBg));
    if (ImGui::InputText(std::format("{}_edit", listId).c_str(), &buffer) && selected > 0 && selected < list.size()) {

      if (editType == EditType::Element) {
        m_system->setElement(selected, buffer);
      }
      if (editType == EditType::Skill) {
        m_system->setSkillType(selected, buffer);
      }
      if (editType == EditType::Weapon) {
        m_system->setWeaponType(selected, buffer);
      }
      if (editType == EditType::Armor) {
        m_system->setArmorType(selected, buffer);
      }
      if (editType == EditType::Equipment) {
        m_system->setEquipType(selected, buffer);
      }
    }
    ImGui::PopStyleColor();

    if (ImGui::Button("Change Maximum...", {ImGui::GetContentRegionAvail().x, 0})) {
      m_editMax = list.size() - 1;
      m_maxEditType = editType;
      m_changeMaxRequested = true;
    }
  }

  ImGui::EndChild();
}
void DBTypesTab::draw() {
  setInitialBuffer(m_system->elements(), m_elementBuffer, m_selectedElement);
  setInitialBuffer(m_system->skillTypes(), m_skillBuffer, m_selectedSkill);
  setInitialBuffer(m_system->weaponTypes(), m_weaponBuffer, m_selectedWeapon);
  setInitialBuffer(m_system->armorTypes(), m_armorBuffer, m_selectedArmor);
  setInitialBuffer(m_system->equipTypes(), m_equipmentBuffer, m_selectedEquipment);

  ImGui::BeginChild("##orpg_types_editor", {}, ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
  {
    drawTypeTable("##orpg_types_elements"sv, "##types_elements_list"sv, "Elements"sv, m_system->elements(), m_selectedElement, EditType::Element, m_elementBuffer);
    ImGui::SameLine();
    drawTypeTable("##orpg_types_skills"sv, "##types_skills_list"sv, "Skill Types"sv, m_system->skillTypes(), m_selectedSkill, EditType::Skill, m_skillBuffer);
    ImGui::SameLine();
    drawTypeTable("##orpg_types_weapons"sv, "##types_weapons_list"sv, "Weapon Types"sv, m_system->weaponTypes(), m_selectedWeapon, EditType::Weapon, m_weaponBuffer);
    ImGui::SameLine();
    drawTypeTable("##orpg_types_armors"sv, "##types_armors_list"sv, "Armor Types"sv, m_system->armorTypes(), m_selectedArmor, EditType::Armor, m_armorBuffer);
    ImGui::SameLine();
    drawTypeTable("##orpg_types_equips"sv, "##types_equips_list"sv, "Equipment Types"sv, m_system->equipTypes(), m_selectedEquipment, EditType::Equipment, m_equipmentBuffer);
  }
  ImGui::EndChild();

  if (m_changeMaxRequested) {
    ImGui::OpenPopup("Change Max");
    m_changeMaxRequested = false;
  }

  if (ImGui::BeginPopupModal("Change Max", nullptr,
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
    ImGui::Text(
        "Specify an amount to resize the list to\n"
        "This cannot be undone!");
    ImGui::InputInt("##value_input", &m_editMax);
    ImGui::SameLine();
    if (ImGui::Button("Accept")) {
      m_confirmationRequested = true;
      m_confirmationClosed = false;
      ImGui::OpenPopup("Confirm Change");
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
    }

    if (ImGui::BeginPopupModal("Confirm Change", nullptr,
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
      ImGui::Text("Are you sure?");
      if (ImGui::Button("Yes")) {
        m_confirmationClosed = true;
        switch (m_maxEditType) {
        case EditType::Element: {
          // m_system.elements().resize(m_editMax + 1);
          // m_selectedElement = m_system.elements().size() - 1;
          break;
        }
        case EditType::Skill: {
          // m_system.skillTypes().resize(m_editMax + 1);
          // m_selectedSkill = m_system.skillTypes().size() - 1;
          break;
        }
        case EditType::Weapon: {
          // m_system.weaponTypes().resize(m_editMax + 1);
          // m_selectedWeapon = m_system.weaponTypes().size() - 1;
          break;
        }
        case EditType::Armor: {
          // m_system.armorTypes().resize(m_editMax + 1);
          // m_selectedArmor = m_system.armorTypes().size() - 1;
          break;
        }
        case EditType::Equipment: {
          // m_system.equipTypes().resize(m_editMax + 1);
          // m_selectedEquipment = (m_system.equipTypes().size() - 1 > 0) ? m_system.equipTypes().size() - 1 : 0;
          break;
        }
        }
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        m_confirmationClosed = true;
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }
    if (m_confirmationRequested && m_confirmationClosed) {
      ImGui::CloseCurrentPopup();
      m_confirmationRequested = false;
    }
    ImGui::EndPopup();
  }
}
