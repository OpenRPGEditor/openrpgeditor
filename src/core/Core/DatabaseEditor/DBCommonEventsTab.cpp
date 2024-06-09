#include "Core/DatabaseEditor/DBCommonEventsTab.hpp"
#include "Database/CommonEvents.hpp"

#include "imgui.h"
#include "Core/DatabaseEditor.hpp"
#include "Core/ImGuiUtils.hpp"
#include "Database/System.hpp"

DBCommonEventsTab::DBCommonEventsTab(CommonEvents& commonEvents, DatabaseEditor* parent)
: IDBEditorTab(parent), m_events(commonEvents) {
  m_selectedCommonEvent = &m_events.m_events[1];
  m_maxCommonEvents = m_events.m_events.size() - 1;
}

void DBCommonEventsTab::draw() {
  ImGui::BeginChild("##orpg_commonevents_editor");
  {
    ImGui::BeginChild("##orpg_commonevents_editor_commonevents", ImVec2{300.f, 0});
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Common Events");
        ImGui::BeginChild("##orpg_commonevents_editor_commonevent_list",
                          ImVec2{0, ImGui::GetContentRegionMax().y - 96});
        {
          ImGui::BeginGroup();
          {
            for (int i = 1; i < m_events.m_events.size(); ++i) {
              CommonEvent& commonEvent = m_events.m_events[i];
              std::string id = "##orpg_commonevent_editor_unnamed_commonevent_" + std::to_string(commonEvent.id);
              ImGui::PushID(id.c_str());
              if (ImGui::Selectable(commonEvent.name.empty() ? id.c_str() : commonEvent.name.c_str(),
                                    &commonEvent == m_selectedCommonEvent)) {
                m_selectedCommonEvent = &commonEvent;
              }
              ImGui::PopID();
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();

        ImGui::Text("Max Common Events %zu", m_events.m_events.size() - 1);
        ImGui::SameLine();
        if (ImGui::Button("Change Max")) {
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
            strncpy(name, m_selectedCommonEvent->name.c_str(), 4096);
            if (ImGui::LabelOverLineEdit("##orpg_commonevents_editor_commonevents_commonevent_name", "Name:", name,
                                         4096, ((ImGui::GetContentRegionMax().x / 2) / 2) - 16)) {
              m_selectedCommonEvent->name = name;
            }
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
            ImGui::BeginGroup();
            {
              ImGui::Text("Trigger:");
              char buf[4096];
              strncpy(buf, DecodeEnumName(m_selectedCommonEvent->trigger).c_str(), 4096);
              ImGui::SetNextItemWidth(((ImGui::GetContentRegionMax().x / 2) / 2) - 16);
              if (ImGui::BeginCombo("##orpg_commonevents_editor_trigger_combo", buf)) {
                for (const auto& e : magic_enum::enum_values<CommonEventTriggerType>()) {
                  strncpy(buf, DecodeEnumName(e).c_str(), 4096);
                  if (ImGui::Selectable(buf, e == m_selectedCommonEvent->trigger)) {
                    m_selectedCommonEvent->trigger = e;
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
              // snprintf(buf, 4096, "%04i %s", m_selectedCommonEvent->switchId,
              // m_parent->switches(m_selectedCommonEvent->switchId)); strncpy(buf,
              // m_mapInfos.map(m_selectedCommonEvent.id)->name.c_str(), 4096);
              std::string text = m_selectedCommonEvent->switchId > 0
                                     ? "##commonevent_switch_empty"
                                     : m_parent->switches(m_selectedCommonEvent->switchId);
              ImGui::PushID("##commonevent_button");
              ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) / 2 - 16);
              if (ImGui::Button(text.c_str(), ImVec2{((ImGui::GetWindowContentRegionMax().x / 2) / 2) - 15, 0})) {
                // Open Menu to select switch
              }
              ImGui::PopID();
            }
            ImGui::EndGroup();
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
    if (ImGui::Begin("Change Max Actors", &m_changeIntDialogOpen,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal |
                         ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
      ImGui::Text(
          "Specify an amount to resize the actor list to\n"
          "This cannot be undone!");
      ImGui::InputInt("##value_input", &m_maxCommonEvents);
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
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoSavedSettings |
                           ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text("Are you sure?");
        if (ImGui::Button("Yes")) {
          m_events.m_events.resize(m_maxCommonEvents + 1);
          m_changeIntDialogOpen = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          m_changeIntDialogOpen = false;
        }
      }
      ImGui::End();
    }
  }
}