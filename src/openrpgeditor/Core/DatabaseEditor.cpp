#include "Core/DatabaseEditor.hpp"

#include "imgui.h"

void DatabaseEditor::draw() {
  if (!m_isOpen) {
    return;
  }
  ImGui::SetNextWindowSizeConstraints(ImVec2{800, 720}, ImVec2{FLT_MAX, FLT_MAX});
  ImGui::PushID("##orpg_database_editor");
  if (ImGui::Begin("Database", &m_isOpen)) {
    ImGui::SetNextItemWidth(100.f);
    ImGui::PushID("##orpg_database_editor_tab_buttons");
    ImGui::BeginGroup();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 100);
    if (ImGui::Selectable("Actors", m_currentTab == &m_actors)) {
      m_currentTab = &m_actors;
    }
    if (ImGui::Selectable("Classes", m_currentTab == &m_classes)) {
      m_currentTab = &m_classes;
    }
    if (ImGui::Selectable("Skills", m_currentTab == &m_skills)) {
      m_currentTab = &m_skills;
    }
    if (ImGui::Selectable("Items", m_currentTab == &m_items)) {
      m_currentTab = &m_items;
    }
    if (ImGui::Selectable("Weapons", m_currentTab == &m_weapons)) {
      m_currentTab = &m_weapons;
    }
    if (ImGui::Selectable("Armors", m_currentTab == &m_armors)) {
      m_currentTab = &m_armors;
    }
    if (ImGui::Selectable("Enemies", m_currentTab == &m_enemies)) {
      m_currentTab = &m_enemies;
    }
    if (ImGui::Selectable("Troops", m_currentTab == &m_troops)) {
      m_currentTab = &m_troops;
    }
    if (ImGui::Selectable("States", m_currentTab == &m_states)) {
      m_currentTab = &m_states;
    }
    if (ImGui::Selectable("Animations", m_currentTab == &m_animations)) {
      m_currentTab = &m_animations;
    }
    if (ImGui::Selectable("Tilesets", m_currentTab == &m_tilesets)) {
      m_currentTab = &m_tilesets;
    }
    if (ImGui::Selectable("Common Events", m_currentTab == &m_commonEvents)) {
      m_currentTab = &m_commonEvents;
    }
    if (ImGui::Selectable("System", m_currentTab == &m_system)) {
      m_currentTab = &m_system;
    }
    ImGui::EndGroup();
    ImGui::PopID();
    ImGui::SameLine();
    if (m_currentTab) {
      m_currentTab->draw();
    }
  }

  ImGui::End();
  ImGui::PopID();
}
