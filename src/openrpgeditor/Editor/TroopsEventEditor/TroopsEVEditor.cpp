#include "Editor/EventEditor.hpp"

#include "Editor/ImGuiExt/ImGuiNotify.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"

#include "Database/EventCommands/EventDummy.hpp"
#include "Database/EventParser.hpp"
#include "Database/Troop.hpp"
#include "Editor/TroopsEventEditor/TroopsEVEditor.hpp"
#include "Editor/TroopsEventEditor/TroopsEVPage.hpp"

#include <format>
#include <string>

#include "Database/Database.hpp"
#include "imgui.h"
#include "imgui_internal.h"

void TroopsEVEditor::fixupPages() {
  if (!m_troop) {
    m_pages.clear();
    return;
  }

  for (int i = 0; i < m_troop->pages().size(); ++i) {
    m_pages[i].setPage(&m_troop->pages()[i], i + 1);
    m_pages[i].setParent(this);
  }
}

bool TroopsEVEditor::draw() {
  if (m_troop) {
    ImGui::BeginGroup();
    {
      if (ImGui::Button(trNOOP("Copy\nEvent Page"))) {}
      ImGui::BeginDisabled(true);
      if (ImGui::Button(trNOOP("Paste\nEvent Page"))) {}
      ImGui::EndDisabled();
      ImGui::BeginDisabled(m_pages.size() <= 1);
      if (ImGui::Button(trNOOP("Delete\nEvent Page"))) {
        m_erased = true;
        m_erasedIdx = m_selectedPage;
      }
      ImGui::EndDisabled();
      if (ImGui::Button(trNOOP("Clear\nEvent Page"))) {
        if (m_selectedPage != -1) {
          m_pages[m_selectedPage].clearPage();
        }
      }
      ImGui::EndGroup();
    }

    ImGui::SameLine();
    if (ImGui::BeginChild("##orpg_troop_editor_page_content", ImVec2(0, 0), false)) {
      if (ImGui::BeginTabBar("##orpg_troop_editor_page", ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_TabListPopupButton | ImGuiTabBarFlags_FittingPolicyScroll)) {
        for (int i = 0; i < m_troop->pages().size(); ++i) {
          if (ImGui::BeginTabItem(std::to_string(i + 1).c_str())) { // Check if BeginTabItem is successful
            m_selectedPage = i;
            m_pages[m_selectedPage].draw(false, i);
            ImGui::EndTabItem();
          }
        }

        // Handle TabItemButton
        if (ImGui::TabItemButton(" + ")) {
          Troop::Page* page = &m_troop->pages().emplace_back();
          page->list().emplace_back(new EventDummy());
          m_pages.emplace_back(this, page, m_troop->id());
          fixupPages();
          m_selectedPage = m_pages.size() - 1;
        }

        ImGui::EndTabBar(); // Ensure EndTabBar is always called
      }
      ImGui::EndChild();
    }

    if (m_erased) {
      m_troop->pages().erase(m_troop->pages().begin() + m_erasedIdx);
      if (m_erasedIdx == m_selectedPage) {
        m_selectedPage = -1;
      }
      fixupPages();
      m_erased = false;
    }
  }
  return m_isOpen;
}
