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
  ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size / 2, ImGuiCond_Appearing);
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, {0.5, 0.5});
  ImGui::PushID("##orpg_database_editor");
  if (ImGui::Begin("Database", &m_isOpen)) {
    ImGui::SetNextItemWidth(100.f);
    const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQR").x;
    ImGui::BeginChild("##orpg_database_editor_tab_buttons", ImVec2{calc + ImGui::GetStyle().ItemSpacing.x, 0}, 0, ImGuiWindowFlags_NoBackground);

    ImGui::SetNextItemWidth(160.f);
    int index{0};
    if (m_filterByHeader) {
      if (ImGui::BeginCombo("##orpg_database_editor_header_list", m_selectedHeaderIndex == -1 ? "" : m_currentTab->getName(m_currentTab->getHeader(m_selectedHeaderIndex)).c_str())) {
        char buf[1024];
        for (int v : m_currentTab->getHeaders()) {
          strncpy(buf, m_commonEvents.value().event(v)->name().c_str(), 1024);
          if (ImGui::Selectable(buf, m_selectedHeaderIndex == index)) {
            m_selectedHeaderIndex = index;
            m_currentTab->setHeaderRange(m_currentTab->getHeader(m_selectedHeaderIndex),
                                         m_selectedHeaderIndex + 1 >= m_currentTab->getHeaders().size() ? m_currentTab->getCount() + 1 : m_currentTab->getHeader(m_selectedHeaderIndex + 1));
          }
          index++;
        }
        ImGui::EndCombo();
      }
    } else {
      ImGui::InputText("##orpg_database_editor_search_filter", &m_searchString);
    }
    ImGui::BeginDisabled(m_currentTab->hasHeader() == 0 ? true : !m_currentTab->hasHeader());
    if (ImGui::Checkbox("By header##orpg_database_editor_filterbyheader_check", &m_filterByHeader)) {
      if (m_filterByHeader) {
        m_currentTab->setHeaderRange(m_currentTab->getHeader(m_selectedHeaderIndex),
                                     m_selectedHeaderIndex + 1 >= m_currentTab->getHeaders().size() ? -1 : m_currentTab->getHeader(m_selectedHeaderIndex + 1));
      } else {
        m_selectedHeaderIndex = -1;
      }
    }
    ImGui::EndDisabled();

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 100);
    if (ImGui::SelectableWithBorder(trNOOP("Actors"), m_currentTab == &m_actors.value())) {
      m_currentTab = &m_actors.value();
    }
    if (ImGui::SelectableWithBorder(trNOOP("Classes"), m_currentTab == &m_classes.value())) {
      m_currentTab = &m_classes.value();
    }
    if (ImGui::SelectableWithBorder(trNOOP("Skills"), m_currentTab == &m_skills.value())) {
      m_currentTab = &m_skills.value();
    }
    if (ImGui::SelectableWithBorder(trNOOP("Items"), m_currentTab == &m_items.value())) {
      m_currentTab = &m_items.value();
    }
    if (ImGui::SelectableWithBorder(trNOOP("Weapons"), m_currentTab == &m_weapons.value())) {
      m_currentTab = &m_weapons.value();
    }
    if (ImGui::SelectableWithBorder(trNOOP("Armors"), m_currentTab == &m_armors.value())) {
      m_currentTab = &m_armors.value();
    }
    if (ImGui::SelectableWithBorder(trNOOP("Enemies"), m_currentTab == &m_enemies.value())) {
      m_currentTab = &m_enemies.value();
    }
    if (ImGui::SelectableWithBorder(trNOOP("Troops"), m_currentTab == &m_troops.value())) {
      m_currentTab = &m_troops.value();
    }
    if (ImGui::SelectableWithBorder(trNOOP("States"), m_currentTab == &m_states.value())) {
      m_currentTab = &m_states.value();
    }
    if (ImGui::SelectableWithBorder(trNOOP("Animations"), m_currentTab == &m_animations.value())) {
      m_currentTab = &m_animations.value();
    }
    if (ImGui::SelectableWithBorder(trNOOP("Tilesets"), m_currentTab == &m_tilesets.value())) {
      m_currentTab = &m_tilesets.value();
    }
    if (ImGui::SelectableWithBorder(trNOOP("Common Events"), m_currentTab == &m_commonEvents.value())) {
      m_currentTab = &m_commonEvents.value();
    }
    if (ImGui::SelectableWithBorder(trNOOP("System"), m_currentTab == &m_system.value())) {
      m_currentTab = &m_system.value();
    }
    if (ImGui::SelectableWithBorder(trNOOP("Types"), m_currentTab == &m_types.value())) {
      m_currentTab = &m_types.value();
    }
    if (ImGui::SelectableWithBorder(trNOOP("Terms"), m_currentTab == &m_terms.value())) {
      m_currentTab = &m_terms.value();
    }
    if (ImGui::SelectableWithBorder(trNOOP("Exported Constants"), m_currentTab == &m_gameConstants.value())) {
      m_currentTab = &m_gameConstants.value();
    }
    if (ImGui::SelectableWithBorder(trNOOP("Templates"), m_currentTab == &m_templates.value())) {
      m_currentTab = &m_templates.value();
    }
    if (ImGui::SelectableWithBorder(trNOOP("Switches & Variables"), m_currentTab == &m_mappings.value())) {
      m_currentTab = &m_mappings.value();
    }
    if (ImGui::SelectableWithBorder(trNOOP("Documentation"), m_currentTab == &m_docs.value())) {
      m_currentTab = &m_docs.value();
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

std::string DatabaseEditor::getFilterString() { return m_searchString; }
