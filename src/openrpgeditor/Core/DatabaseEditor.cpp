#include "Core/DatabaseEditor.hpp"

#include "Core/DatabaseEditor/DBActorsTab.hpp"
#include "Core/DatabaseEditor/DBAnimationsTab.hpp"
#include "Core/DatabaseEditor/DBArmorsTab.hpp"
#include "Core/DatabaseEditor/DBClassesTab.hpp"
#include "Core/DatabaseEditor/DBCommonEventsTab.hpp"
#include "Core/DatabaseEditor/DBDocTab.hpp"
#include "Core/DatabaseEditor/DBEnemiesTab.hpp"
#include "Core/DatabaseEditor/DBGameConstantsTab.hpp"
#include "Core/DatabaseEditor/DBItemsTab.hpp"
#include "Core/DatabaseEditor/DBLocaleTab.hpp"
#include "Core/DatabaseEditor/DBMappingsTab.hpp"
#include "Core/DatabaseEditor/DBSkillsTab.hpp"
#include "Core/DatabaseEditor/DBStatesTab.hpp"
#include "Core/DatabaseEditor/DBSystemTab.hpp"
#include "Core/DatabaseEditor/DBTemplatesTab.hpp"
#include "Core/DatabaseEditor/DBTermsTab.hpp"
#include "Core/DatabaseEditor/DBTilesetsTab.hpp"
#include "Core/DatabaseEditor/DBTroopsTab.hpp"
#include "Core/DatabaseEditor/DBTypesTab.hpp"
#include "Core/DatabaseEditor/DBWeaponsTab.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuiExt/ImGuiUtils.hpp"

DatabaseEditor::DatabaseEditor() {
  // Add Core tabs
  addTab(std::make_shared<DBActorsTab>(this));
  addTab(std::make_shared<DBClassesTab>(this));
  addTab(std::make_shared<DBSkillsTab>(this));
  addTab(std::make_shared<DBItemsTab>(this));
  addTab(std::make_shared<DBWeaponsTab>(this));
  addTab(std::make_shared<DBArmorsTab>(this));
  addTab(std::make_shared<DBEnemiesTab>(this));
  addTab(std::make_shared<DBTroopsTab>(this));
  addTab(std::make_shared<DBStatesTab>(this));
  addTab(std::make_shared<DBAnimationsTab>(this));
  addTab(std::make_shared<DBTilesetsTab>(this));
  addTab(std::make_shared<DBCommonEventsTab>(this));
  addTab(std::make_shared<DBSystemTab>(this));
  addTab(std::make_shared<DBTypesTab>(this));
  addTab(std::make_shared<DBTermsTab>(this));
  addTab(std::make_shared<DBGameConstantsTab>(this));
  addTab(std::make_shared<DBTemplatesTab>(this));
  addTab(std::make_shared<DBMappingsTab>(this));
  addTab(std::make_shared<DBDocTab>(this));
  addTab(std::make_shared<DBLocaleTab>(this));
}
void DatabaseEditor::draw() {
  if (isReady() && !onReady.is_empty()) {
    emit_signal(onReady);
  } else if (!isReady()) {
    return;
  }

  if (!m_isOpen) {
    return;
  }

  if (m_currentTab == nullptr && !m_editorTabs.empty()) {
    m_currentTab = m_editorTabs.front().get();
  }

  ImGui::SetNextWindowSizeConstraints(ImGui::GetDPIScaledSize(800, 720), ImVec2{FLT_MAX, FLT_MAX});
  ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size / 2, ImGuiCond_Appearing);
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, {0.5, 0.5});
  if (ImGui::Begin(std::format("{}###databaseeditor", trNOOP("Database")).c_str(), &m_isOpen)) {
    ImGui::BeginHorizontal("##database_editor_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      // TODO: Calculate necessary width
      const auto calc = ImGui::CalcTextSize("#").x * 32;
      ImGui::BeginChild("##orpg_database_editor_tab_buttons", ImVec2{calc + ImGui::GetStyle().ItemSpacing.x, 0}, 0, ImGuiWindowFlags_NoBackground);
      {
        drawCategoryHeaders();
        ImGui::BeginVertical("##database_editor_tab_layout", ImGui::GetContentRegionAvail(), 0);
        {
          ImGui::Spring(0.25f);
          // TODO: Add ability to re-sort tabs
          for (const auto& tab : m_editorTabs) {
            // If the tab is not ready we can simply ignore it for now
            if (!tab->isReady()) {
              continue;
            }

            // If the tab is uninitialized attempt to initialize and skip for now, this allows tabs to acquire their required data and set up their initial states
            if (!tab->isInitialized()) {
              tab->initialize();
              continue;
            }

            if (tab->isExperimental()) {
              ORE_DISABLE_EXPERIMENTAL_BEGIN();
            }

            if (ImGui::SelectableWithBorder(std::format("{}{}", tab->tabName(), tab->tabId()).c_str(), m_currentTab == tab.get())) {
              m_currentTab = tab.get();
            }

            if (tab->isExperimental()) {
              ORE_DISABLE_EXPERIMENTAL_END();
            }
          }
#if 0
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
          ORE_DISABLE_EXPERIMENTAL_BEGIN();
          if (ImGui::SelectableWithBorder(trNOOP("Animations"), m_currentTab == &m_animations.value())) {
            m_currentTab = &m_animations.value();
          }
          if (ImGui::SelectableWithBorder(trNOOP("Tilesets"), m_currentTab == &m_tilesets.value())) {
            m_currentTab = &m_tilesets.value();
          }
          ORE_DISABLE_EXPERIMENTAL_END();
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
          ORE_DISABLE_EXPERIMENTAL_BEGIN();
          if (ImGui::SelectableWithBorder(trNOOP("Templates"), m_currentTab == &m_templates.value())) {
            m_currentTab = &m_templates.value();
          }
          if (ImGui::SelectableWithBorder(trNOOP("Data Sorting"), m_currentTab == &m_mappings.value())) {
            m_currentTab = &m_mappings.value();
          }
          if (ImGui::SelectableWithBorder(trNOOP("Documentation"), m_currentTab == &m_docs.value())) {
            m_currentTab = &m_docs.value();
          }
          if (ImGui::SelectableWithBorder(trNOOP("Localization"), m_currentTab == &m_locales.value())) {
            m_currentTab = &m_locales.value();
          }
          ORE_DISABLE_EXPERIMENTAL_END();
#endif
          ImGui::Spring(0.75f);
        }
        ImGui::EndVertical();
      }
      ImGui::EndChild();
      if (m_currentTab) {
        m_currentTab->draw();
      }
    }
    ImGui::EndHorizontal();
  }

  ImGui::End();
}

void DatabaseEditor::drawCategoryHeaders() {
  ImGui::BeginVertical("##database_category_headers", {-1, 0}, 0);
  {
    ImGui::SetNextItemWidth(-1);
    int index{0};
    if (m_filterByHeader) {
      if (ImGui::BeginCombo("##orpg_database_editor_header_list", m_selectedHeaderIndex == -1 ? "" : m_currentTab->getName(m_currentTab->getHeader(m_selectedHeaderIndex)).c_str())) {
        char buf[1024];
        for (int v : m_currentTab->getHeaders()) {
          strncpy(buf, m_currentTab->getName(v).c_str(), 1024);
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
    ImGui::BeginDisabled(!m_currentTab->hasHeader());
    if (ImGui::Checkbox("By header##orpg_database_editor_filterbyheader_check", &m_filterByHeader)) {
      if (m_filterByHeader) {
        m_selectedHeaderIndex = 0;
        m_currentTab->setHeaderRange(m_currentTab->getHeader(m_selectedHeaderIndex),
                                     m_selectedHeaderIndex + 1 >= m_currentTab->getHeaders().size() ? -1 : m_currentTab->getHeader(m_selectedHeaderIndex + 1));
      } else {
        m_selectedHeaderIndex = -1;
      }
    }
    ImGui::EndDisabled();
  }
  ImGui::EndVertical();
}