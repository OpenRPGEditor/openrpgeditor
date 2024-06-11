#include "Core/EventEditor.hpp"

#include "DPIHandler.hpp"
#include "ImGuiUtils.hpp"
#include "Database/Event.hpp"
#include "Database/EventPage.hpp"

#include <format>
#include <string>

#include "imgui.h"
#include "imgui_internal.h"

EventEditor::EventEditor(Project* parent, Event* event) : m_parent(parent), m_event(event) {
  for (auto& page : event->pages) {
    m_pages.emplace_back(this, &page);
  }
}

bool EventEditor::draw() {
  if (m_event) {
    std::string title = std::format("Event {} - ID {}", m_event->name, m_event->id);
    if (ImGui::Begin(title.c_str(), &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
      ImGui::BeginGroup();
      {
        char tmpName[4096];
        float oldY = ImGui::GetCursorPosY();
        strncpy(tmpName, m_event->name.c_str(), 4096);
        if (ImGui::LabelOverLineEdit("##orpg_event_editor_event_name", "Name:", tmpName, 4096,
                                     (100 * App::DPIHandler::get_scale()))) {
          m_event->name = tmpName;
        }
        ImGui::SameLine();
        ImGui::SetCursorPosY(oldY - App::DPIHandler::scale_value(4));
        strncpy(tmpName, m_event->note.c_str(), 4096);
        if (ImGui::LabelOverLineEdit("##orpg_event_editor_event_note", "Note:", tmpName, 4096,
                                     (100 * App::DPIHandler::get_scale()))) {
          m_event->note = tmpName;
        }
      }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        ImGui::SameLine();
        if (ImGui::Button("New\nEvent Page")) {}
        ImGui::SameLine();
        if (ImGui::Button("Copy\nEvent Page")) {}
        ImGui::SameLine();
        /* TODO: Undo/Clipboard stack */
        ImGui::BeginDisabled(true);
        if (ImGui::Button("Paste\nEvent Page")) {}
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::BeginDisabled(m_pages.size() <= 1);
        if (ImGui::Button("Delete\nEvent Page")) {}
        ImGui::EndDisabled();
        ImGui::SameLine();
        if (ImGui::Button("Clear\nEvent Page")) {
          if (m_selectedPage) {
            m_selectedPage->clearPage();
          }
        }
      }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      if (ImGui::BeginTabBar("##orpg_event_editor_page")) {
        int idx = 0;
        std::erase_if(m_pages, [&](EVPage& page) {
          bool del = page.draw(m_pages.size() > 1, idx);
          ++idx;
          if (!del) {
            m_selectedPage = &page;
          }
          return del;
        });
        ImGui::EndTabBar();
      }
      ImGui::EndGroup();
    }
    ImGui::End();
  }
  return m_isOpen;
}
