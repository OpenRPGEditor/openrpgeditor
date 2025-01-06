#include "Core/EventEditor.hpp"

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
  ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size / 2, ImGuiCond_Once);
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Once, {0.5f, 0.5f});
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
      if (ImGui::LabelOverLineEdit("##orpg_event_editor_event_note", "Note:", tmpName, 4096, (200))) {
        m_event->setNote(tmpName);
      }
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      if (ImGui::Button("Copy\nEvent Page")) {}
      ImGui::SameLine();
      /* TODO: Undo/Clipboard stack */
      ImGui::BeginDisabled(true);
      if (ImGui::Button("Paste\nEvent Page")) {}
      ImGui::EndDisabled();
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
    if (ImGui::BeginTabBar("##orpg_event_editor_page" /*, ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable*/)) {
      auto curTabBar = ImGui::GetCurrentTabBar();
      int erasedIdx = 0;
      bool erased = false;
      const ImGuiTabItem* curTab = nullptr;
      int indexDuringRender = m_selectedPage;
      for (int i = 0; i < m_event->pageCount(); ++i) {
        if (!m_event->page(i)) {
          continue;
        }
        if (i == m_selectedPage && !m_event->page(i)->editor()->isSelected()) {
          m_event->page(i)->editor()->select();
        }
        auto [del, selected] = m_event->page(i)->editor()->draw(m_event->pageCount() > 1, i);
        if (i == m_selectedPage) {
          curTab = ImGui::TabBarGetCurrentTab(curTabBar);
          indexDuringRender = i;
        }

        if (del) {
          erasedIdx = i;
          erased = true;
        }

        if (selected && !del) {
          m_selectedPage = i;
          m_event->renderer()->setPage(i);
        }
      }
      // /* Handle re-ordering, we want to handle this as soon as we're out of the loop to capture the page index asap */
      // const auto orderedTab = ImGui::TabBarFindTabByOrder(curTabBar, indexDuringRender);
      // if (curTab && orderedTab && curTab != orderedTab && orderedTab->IndexDuringLayout != -1 && curTab->IndexDuringLayout != -1 && curTab->IndexDuringLayout < m_event->pageCount() &&
      //     orderedTab->IndexDuringLayout < m_event->pageCount()) {
      //   m_event->swapPages(orderedTab->IndexDuringLayout, curTab->IndexDuringLayout);
      //   printf("Tabs reordered %i -> %i!\n", orderedTab->IndexDuringLayout, curTab->IndexDuringLayout);
      //   m_selectedPage = curTab->IndexDuringLayout;
      //   //  TODO: We need to add a confirmation prompt and ok/cancel buttons to the dialog, but for now just accept changes
      //   m_event->acceptChanges();
      // }

      if (ImGui::TabItemButton(" + ", ImGuiTabItemFlags_Trailing)) {
        m_event->addPage({});
        m_selectedPage = m_event->pageCount() - 1;
        m_event->renderer()->setPage(m_selectedPage);
      }
      if (erased) {
        m_event->removePage(erasedIdx);
        if (erasedIdx == m_selectedPage) {
          m_selectedPage--;
          if (m_selectedPage >= 0) {
            m_event->renderer()->setPage(m_selectedPage);
          } else {
            m_selectedPage = 0;
            m_event->renderer()->setPage(0);
          }
        } else if (m_selectedPage > erasedIdx) {
          m_selectedPage--;
          m_event->renderer()->setPage(m_selectedPage);
        }
      }
      ImGui::EndTabBar();
    }
    ImGui::EndGroup();
  }
  ImGui::End();
  return {!m_open, m_confirmed};
}

IEventEditor* IEventEditor::create(Event* ev) { return new EventEditor(ev); }
