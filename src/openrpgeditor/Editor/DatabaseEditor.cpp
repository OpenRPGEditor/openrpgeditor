#include "Editor/DatabaseEditor.hpp"

#include "Editor/DatabaseEditor/DBActorsTab.hpp"
#include "Editor/DatabaseEditor/DBAnimationsTab.hpp"
#include "Editor/DatabaseEditor/DBArmorsTab.hpp"
#include "Editor/DatabaseEditor/DBClassesTab.hpp"
#include "Editor/DatabaseEditor/DBCommonEventsTab.hpp"
#include "Editor/DatabaseEditor/DBDocTab.hpp"
#include "Editor/DatabaseEditor/DBEnemiesTab.hpp"
#include "Editor/DatabaseEditor/DBGameConstantsTab.hpp"
#include "Editor/DatabaseEditor/DBItemsTab.hpp"
#include "Editor/DatabaseEditor/DBLocaleTab.hpp"
#include "Editor/DatabaseEditor/DBMappingsTab.hpp"
#include "Editor/DatabaseEditor/DBSkillsTab.hpp"
#include "Editor/DatabaseEditor/DBStatesTab.hpp"
#include "Editor/DatabaseEditor/DBSystemTab.hpp"
#include "Editor/DatabaseEditor/DBTemplatesTab.hpp"
#include "Editor/DatabaseEditor/DBTermsTab.hpp"
#include "Editor/DatabaseEditor/DBTilesetsTab.hpp"
#include "Editor/DatabaseEditor/DBTroopsTab.hpp"
#include "Editor/DatabaseEditor/DBTypesTab.hpp"
#include "Editor/DatabaseEditor/DBWeaponsTab.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuiExt/ImGuiUtils.hpp"

DatabaseEditor* DatabaseEditor::m_instance = nullptr;

DatabaseEditor::DatabaseEditor() {
  m_instance = this;
  // Add Core tabs
  addTab(std::make_shared<DBActorsTab>());
  addTab(std::make_shared<DBClassesTab>());
  addTab(std::make_shared<DBSkillsTab>());
  addTab(std::make_shared<DBItemsTab>());
  addTab(std::make_shared<DBWeaponsTab>());
  addTab(std::make_shared<DBArmorsTab>());
  addTab(std::make_shared<DBEnemiesTab>());
  addTab(std::make_shared<DBTroopsTab>());
  addTab(std::make_shared<DBStatesTab>());
  addTab(std::make_shared<DBAnimationsTab>());
  addTab(std::make_shared<DBTilesetsTab>());
  addTab(std::make_shared<DBCommonEventsTab>());
  addTab(std::make_shared<DBSystemTab>());
  addTab(std::make_shared<DBTypesTab>());
  addTab(std::make_shared<DBTermsTab>());
  addTab(std::make_shared<DBGameConstantsTab>());
  addTab(std::make_shared<DBTemplatesTab>());
  addTab(std::make_shared<DBMappingsTab>());
  addTab(std::make_shared<DBDocTab>());
  addTab(std::make_shared<DBLocaleTab>());
}

DatabaseEditor::~DatabaseEditor() {
  if (m_instance == this) {
    m_instance = nullptr;
  }
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

            if (ImGui::SelectableWithBorder(std::format("{}##{}", tab->tabName(), tab->tabId()).c_str(), m_currentTab == tab.get())) {
              m_currentTab = tab.get();
            }

            if (tab->isExperimental()) {
              ORE_DISABLE_EXPERIMENTAL_END();
            }
          }
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
    if (m_filterByHeader) {
      if (ImGui::BeginCombo("##orpg_database_editor_header_list", m_selectedHeaderIndex == -1 ? "" : m_currentTab->objectName(m_currentTab->header(m_selectedHeaderIndex)).c_str())) {
        int index{0};
        char buf[1024];
        for (const int obj : m_currentTab->headers()) {
          strncpy(buf, m_currentTab->objectName(obj).c_str(), 1024);
          if (ImGui::Selectable(buf, m_selectedHeaderIndex == index)) {
            m_selectedHeaderIndex = index;
            m_currentTab->setHeaderRange(m_currentTab->header(m_selectedHeaderIndex),
                                         m_selectedHeaderIndex + 1 >= m_currentTab->headers().size() ? m_currentTab->getCount() + 1 : m_currentTab->header(m_selectedHeaderIndex + 1));
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
        m_currentTab->setHeaderRange(m_currentTab->header(m_selectedHeaderIndex),
                                     m_selectedHeaderIndex + 1 >= m_currentTab->headers().size() ? -1 : m_currentTab->header(m_selectedHeaderIndex + 1));
      } else {
        m_selectedHeaderIndex = -1;
      }
    }
    ImGui::EndDisabled();
  }
  ImGui::EndVertical();
}