#include "Core/DatabaseEditor/DBCommonEventsTab.hpp"
#include "Database/CommonEvents.hpp"

#include "imgui.h"
#include "Core/Application.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/DatabaseEditor.hpp"
#include "Core/ImGuiUtils.hpp"
#include "Database/System.hpp"

#include <Core/ImGuiParsedText.hpp>
#include <Core/Utils.hpp>
#include <string>

DBCommonEventsTab::DBCommonEventsTab(CommonEvents& commonEvents, DatabaseEditor* parent)
: IDBEditorTab(parent), m_events(commonEvents) {
  m_selectedCommonEvent = &m_events.m_events[1];
  m_maxCommonEvents = m_events.m_events.size() - 1;
}

void DBCommonEventsTab::draw() {
  ImGui::BeginChild("##orpg_commonevents_editor");
  {
    ImGui::BeginChild("##orpg_commonevents_editor_commonevents", ImVec2{400.f, 0} * App::DPIHandler::get_scale());
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Common Events");
        ImGui::BeginChild("##orpg_commonevents_editor_commonevent_list",
                          ImVec2{0, ImGui::GetContentRegionMax().y - (108 * App::DPIHandler::get_scale())});
        {
          ImGui::BeginGroup();
          {
            for (int i = 0; i < m_events.m_events.size() - 1; ++i) {
              CommonEvent& commonEvent = m_events.m_events[i];
              std::string id = "##orpg_commonevent_editor_unnamed_commonevent_" + std::to_string(commonEvent.id);
              ImGui::PushID(id.c_str());
              char name[4096];
              snprintf(name, 4096, "%04i %s", commonEvent.id, commonEvent.name.c_str());
              if (ImGui::Selectable(name, &commonEvent == m_selectedCommonEvent) ||
                  (ImGui::IsItemFocused() && m_selectedCommonEvent != &commonEvent)) {
                m_selectedCommonEvent = &commonEvent;
              }
              ImGui::PopID();
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();

        char str[4096];
        snprintf(str, 4096, "Max Common Events %zu", m_events.m_events.size() - 1);
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max",
                          ImVec2{ImGui::GetContentRegionMax().x - (8 * App::DPIHandler::get_scale()), 0})) {
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
            if (ImGui::LabelOverLineEdit(
                    "##orpg_commonevents_editor_commonevents_commonevent_name", "Name:", name, 4096,
                    ((ImGui::GetContentRegionMax().x / 2) / 2) - (16 * App::DPIHandler::get_scale()))) {
              m_selectedCommonEvent->name = name;
            }
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (4 * App::DPIHandler::get_scale()));
            ImGui::BeginGroup();
            {
              ImGui::Text("Trigger:");
              char buf[4096];
              strncpy(buf, DecodeEnumName(m_selectedCommonEvent->trigger).c_str(), 4096);
              ImGui::SetNextItemWidth(((ImGui::GetContentRegionMax().x / 2) / 2) - (16 * App::DPIHandler::get_scale()));
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
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (4 * App::DPIHandler::get_scale()));
            ImGui::BeginGroup();
            {
              ImGui::Text("Switch:");
              // snprintf(buf, 4096, "%04i %s", m_selectedCommonEvent->switchId,
              // m_parent->switches(m_selectedCommonEvent->switchId)); strncpy(buf,
              // m_mapInfos.map(m_selectedCommonEvent.id)->name.c_str(), 4096);
              bool isSwitchEnabled = m_selectedCommonEvent->trigger == CommonEventTriggerType::None;
              std::string text =
                  isSwitchEnabled ? "##commonevent_switch_empty" : m_parent->switches(m_selectedCommonEvent->switchId);
              ImGui::PushID("##commonevent_button");
              ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) / 2 - (16 * App::DPIHandler::get_scale()));
              ImGui::BeginDisabled(isSwitchEnabled);
              if (ImGui::Button(text.c_str(), ImVec2{((ImGui::GetWindowContentRegionMax().x / 2) / 2) -
                                                         (15 * App::DPIHandler::get_scale()),
                                                     0})) {
                // Open Menu to select switch
              }
              ImGui::PopID();
              ImGui::EndDisabled();
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Content:");
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (4 * App::DPIHandler::get_scale()));
              ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x - (16 * App::DPIHandler::get_scale()));
              ImGui::PushFont(App::APP->getMonoFont());
              static int item_current_idx = 0; // Here we store our selection data as an index.
              // Custom size: use all width, 5 items tall
              if (ImGui::BeginListBox("##commonevent_code_contents",
                                      ImVec2(0, ImGui::GetContentRegionAvail().y - 16))) {
                for (int n = 0; n < m_selectedCommonEvent->commands.size(); n++) {
                  const bool is_selected = (item_current_idx == n);
                  std::string indentPad = m_selectedCommonEvent->commands[n]->stringRep();
                  if (m_selectedCommonEvent->commands[n]->code() == EventCode::Common_Event) {
                    CommonEventCommand* cec =
                        dynamic_cast<CommonEventCommand*>(m_selectedCommonEvent->commands[n].get());
                    indentPad += m_events.m_events[cec->event].name.c_str();
                  }
                  else if (m_selectedCommonEvent->commands[n]->code() == EventCode::Conditional_Branch) {
                    ConditionalBranchCommand* cb =
                        dynamic_cast<ConditionalBranchCommand*>(m_selectedCommonEvent->commands[n].get());
                    if (cb->type == ConditionType::Variable) {
                      indentPad.replace(indentPad.find("{"), 2, m_parent->variables(cb->variable.id).c_str());
                      if (cb->variable.source == VariableComparisonSource::Variable) {
                        indentPad.replace(indentPad.find("{"), 2, m_parent->variables(cb->variable.otherId).c_str());
                      }
                    }
                    else if (cb->type == ConditionType::Switch) {
                      indentPad.replace(indentPad.find("{"), 2, m_parent->switches(cb->globalSwitch.switchIdx).c_str());
                    }
                  }

                  // ImGui::PushStyleColor(ImGuiCol_Text, m_selectedCommonEvent->commands[n]->color());
                  auto oldCursor = ImGui::GetCursorPos();
                  if (ImGui::Selectable("##common_event_selectable", is_selected, 0, ImVec2{0, ImGui::CalcTextSize(indentPad.c_str()).y}))
                    item_current_idx = n;
                  ImGui::SetCursorPos(oldCursor);
                  /* Nexus: TextParsed takes the syntax of `&<token>[=value];` where value is optional and token is
                   * required.
                   * Currently there are only 3 tokens, push-color, pop-color, and color, push and pop allow you to
                   * control which blocks of text get color, color applies to the whole string
                   * if a token is unsupported it won't get processed and will show up in the resulting text
                   */
                  ImGui::TextParsed("&push-color=255,255,255;%s&pop-color;", indentPad.c_str());
                  // ImGui::PopStyleColor();

                  // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                  if (is_selected)
                    ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
              }
              ImGui::PopFont();
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
    if (ImGui::Begin("Change Max Common Events", &m_changeIntDialogOpen,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal |
                         ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
      ImGui::Text(
          "Specify an amount to resize the common event list to\n"
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
          int tmpId = m_selectedCommonEvent->id;
          m_events.m_events.resize(m_maxCommonEvents + 1);
          m_selectedCommonEvent = m_events.event(tmpId);
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