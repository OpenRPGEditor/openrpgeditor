#include "Core/EventEditor.hpp"

#include "Core/DPIHandler.hpp"
#include "Core/ImGuiExt/ImGuiNotify.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include "Core/TroopsEventEditor/TroopsEVEditor.hpp"
#include "Core/TroopsEventEditor/TroopsEVPage.hpp"
#include "Database/EventCommands/EventDummy.hpp"
#include "Database/EventParser.hpp"
#include "Database/Troop.hpp"

#include <format>
#include <string>

#include "Database/Database.hpp"
#include "imgui.h"
#include "imgui_internal.h"

void TroopsEVEditor::fixupPages() {
  for (int i = 0; i < m_troop->m_pages.size(); ++i) {
    m_pages[i].setPage(&m_troop->m_pages[i], i + 1);
    m_pages[i].setParent(this);
  }
}

bool TroopsEVEditor::draw() {
  if (m_troop) {
    ImGui::BeginGroup();
    {
      if (ImGui::Button("Copy\nEvent Page")) {}
      ImGui::BeginDisabled(true);
      if (ImGui::Button("Paste\nEvent Page")) {}
      ImGui::EndDisabled();
      ImGui::BeginDisabled(m_pages.size() <= 1);
      if (ImGui::Button("Delete\nEvent Page")) {
        erased = true;
        erasedIdx = m_selectedPage;
      }
      ImGui::EndDisabled();
      if (ImGui::Button("Clear\nEvent Page")) {
        if (m_selectedPage != -1) {
          m_pages[m_selectedPage].clearPage();
        }
      }
      ImGui::EndGroup();
    }
    
    ImGui::SameLine();
    if (ImGui::BeginChild("##orpg_troop_editor_page_content", ImVec2(0, 0), false)) {
      if (ImGui::BeginTabBar("##orpg_troop_editor_page", ImGuiTabBarFlags_AutoSelectNewTabs)) {
        for (int i = 0; i < m_troop->m_pages.size(); ++i) {
          if (ImGui::BeginTabItem(std::to_string(i + 1).c_str())) { // Check if BeginTabItem is successful
            m_selectedPage = i;
            m_pages[m_selectedPage].draw(false, i);
            ImGui::EndTabItem();
          }
        }

        // Handle TabItemButton
        if (ImGui::TabItemButton(" + ")) {
          Troop::Page* page = &m_troop->m_pages.emplace_back();
          page->list.emplace_back(new EventDummy());
          m_pages.emplace_back(this, page);
          fixupPages();
          m_selectedPage = m_pages.size() - 1;
        }

        ImGui::EndTabBar(); // Ensure EndTabBar is always called
      }
      ImGui::EndChild();
    }

    if (erased) {
      m_troop->m_pages.erase(m_troop->m_pages.begin() + erasedIdx);
      if (erasedIdx == m_selectedPage) {
        m_selectedPage = -1;
      }
      fixupPages();
      erased = false;
    }
  }
  return m_isOpen;
}
