#include "Core/DatabaseEditor/DBCommonEventsTab.hpp"

#include "Core/Application.hpp"

#include "Core/DatabaseEditor.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Utils.hpp"

#include "Database/CommonEvents.hpp"

#include "imgui.h"

#include <string>

DBCommonEventsTab::DBCommonEventsTab(CommonEvents& commonEvents, DatabaseEditor* parent)
: IDBEditorTab(parent)
, m_events(commonEvents) {
  m_selectedCommonEvent = m_events.event(1);
  m_commandEditor.setCommands(&m_selectedCommonEvent->commands());

  for (const auto& cmn : m_events.events()) {
    if (cmn.has_value()) {
      if (hasUnicodeFormatting(cmn.value().name())) {
        m_headers.push_back(cmn.value().id());
      }
    }
  }
}

void DBCommonEventsTab::draw() {
  if (picker) {
    auto [closed, confirmed] = picker->draw();
    if (closed) {
      if (confirmed) {
        m_selectedCommonEvent->setSwitchId(picker->selection());
      }
      picker.reset();
    }
  }
  if (preview) {
    auto [closed, confirmed] = preview->draw();
    if (closed) {
      if (confirmed) {}
      preview.reset();
    }
  }

  ImGui::BeginChild("##orpg_commonevents_editor");
  {
    const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
    ImGui::BeginChild("##orpg_commonevents_editor_commonevents", ImVec2{calc.x + (ImGui::GetStyle().ItemSpacing.x * 2), 0}, 0, ImGuiWindowFlags_NoScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Common Events");
        ImGui::BeginChild("##orpg_commonevents_editor_commonevent_list", ImVec2{0, ImGui::GetContentRegionMax().y - ((calc.y + ImGui::GetStyle().ItemSpacing.y) * 4)});
        {
          ImGui::BeginGroup();
          {
            ImGuiListClipper clipper;
            int itemCount = m_events.count() + 1;
            // if (m_parent->isFilteredByCategory()) {
            //   // if (commonEvent.id() <= m_categoryStart || m_categoryEnd == -1 ? true : commonEvent.id() >= m_categoryEnd) {
            //   //   continue;
            //   // }
            // } else {
            //   std::string searchString = m_parent->getFilterString();
            //   if (!searchString.empty()) {
            //     if (commonEvent.name().contains(searchString)) {
            //       continue;
            //     }
            //   }
            // }

            if (m_parent->isFilteredByCategory()) {
              itemCount = (m_categoryEnd - m_categoryStart);
            } else {
              if (m_categoryStart > 0 || m_categoryEnd > 0) {
                m_categoryEnd = 0;
                m_categoryStart = 0;
              }
            }
            clipper.Begin(itemCount, 22.f);
            while (clipper.Step()) {

              for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
                if (m_parent->isFilteredByCategory()) {
                  if (i + m_categoryStart > m_categoryEnd || i + m_categoryStart > m_events.count()) {
                    continue;
                  }
                }
                auto& event = m_events.events()[i + m_categoryStart];

                if (!event) {
                  continue;
                }
                CommonEvent& commonEvent = event.value();

                std::string id = "##orpg_commonevent_editor_unnamed_commonevent_" + std::to_string(commonEvent.id());
                ImGui::PushID(id.c_str());

                bool isFormatted{false};
                if (hasUnicodeFormatting(commonEvent.name())) {
                  auto col = ImGui::GetStyleColorVec4(ImGuiCol_Text);
                  ImGui::PushStyleColor(ImGuiCol_Text, ImColor(1.f - col.x, 1.f - col.y, 1.f - col.z, col.w).Value);
                  col = ImGui::GetStyleColorVec4(ImGuiCol_Header);
                  ImGui::PushStyleColor(ImGuiCol_Header, ImColor(1.f - col.x, 1.f - col.y, 1.f - col.z, col.w * .75f).Value);
                  col = ImGui::GetStyleColorVec4(ImGuiCol_HeaderHovered);
                  ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImColor(1.f - col.x, 1.f - col.y, 1.f - col.z, col.w * .75f).Value);
                  col = ImGui::GetStyleColorVec4(ImGuiCol_HeaderActive);
                  ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImColor(1.f - col.x, 1.f - col.y, 1.f - col.z, col.w * .75f).Value);
                  isFormatted = true;
                }
                if (ImGui::Selectable(Database::instance()->commonEventNameAndId(commonEvent.id()).c_str(), &commonEvent == m_selectedCommonEvent || isFormatted) ||
                    (ImGui::IsItemFocused() && m_selectedCommonEvent != &commonEvent)) {
                  m_selectedCommonEvent = &commonEvent;
                  m_commandEditor.setCommands(&m_selectedCommonEvent->commands());
                }
                if (isFormatted) {
                  ImGui::PopStyleColor(4);
                }

                ImGui::PopID();
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();

        char str[4096];
        snprintf(str, 4096, "Max Common Events %i", m_events.count());
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
          m_changeIntDialogOpen = true;
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_commonevents_editor_commonevents_commonevent_properties");
    {
      if (m_selectedCommonEvent) {
        ImGui::BeginChild("##orpg_commonevents_commonevent_panel", ImVec2{ImGui::GetContentRegionMax().x, 0.f});
        {
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("General Settings");
            char name[4096];
            strncpy(name, m_selectedCommonEvent->name().c_str(), 4096);
            if (ImGui::LabelOverLineEdit("##orpg_commonevents_editor_commonevents_commonevent_name", "Name:", name, 4096, ImGui::GetContentRegionMax().x / 2 / 2 - 16, nullptr,
                                         ImGuiInputTextFlags_None)) {
              m_selectedCommonEvent->setName(name);
            }
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
            ImGui::BeginGroup();
            {
              ImGui::Text("Trigger:");
              char buf[4096];
              strncpy(buf, DecodeEnumName(m_selectedCommonEvent->trigger()).c_str(), 4096);
              ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x / 2 / 2 - 16);
              if (ImGui::BeginCombo("##orpg_commonevents_editor_trigger_combo", buf)) {
                for (const auto& e : magic_enum::enum_values<CommonEventTriggerType>()) {
                  strncpy(buf, DecodeEnumName(e).c_str(), 4096);
                  if (ImGui::Selectable(buf, e == m_selectedCommonEvent->trigger())) {
                    m_selectedCommonEvent->setTrigger(e);
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndGroup();

            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
            ImGui::BeginGroup();
            {
              ImGui::Text("Switch:");
              const bool isSwitchEnabled = m_selectedCommonEvent->trigger() == CommonEventTriggerType::None;
              const std::string text = isSwitchEnabled ? "##commonevent_switch_empty" : Database::instance()->system.switche(m_selectedCommonEvent->switchId());
              ImGui::PushID("##commonevent_button");
              ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x / 2 / 2 - 16);
              ImGui::BeginDisabled(isSwitchEnabled);
              if (ImGui::Button(text.c_str(), ImVec2{ImGui::GetWindowContentRegionMax().x / 2 / 2 - 15, 0})) {
                picker.emplace("Switches", Database::instance()->system.switches(), m_selectedCommonEvent->switchId());
                picker->setOpen(true);
              }
              ImGui::PopID();
              ImGui::EndDisabled();
            }
            ImGui::SameLine();
            // ImGui::SetCursorPosY(ImGui::GetCursorPosY());
            ImGui::BeginGroup();
            {
              if (ImGui::Button("\u25B6", ImVec2{50.f, 0})) {
                preview.emplace(m_selectedCommonEvent->id());
                preview->setOpen(true);
              }
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
            m_commandEditor.draw();
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
      }
    }
    ImGui::EndChild();
  }
  ImGui::EndChild();

  if (m_changeIntDialogOpen) {
    if (ImGui::Begin("Change Max Common Events", &m_changeIntDialogOpen,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
      ImGui::Text(
          "Specify an amount to resize the common event list to\n"
          "This cannot be undone!");
      ImGui::InputInt("##value_input", &m_editMaxCommonEvents);
      ImGui::SameLine();
      if (ImGui::Button("Accept")) {
        m_changeConfirmDialogOpen = true;
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        m_changeIntDialogOpen = false;
      }
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImGui::GetWindowSize() / 2);
    if (m_changeConfirmDialogOpen) {
      if (ImGui::Begin("Confirm Change", &m_changeConfirmDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text("Are you sure?");
        if (ImGui::Button("Yes")) {
          const int tmpId = m_selectedCommonEvent->id();
          m_events.resize(m_editMaxCommonEvents);
          m_selectedCommonEvent = m_events.event(tmpId);
          m_commandEditor.setCommands(&m_selectedCommonEvent->commands());
          m_changeIntDialogOpen = false;
          m_changeConfirmDialogOpen = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          m_changeIntDialogOpen = false;
          m_changeConfirmDialogOpen = false;
        }
      }
      ImGui::End();
    }
  }
}