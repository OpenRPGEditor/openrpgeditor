#include "Core/CommonUI/EventCommandEditor.hpp"
#include "Core/Project.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiParsedText.hpp"

#include "imgui.h"
#include "imgui_internal.h"

void EventCommandEditor::draw() {
  ImGui::BeginGroup();
  {
    ImGui::Text("Content:");
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - App::DPIHandler::scale_value(4));
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - App::DPIHandler::scale_value(16));
    ImGui::PushFont(App::APP->getMonoFont());
    static int item_current_idx = 0; // Here we store our selection data as an index.
    // Custom size: use all width, 5 items tall
    if (ImGui::BeginListBox("##commonevent_code_contents", ImVec2(0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(16)))) {
      if (ImGui::BeginChild("##orpg_commonevents_editor_commonevent_list", {}, 0, ImGuiWindowFlags_HorizontalScrollbar)) {
        if (m_commands) {
          for (int n = 0; n < m_commands->size(); n++) {
            const bool is_selected = (item_current_idx == n);
            std::string indentPad = m_commands->at(n)->stringRep();
            if (m_commands->at(n)->code() == EventCode::Common_Event) {
              CommonEventCommand* cec = dynamic_cast<CommonEventCommand*>(m_commands->at(n).get());
              indentPad += m_project->commentEvent(cec->event)->name.c_str();
            } else if (m_commands->at(n)->code() == EventCode::Conditional_Branch) {
              ConditionalBranchCommand* cb = dynamic_cast<ConditionalBranchCommand*>(m_commands->at(n).get());
              if (cb->type == ConditionType::Variable) {
                indentPad.replace(indentPad.find("{"), 2, m_project->variable(cb->variable.id).c_str());
                if (cb->variable.source == VariableComparisonSource::Variable) {
                  indentPad.replace(indentPad.find("{"), 2, m_project->variable(cb->variable.otherId).c_str());
                }
              } else if (cb->type == ConditionType::Switch) {
                indentPad.replace(indentPad.find("{"), 2, m_project->switche(cb->globalSwitch.switchIdx).c_str());
              }
            }

            // ImGui::PushStyleColor(ImGuiCol_Text, m_selectedCommonEvent->commands->at(n)->color());
            auto oldCursor = ImGui::GetCursorPos();
            if (ImGui::Selectable("##common_event_selectable", is_selected, 0,
                                  ImVec2{0, ImGui::CalcTextSize(indentPad.c_str()).y}))
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
        }
        ImGui::EndChild();
      }
      ImGui::EndListBox();
    }
    ImGui::PopFont();
  }
  ImGui::EndGroup();
}