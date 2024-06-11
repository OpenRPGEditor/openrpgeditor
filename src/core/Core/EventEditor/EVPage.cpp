#include "Core/EventEditor/EVPage.hpp"
#include "Core/EventEditor.hpp"
#include "Core/Project.hpp"

#include <string>

#include "imgui.h"
#include "Core/Application.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/ImGuiParsedText.hpp"
#include "Database/EventPage.hpp"

bool EVPage::draw(bool canDelete, int index) {
  bool open = true;
  bool* p_open = canDelete ? &open : nullptr;
  if (ImGui::BeginTabItem(std::to_string(index + 1).c_str(), p_open)) {
    ImGui::SetNextItemWidth(App::DPIHandler::scale_value(250));
    ImGui::BeginGroup();
    {
      ImGui::BeginGroup();
      { ImGui::SeparatorText("Conditions"); }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      { ImGui::SeparatorText("Image"); }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      { ImGui::SeparatorText("Autonomous Movement"); }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      { ImGui::SeparatorText("Options"); }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        ImGui::BeginGroup();
        { ImGui::SeparatorText("Priority"); }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        { ImGui::SeparatorText("Trigger"); }
        ImGui::EndGroup();
      }
      ImGui::EndGroup();
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::Text("Content:");
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (4 * App::DPIHandler::get_scale()));
      ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x - (16 * App::DPIHandler::get_scale()));
      ImGui::PushFont(App::APP->getMonoFont());
      static int item_current_idx = 0; // Here we store our selection data as an index.
      // Custom size: use all width, 5 items tall
      if (ImGui::BeginListBox("##commonevent_code_contents", ImVec2(0, ImGui::GetContentRegionAvail().y - 16))) {
        for (int n = 0; n < m_page->list.size(); n++) {
          const bool is_selected = (item_current_idx == n);
          std::string indentPad = m_page->list[n]->stringRep();
          if (m_page->list[n]->code() == EventCode::Common_Event) {
            CommonEventCommand* cec = dynamic_cast<CommonEventCommand*>(m_page->list[n].get());
            auto* ce = m_parent->project()->commentEvent(cec->event);
            if (ce) {
              indentPad += ce->name;
            }
          }

          // ImGui::PushStyleColor(ImGuiCol_Text, m_selectedCommonEvent->commands[n]->color());
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
        ImGui::EndListBox();
      }
    }
    ImGui::EndGroup();
    ImGui::EndTabItem();
  }

  return !open;
}

void EVPage::clearPage() { m_page->list.clear(); }
