#include "Core/EventEditor.hpp"

#include "Core/DPIHandler.hpp"
#include "Core/ImGuiExt/ImGuiNotify.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include "Database/Event.hpp"
#include "Database/EventCommands/EventDummy.hpp"
#include "Database/EventPage.hpp"
#include "Database/EventParser.hpp"

#include <format>
#include <string>

#include "Database/Database.hpp"
#include "imgui.h"
#include "imgui_internal.h"

std::tuple<bool, bool> EventEditor::draw() {
  if (!m_event || !m_open) {
    return {!m_open, m_confirmed};
  }

  std::string title = std::format("Event {} - ID {}##event_editor_{}_{}", m_event->name(), m_event->id(), Database::instance()->mapInfos.currentMap()->id(), m_event->id());
  ImGui::SetNextWindowSize(ImVec2{640, 480}, ImGuiCond_Once);
  if (ImGui::Begin(title.c_str(), &m_open)) {
    ImGui::BeginGroup();
    {
      char tmpName[4096];
      float oldY = ImGui::GetCursorPosY();
      strncpy(tmpName, m_event->name().c_str(), 4096);
      if (ImGui::LabelOverLineEdit("##orpg_event_editor_event_name", "Name:", tmpName, 4096, (200))) {
        m_event->setName(tmpName);
      }
      ImGui::SameLine();
      ImGui::SetCursorPosY(oldY - 4);
      strncpy(tmpName, m_event->note().c_str(), 4096);
      if (ImGui::LabelOverLineEdit("##orpg_event_editor_event_note", "Note:", tmpName, 4096, (200 ))) {
        m_event->setNote(tmpName);
      }
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::SameLine();
      if (ImGui::Button("New\nEvent Page")) {
        m_event->addPage({});
        m_selectedPage = m_event->pageCount() - 1;
      }
      ImGui::SameLine();
      if (ImGui::Button("Copy\nEvent Page")) {}
      ImGui::SameLine();
      /* TODO: Undo/Clipboard stack */
      ImGui::BeginDisabled(true);
      if (ImGui::Button("Paste\nEvent Page")) {}
      ImGui::EndDisabled();
      ImGui::SameLine();
      ImGui::BeginDisabled(m_event->pageCount() <= 1);
      if (ImGui::Button("Delete\nEvent Page")) {}
      ImGui::EndDisabled();
      ImGui::SameLine();
      if (ImGui::Button("Clear\nEvent Page")) {
        if (m_selectedPage != -1) {
          m_event->page(m_selectedPage)->clear();
        }
      }
      ImGui::SameLine();
      if (ImGui::Button("Save as\nTemplate")) {
        m_templatePicker = ObjectPicker("Templates"sv, Database::instance()->templates.templateList(Template::TemplateType::Event), 0);
      }
    }
    if (m_templatePicker) {
      auto [closed, confirmed] = m_templatePicker->draw();
      if (closed) {
        if (confirmed) {
          nlohmann::ordered_json eventJson;
          EventParser parser;
          EventParser::serialize(eventJson, *m_event);
          if (m_templatePicker.value().selection() == 0) {
            Database::instance()->templates.addTemplate(Template(Database::instance()->templates.templates.size() + 1,
                                                                 "New Event Template " + std::to_string(Database::instance()->templates.templates.size() + 1), "", Template::TemplateType::Event,
                                                                 eventJson.dump(), {}));
          } else {
            Database::instance()->templates.templates.at(m_templatePicker.value().selection() - 1).setCommands(eventJson.dump());
          }
          if (Database::instance()->templates.serialize(Database::instance()->basePath + "data/Templates.json")) {
            ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Success, "Saved event as template successfully!"});
          } else {
            ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Error, "Failed to saved event as template!"});
          }
          Database::instance()->templates.templates.back().setCommands(eventJson.dump());
        }
        m_templatePicker.reset();
      }
    }
    ImGui::EndGroup();
    ImGui::BeginGroup();
    if (ImGui::BeginTabBar("##orpg_event_editor_page", ImGuiTabBarFlags_AutoSelectNewTabs)) {
      int idx = 0;
      int erasedIdx = idx;
      bool erased = std::ranges::any_of(m_event->pages().begin(), m_event->pages().end(), [&](const EventPage& page) {
        auto [del, selected] = page.editor()->draw(m_event->pageCount() > 1, idx);
        if (del) {
          erasedIdx = idx;
          erased = true;
        }

        if (selected && !del) {
          m_selectedPage = idx;
          m_event->renderer()->setPage(idx);
        }
        ++idx;
        return del;
      });
      if (erased) {
        m_event->removePage(erasedIdx);
        if (erasedIdx == m_selectedPage) {
          m_selectedPage = -1;
          m_event->renderer()->setPage(0);
        }
      }

      if (ImGui::TabItemButton(" + ")) {
        m_event->addPage({});
        m_selectedPage = m_event->pageCount() - 1;
        m_event->renderer()->setPage(m_selectedPage);
      }
      ImGui::EndTabBar();
    }
    ImGui::EndGroup();
  }
  ImGui::End();
  return {!m_open, m_confirmed};
}

IEventEditor* IEventEditor::create(Event* ev) { return new EventEditor(ev); }
