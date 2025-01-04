#include "Core/DatabaseEditor.hpp"

#include "ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"

void DatabaseEditor::draw() {
  if (isReady() && !onReady.is_empty()) {
    onReady.fire();
  } else if (!isReady()) {
    return;
  }

  if (!m_isOpen) {
    return;
  }

  if (m_currentTab == nullptr && m_actors) {
    m_currentTab = &m_actors.value();
  }

  ImGui::SetNextWindowSizeConstraints(ImVec2{800, 720}, ImVec2{FLT_MAX, FLT_MAX});
  ImGui::PushID("##orpg_database_editor");
  if (ImGui::Begin("Database", &m_isOpen)) {
    ImGui::SetNextItemWidth(100.f);
    const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQR").x;
    ImGui::BeginChild("##orpg_database_editor_tab_buttons", ImVec2{calc + ImGui::GetStyle().ItemSpacing.x, 0}, 0, ImGuiWindowFlags_NoBackground);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 100);
    if (ImGui::SelectableWithBorder("Actors", m_currentTab == &m_actors.value())) {
      m_currentTab = &m_actors.value();
    }
    if (ImGui::SelectableWithBorder("Classes", m_currentTab == &m_classes.value())) {
      m_currentTab = &m_classes.value();
    }
    if (ImGui::SelectableWithBorder("Skills", m_currentTab == &m_skills.value())) {
      m_currentTab = &m_skills.value();
    }
    if (ImGui::SelectableWithBorder("Items", m_currentTab == &m_items.value())) {
      m_currentTab = &m_items.value();
    }
    if (ImGui::SelectableWithBorder("Weapons", m_currentTab == &m_weapons.value())) {
      m_currentTab = &m_weapons.value();
    }
    if (ImGui::SelectableWithBorder("Armors", m_currentTab == &m_armors.value())) {
      m_currentTab = &m_armors.value();
    }
    if (ImGui::SelectableWithBorder("Enemies", m_currentTab == &m_enemies.value())) {
      m_currentTab = &m_enemies.value();
    }
    if (ImGui::SelectableWithBorder("Troops", m_currentTab == &m_troops.value())) {
      m_currentTab = &m_troops.value();
    }
    if (ImGui::SelectableWithBorder("States", m_currentTab == &m_states.value())) {
      m_currentTab = &m_states.value();
    }
    if (ImGui::SelectableWithBorder("Animations", m_currentTab == &m_animations.value())) {
      m_currentTab = &m_animations.value();
    }
    if (ImGui::SelectableWithBorder("Tilesets", m_currentTab == &m_tilesets.value())) {
      m_currentTab = &m_tilesets.value();
    }
    if (ImGui::SelectableWithBorder("Common Events", m_currentTab == &m_commonEvents.value())) {
      m_currentTab = &m_commonEvents.value();
    }
    if (ImGui::SelectableWithBorder("System", m_currentTab == &m_system.value())) {
      m_currentTab = &m_system.value();
    }
    if (ImGui::SelectableWithBorder("Types", m_currentTab == &m_types.value())) {
      m_currentTab = &m_types.value();
    }
    if (ImGui::SelectableWithBorder("Terms", m_currentTab == &m_terms.value())) {
      m_currentTab = &m_terms.value();
    }
    if (ImGui::SelectableWithBorder("Exported Constants", m_currentTab == &m_gameConstants.value())) {
      m_currentTab = &m_gameConstants.value();
    }
    if (ImGui::SelectableWithBorder("Templates", m_currentTab == &m_templates.value())) {
      m_currentTab = &m_templates.value();
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

const IconSheet* DatabaseEditor::getIconSheet() {
  if (!m_iconSheet) {
    m_iconSheet.emplace("system/IconSet"sv);
  }
  return &m_iconSheet.value();
}