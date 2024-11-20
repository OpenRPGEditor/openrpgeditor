#include "Core/DatabaseEditor.hpp"

#include "imgui.h"
#include "ImGuiExt/ImGuiUtils.hpp"

void DatabaseEditor::draw() {
  if (!m_isOpen) {
    return;
  }
  ImGui::SetNextWindowSizeConstraints(ImVec2{800, 720}, ImVec2{FLT_MAX, FLT_MAX});
  ImGui::PushID("##orpg_database_editor");
  if (ImGui::Begin("Database", &m_isOpen)) {
    ImGui::SetNextItemWidth(100.f);
    ImGui::BeginChild("##orpg_database_editor_tab_buttons", ImVec2{App::DPIHandler::scale_value(130), 0}, 0,
                      ImGuiWindowFlags_NoBackground);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 100);
    if (ImGui::SelectableWithBorder("Actors", m_currentTab == &m_actors)) {
      m_currentTab = &m_actors;
    }
    if (ImGui::SelectableWithBorder("Classes", m_currentTab == &m_classes)) {
      m_currentTab = &m_classes;
    }
    if (ImGui::SelectableWithBorder("Skills", m_currentTab == &m_skills)) {
      m_currentTab = &m_skills;
    }
    if (ImGui::SelectableWithBorder("Items", m_currentTab == &m_items)) {
      m_currentTab = &m_items;
    }
    if (ImGui::SelectableWithBorder("Weapons", m_currentTab == &m_weapons)) {
      m_currentTab = &m_weapons;
    }
    if (ImGui::SelectableWithBorder("Armors", m_currentTab == &m_armors)) {
      m_currentTab = &m_armors;
    }
    if (ImGui::SelectableWithBorder("Enemies", m_currentTab == &m_enemies)) {
      m_currentTab = &m_enemies;
    }
    if (ImGui::SelectableWithBorder("Troops", m_currentTab == &m_troops)) {
      m_currentTab = &m_troops;
    }
    if (ImGui::SelectableWithBorder("States", m_currentTab == &m_states)) {
      m_currentTab = &m_states;
    }
    if (ImGui::SelectableWithBorder("Animations", m_currentTab == &m_animations)) {
      m_currentTab = &m_animations;
    }
    if (ImGui::SelectableWithBorder("Tilesets", m_currentTab == &m_tilesets)) {
      m_currentTab = &m_tilesets;
    }
    if (ImGui::SelectableWithBorder("Common Events", m_currentTab == &m_commonEvents)) {
      m_currentTab = &m_commonEvents;
    }
    if (ImGui::SelectableWithBorder("System", m_currentTab == &m_system)) {
      m_currentTab = &m_system;
    }
    if (ImGui::SelectableWithBorder("Exported Constants", m_currentTab == &m_gameConstants)) {
      m_currentTab = &m_gameConstants;
    }
    if (ImGui::SelectableWithBorder("Templates", m_currentTab == &m_templates)) {
      m_currentTab = &m_templates;
    }
    ImGui::EndChild();
    ImGui::SameLine();
    if (m_currentTab) {
      m_currentTab->draw();
    }
  }

  ImGui::End();
  ImGui::PopID();
}
