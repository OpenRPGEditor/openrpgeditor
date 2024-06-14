#include "Core/EventEditor/EVPage.hpp"
#include "Core/EventEditor.hpp"
#include "Core/Project.hpp"

#include <string>
#include <format>

#include "imgui.h"
#include "Core/Application.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/ImGuiParsedText.hpp"
#include "Core/ImGuiUtils.hpp"
#include "Database/EventPage.hpp"

EVPage::EVPage(EventEditor* parent, EventPage* page)
: m_parent(parent), m_page(page), m_commandEditor(parent->project()), m_characterSheet(page->image.characterName) {
  m_commandEditor.setCommands(&m_page->list);
}

std::tuple<bool, bool> EVPage::draw(bool canDelete, int index) {
  bool open = true;
  bool* p_open = canDelete ? &open : nullptr;
  std::string title = m_page->name.empty() ? std::format("Page {}", std::to_string(index + 1)) : m_page->name;
  bool selected = ImGui::BeginTabItem(title.c_str(), p_open);
  if (selected) {
    ImGui::SetNextItemWidth(App::DPIHandler::scale_value(250));
    ImGui::BeginGroup();
    {
      ImGui::BeginChild("##event_page_settings_panel", ImVec2{App::DPIHandler::scale_value(350), 0.f});
      {
        char nameBuf[4096];
        strncpy(nameBuf, m_page->name.c_str(), 4096);
        if (ImGui::LabelOverLineEdit(
                "##event_page_name_edit", "Page Name:", nameBuf, 4096, App::DPIHandler::scale_value(150),
                "Page names are an OpenRPGMaker addition and will not be viewable in RPG Maker MV/MZ")) {
          m_page->name = nameBuf;
        }

        ImGui::BeginGroup();
        {
          ImGui::SeparatorText("Conditions");
          ImGui::BeginGroup();
          {
            ImGui::PushID("##event_page_switch1_selection_check");
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 2);
            ImGui::Checkbox("Switch", &m_page->conditions.switch1Valid);
            ImGui::PopID();
            ImGui::SameLine();
            ImGui::BeginDisabled(!m_page->conditions.switch1Valid);
            ImGui::PushID("##event_page_switch1_selection_button");
            std::string label = m_page->conditions.switch1Valid
                                    ? m_parent->project()->switche(m_page->conditions.switch1Id)
                                    : "##event_page_switch1_selection_button_text";
            if (ImGui::Button(label.c_str(),
                              ImVec2{ImGui::GetContentRegionAvail().x - App::DPIHandler::scale_value(15), 0})) {
              // TODO: Implement switch selection window;
            }
            ImGui::PopID();
            ImGui::EndDisabled();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::PushID("##event_page_switch2_selection_check");
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - App::DPIHandler::scale_value(15));
            ImGui::Checkbox("Switch", &m_page->conditions.switch2Valid);
            ImGui::PopID();
            ImGui::SameLine();
            ImGui::BeginDisabled(!m_page->conditions.switch2Valid);
            ImGui::PushID("##event_page_switch2_selection_button");
            std::string label = m_page->conditions.switch2Valid
                                    ? m_parent->project()->switche(m_page->conditions.switch2Id)
                                    : "##event_page_switch2_selection_button_text";
            if (ImGui::Button(label.c_str(),
                              ImVec2{ImGui::GetContentRegionAvail().x - App::DPIHandler::scale_value(15), 0})) {
              // TODO: Implement switch selection window;
            }
            ImGui::PopID();
            ImGui::EndDisabled();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::PushID("##event_page_variable_selection_check");
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - App::DPIHandler::scale_value(15));
            ImGui::Checkbox("Variable", &m_page->conditions.variableValid);
            ImGui::PopID();
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::BeginDisabled(!m_page->conditions.variableValid);
            {
              ImGui::PushID("##event_page_variable_selection_button");
              std::string label = m_page->conditions.variableValid
                                      ? m_parent->project()->variable(m_page->conditions.variableId)
                                      : "##event_page_variable_selection_button_text";
              if (ImGui::Button(label.c_str(),
                                ImVec2{ImGui::GetContentRegionAvail().x - App::DPIHandler::scale_value(15), 0})) {
                // TODO: Implement switch selection window;
              }
              ImGui::PopID();
              ImGui::Text("≥");
              ImGui::SameLine();
              if (ImGui::InputInt("##event_page_variable_selection_button_text", &m_page->conditions.variableValue)) {
                m_page->conditions.variableValue = std::clamp(m_page->conditions.variableValue, 0, 99999999);
              }
            }
            ImGui::EndDisabled();
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Checkbox("Self Switch", &m_page->conditions.selfSwitchValid);
              ImGui::BeginDisabled(!m_page->conditions.selfSwitchValid);
              ImGui::SameLine();
              std::string sw = m_page->conditions.selfSwitchValid ? m_page->conditions.selfSwitchCh : "";
              if (ImGui::BeginCombo("##event_page_self_switch_selection_combo", sw.c_str())) {
                for (int i = 0; i < 4; ++i) {
                  std::string label = std::format("{}", static_cast<char>('A' + i));
                  if (ImGui::Selectable(label.c_str(), !m_page->conditions.selfSwitchCh.compare(label))) {
                    m_page->conditions.selfSwitchCh = label;
                  }
                }
                ImGui::EndCombo();
              }

              ImGui::EndDisabled();
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
        }
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
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_event_page_commands");
    { m_commandEditor.draw(); }
    ImGui::EndChild();
    ImGui::EndTabItem();
  }

  open ^= 1;
  return std::make_tuple(open, selected);
}

void EVPage::clearPage() {
  m_page->list.clear();
  m_page->list.emplace_back(new EventDummy())->indent = 0;
}
