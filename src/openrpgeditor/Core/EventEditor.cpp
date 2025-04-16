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
#include <clip.h>

static clip::format OREEventPageFormat = -1;
std::tuple<bool, bool> EventEditor::draw() {
  if (OREEventPageFormat == -1) {
    OREEventPageFormat = clip::register_format("application/ore-EventPage");
  }

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
      if (ImGui::LabelOverLineEdit("##orpg_event_editor_event_name", "Name:", tmpName, 4096, (200), nullptr, ImGuiInputTextFlags_None)) {
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
      if (ImGui::Button(trNOOP("Copy\nEvent Page"))) {
        clip::lock l;
        nlohmann::ordered_json pageJson = *m_event->page(m_selectedPage);
        auto v = pageJson.dump();
        l.set_data(OREEventPageFormat, v.data(), v.size());
      }
      ImGui::SameLine();
      /* TODO: Undo/Clipboard stack */
      ImGui::BeginDisabled(!clip::has(OREEventPageFormat));
      if (ImGui::Button(trNOOP("Paste\nEvent Page"))) {
        clip::lock l;
        if (l.is_convertible(OREEventPageFormat)) {
          auto len = l.get_data_length(OREEventPageFormat);
          std::string page;
          page.resize(len);
          if (l.get_data(OREEventPageFormat, page.data(), len)) {
            nlohmann::ordered_json pageJson = nlohmann::ordered_json::parse(page);
            EventPage pastedPage = pageJson.get<EventPage>();
            m_event->addPage(pastedPage);
          }
        }
      }
      ImGui::EndDisabled();
      ImGui::SameLine();
      if (ImGui::Button(trNOOP("Save as\nTemplate"))) {
        m_templatePicker = ObjectPicker(tr("Templates"), Database::instance()->templates.templateList(Template::TemplateType::Event), 0);
      }
      ImGui::SameLine();
      if (ImGui::Button(trNOOP("Insert\n Locale Keys"))) {
        m_maxLocaleLines = 0;
        for (auto& cmd : m_event->page(m_selectedPage)->list()) {
          if (cmd->hasStringReference("{}", SearchType::Text) && cmd->code() != EventCode::When_Selected) {
            m_maxLocaleLines++;
          }
        }
        ImGui::OpenPopup("Localization");
      }
      if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        {
          ImGui::TextUnformatted(trNOOP(
              "Inserts string keys into all ShowText or ShowChoice commands on the page.\n- Keys must be seperated by line.\n- Key total must equal dialogue total.\nThis feature is temporary until "
              "a proper feature is introduced."));
          ImGui::EndTooltip();
        }
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
                                                                 tr("New Event Template") + " " + std::to_string(Database::instance()->templates.templates.size() + 1), "",
                                                                 Template::TemplateType::Event, eventJson.dump(), {}));
          } else {
            Database::instance()->templates.templates.at(m_templatePicker.value().selection() - 1).setCommands(eventJson.dump());
          }
          if (Database::instance()->templates.serialize(Database::instance()->basePath + "data/Templates.json")) {
            ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Success, trNOOP("Saved event as template successfully!")});
          } else {
            ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Error, trNOOP("Failed to saved event as template!")});
          }
          Database::instance()->templates.templates.back().setCommands(eventJson.dump());
        }
        m_templatePicker.reset();
      }
    }
    ImGui::EndGroup();
    ImGui::BeginGroup();
    if (ImGui::BeginTabBar("##orpg_event_editor_page", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_TabListPopupButton | ImGuiTabBarFlags_FittingPolicyScroll)) {
      int erasedIdx = 0;
      bool erased = false;
      for (int i = 0; i < m_event->pageCount(); ++i) {
        if (!m_event->page(i)) {
          continue;
        }
        if (i == m_selectedPage && !m_event->page(i)->editor()->isSelected()) {
          m_event->page(i)->editor()->select();
        }
        auto [del, selected] = m_event->page(i)->editor()->draw(m_event->pageCount() > 1, i);

        if (del) {
          erasedIdx = i;
          erased = true;
        }

        if (selected && !del) {
          m_selectedPage = i;
          m_event->renderer()->setPage(i);
        }
      }
      const auto curTabBar = ImGui::GetCurrentTabBar();
      std::vector<std::pair<int, int>> reorderTable;
      auto& pages = m_event->pages();
      int selectedPageId = static_cast<EVPage*>(m_event->page(m_selectedPage)->editor())->tabItem()->ID;
      for (int i = 0; i < pages.size(); i++) {
        auto pageEditor = static_cast<EVPage*>(m_event->page(i)->editor());
        for (int j = 0; j < curTabBar->Tabs.size(); j++) {
          int id = curTabBar->Tabs[j].ID == curTabBar->ReorderRequestTabId ? curTabBar->Tabs[curTabBar->Tabs[j].IndexDuringLayout].ID : curTabBar->Tabs[j].ID;
          if (pageEditor->tabItem()->ID == id) {
            if (selectedPageId == pageEditor->tabItem()->ID) {
              m_selectedPage = j;
            }
            reorderTable.emplace_back(id, j);
            break;
          }
        }
      }

      if (!std::ranges::is_sorted(reorderTable, [](const auto& a, const auto& b) { return a.second < b.second; })) {
        std::ranges::sort(pages, [&reorderTable](const EventPage& a, const EventPage& b) {
          auto itA = std::ranges::find_if(reorderTable, [&a](const std::pair<int, int>& a2) { return a2.first == static_cast<EVPage*>(a.editor())->tabItem()->ID; });
          auto itB = std::ranges::find_if(reorderTable, [&b](const std::pair<int, int>& a2) { return a2.first == static_cast<EVPage*>(b.editor())->tabItem()->ID; });
          return itA->second < itB->second;
        });
        m_event->renderer()->setPage(m_selectedPage);
      }

      if (ImGui::TabItemButton(" + ", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoReorder)) {
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
    drawLocalization();
  }
  ImGui::End();

  return {!m_open, m_confirmed};
}
void EventEditor::drawLocalization() {
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{750, 650}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal("Localization", NULL, ImGuiWindowFlags_NoScrollbar)) {
    ImGui::TextUnformatted(m_maxLocaleLines == 0 ? trNOOP("No text found") : std::format("{} / {} keys required", m_localeLinesRequired, m_maxLocaleLines).c_str());
    if (ImGui::InputTextMultiline("##orpg_eventeditor_localization_multiline", &m_localizationInput,
                                  ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y * 5}))

    {
      int count = 0;
      for (char c : m_localizationInput) {
        if (c == '\n') {
          ++count;
        }
      }
      m_localeLinesRequired = count;
    }
    ImGui::BeginGroup();
    {
      ImGui::BeginDisabled(m_localeLinesRequired > m_maxLocaleLines || m_localeLinesRequired < m_maxLocaleLines);
      if (ImGui::Button("OK")) {
        m_localeConfirm = true;
        int index{0};

        int mapId = Database::instance()->mapInfos.currentMap()->id();
        int eventId = id();
        int pageId = m_selectedPage + 1;

        Database::instance()->locales.loadMap(Database::instance()->basePath + std::format("locales/en/Map0{:03}.json", mapId));

        std::vector<std::string> lines = splitString(m_localizationInput, '\n');
        std::vector<std::string> choiceLines;

        int localeIndex{1};
        for (auto& line : lines) {
          for (int i = 0; i < Database::instance()->locales.locales.size(); i++) {
            if (Database::instance()->locales.locales.at(i).first == trim(line)) {
              Database::instance()->locales.locales.at(i).first = std::format("Map{}-EV{}-Page{}-{}", mapId, eventId, pageId, localeIndex);
              localeIndex++;
            }
          }
        }

        for (auto& cmd : m_event->page(m_selectedPage)->list()) {
          if (cmd->code() == EventCode::Show_Choices) {
            if (cmd->hasStringReference("{}", SearchType::Text)) {
              if (!m_choiceParsing) {
                m_choiceParsing = true;
              }

              std::shared_ptr<ShowChoiceCommand> choiceCmd = std::static_pointer_cast<ShowChoiceCommand>(cmd);
              int showChoiceIndex{0};
              for (auto& choiceStr : choiceCmd->choices) {
                // choiceCmd->choices.at(setStringReference("{}", lines.at(index), SearchType::Text);
                int idx = stoi(lines.at(index));
                choiceCmd->choices.at(showChoiceIndex) = "{" + trim(Database::instance()->locales.locales.at(idx).first) + "}";
                if (m_choiceParsing) {
                  choiceLines.insert(choiceLines.begin() + showChoiceIndex, choiceStr);
                } else {
                  choiceLines.push_back(choiceStr);
                }
                showChoiceIndex++;
                index++;
              }
            }
          } else if (cmd->code() == EventCode::Show_Text) {
            if (cmd->hasStringReference("{}", SearchType::Text)) {
              int idx = stoi(lines.at(index));
              if (idx < Database::instance()->locales.locales.size()) {
                cmd->setStringReference("{}", "{" + trim(Database::instance()->locales.locales.at(idx).first) + "}", SearchType::Text);
              }
              index++;
            }
          }
          if (cmd->code() == EventCode::When_Selected) {
            if (cmd->hasStringReference("{}", SearchType::Text)) {
              cmd->setStringReference("", choiceLines.at(0), SearchType::Text);
              choiceLines.erase(choiceLines.begin(), choiceLines.begin() + 1);
            }
          }
          if (cmd->code() == EventCode::End_del_ShowChoices && m_choiceParsing) {
            m_choiceParsing = false;
          }
        }
        m_choiceParsing = false;
        m_maxLocaleLines = 0;
        m_localeLinesRequired = 0;

        Database::instance()->locales.saveCurrentLocale();
        m_localizationInput.clear();
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndDisabled();
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        m_localeConfirm = false;
        m_localizationInput.clear();
        m_maxLocaleLines = 0;
        m_localeLinesRequired = 0;
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndGroup();
    }
    ImGui::EndPopup();
  }
}
IEventEditor* IEventEditor::create(Event* ev) { return new EventEditor(ev); }
