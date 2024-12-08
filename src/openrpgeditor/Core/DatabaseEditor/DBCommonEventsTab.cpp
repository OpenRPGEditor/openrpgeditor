#include "Core/DatabaseEditor/DBCommonEventsTab.hpp"

#include "Core/Application.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/DatabaseEditor.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Utils.hpp"

#include "Database/CommonEvents.hpp"

#include "imgui.h"

#include <string>

DBCommonEventsTab::DBCommonEventsTab(CommonEvents& commonEvents, DatabaseEditor* parent) : IDBEditorTab(parent), m_events(commonEvents), m_commandEditor(parent->project()) {
  m_selectedCommonEvent = m_events.event(1);
  m_commandEditor.setCommands(&m_selectedCommonEvent->commands());
}

void DBCommonEventsTab::draw() {
  ImGui::BeginChild("##orpg_commonevents_editor");
  {
    ImGui::BeginChild("##orpg_commonevents_editor_commonevents", ImVec2{250.f, 0} * App::DPIHandler::get_ui_scale(), 0, ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Common Events");
        ImGui::BeginChild("##orpg_commonevents_editor_commonevent_list", ImVec2{0, ImGui::GetContentRegionMax().y - 108 * App::DPIHandler::get_ui_scale()});
        {
          ImGui::BeginGroup();
          {
            for (auto& event : m_events.events()) {
              if (!event) {
                continue;
              }

              CommonEvent& commonEvent = event.value();
              std::string id = "##orpg_commonevent_editor_unnamed_commonevent_" + std::to_string(commonEvent.id());
              ImGui::PushID(id.c_str());
              if (ImGui::Selectable(Database::instance()->commonEventNameAndId(commonEvent.id()).c_str(), &commonEvent == m_selectedCommonEvent) ||
                  (ImGui::IsItemFocused() && m_selectedCommonEvent != &commonEvent)) {
                m_selectedCommonEvent = &commonEvent;
                m_commandEditor.setCommands(&m_selectedCommonEvent->commands());
              }
              ImGui::PopID();
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();

        char str[4096];
        snprintf(str, 4096, "Max Common Events %i", m_events.count());
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8 * App::DPIHandler::get_ui_scale(), 0})) {
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
            if (ImGui::LabelOverLineEdit("##orpg_commonevents_editor_commonevents_commonevent_name", "Name:", name, 4096,
                                         ImGui::GetContentRegionMax().x / 2 / 2 - 16 * App::DPIHandler::get_ui_scale())) {
              m_selectedCommonEvent->setName(name);
            }
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4 * App::DPIHandler::get_ui_scale());
            ImGui::BeginGroup();
            {
              ImGui::Text("Trigger:");
              char buf[4096];
              strncpy(buf, DecodeEnumName(m_selectedCommonEvent->trigger()).c_str(), 4096);
              ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x / 2 / 2 - 16 * App::DPIHandler::get_ui_scale());
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
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4 * App::DPIHandler::get_ui_scale());
            ImGui::BeginGroup();
            {
              ImGui::Text("Switch:");
              // snprintf(buf, 4096, "%04i %s", m_selectedCommonEvent->switchId,
              // m_parent->switches(m_selectedCommonEvent->switchId)); strncpy(buf,
              // m_mapInfos.map(m_selectedCommonEvent.id)->name.c_str(), 4096);
              const bool isSwitchEnabled = m_selectedCommonEvent->trigger() == None;
              const std::string text = isSwitchEnabled ? "##commonevent_switch_empty" : m_parent->switches(m_selectedCommonEvent->switchId());
              ImGui::PushID("##commonevent_button");
              ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x / 2 / 2 - 16 * App::DPIHandler::get_ui_scale());
              ImGui::BeginDisabled(isSwitchEnabled);
              if (ImGui::Button(text.c_str(), ImVec2{ImGui::GetWindowContentRegionMax().x / 2 / 2 - 15 * App::DPIHandler::get_ui_scale(), 0})) {
                // Open Menu to select switch
              }
              ImGui::PopID();
              ImGui::EndDisabled();
            }
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