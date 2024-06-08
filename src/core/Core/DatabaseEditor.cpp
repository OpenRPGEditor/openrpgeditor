#include "Core/DatabaseEditor.hpp"

#include "imgui.h"

void DBActorsTab::draw() {
  ImGui::BeginChild("#orpg_actors_tab");
  ImGui::Text("Actors");
  ImGui::EndChild();
}

void DBClassesTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("Classes");
  ImGui::EndChild();
}

void DBSkillsTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("Skills");
  ImGui::EndChild();
}

void DBItemsTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("Items");
  ImGui::EndChild();
}

void DBWeaponsTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("Weapons");
  ImGui::EndChild();
}

void DBArmorsTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("Armors");
  ImGui::EndChild();
}

void DBEnemiesTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("Enemies");
  ImGui::EndChild();
}

void DBTroopsTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("Troops");
  ImGui::EndChild();
}

void DBStatesTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("States");
  ImGui::EndChild();
}

void DBAnimationsTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("Animations");
  ImGui::EndChild();
}

void DBTilesetsTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("Tilesets");
  ImGui::EndChild();
}

void DBCommonEventsTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("CommonEvents");
  ImGui::EndChild();
}

void DBSystemTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("System");
  ImGui::EndChild();
}

bool DatabaseEditor::draw() {
  ImGui::PushID("##orpg_database_editor");
  if (ImGui::Begin("Database", &m_isOpen)) {
    ImGui::SetNextItemWidth(100.f);
    ImGui::PushID("##orpg_database_editor_tab_buttons");
    ImGui::BeginGroup();
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
    ImGui::PushID("##orpg_database_editor_tab_contents");
    ImGui::BeginGroup();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4.f);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x);
    if (m_currentTab) {
      m_currentTab->draw();
    }
    ImGui::EndGroup();
    ImGui::PopID();
  }

  ImGui::End();
  ImGui::PopID();

  return m_isOpen;
}






