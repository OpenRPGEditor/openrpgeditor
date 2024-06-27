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
            if (ImGui::InputInt("##event_page_variable_value_input_text", &m_page->conditions.variableValue)) {
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
          ImGui::BeginGroup();
          {
            ImGui::Checkbox("Item", &m_page->conditions.itemValid);
            ImGui::SameLine();
            ImGui::BeginDisabled(!m_page->conditions.itemValid);
            auto it = m_parent->project()->item(m_page->conditions.itemId);
            std::string item =
                m_page->conditions.itemValid && it != nullptr ? it->name : "##event_page_item_selection_button_text";
            if (ImGui::Button(item.c_str(),
                              ImVec2{ImGui::GetContentRegionAvail().x - App::DPIHandler::scale_value(15), 0})) {
              // TODO: Implement item selection
            }
            ImGui::EndDisabled();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::Checkbox("Actor", &m_page->conditions.actorValid);
            ImGui::SameLine();
            ImGui::BeginDisabled(!m_page->conditions.actorValid);

            auto ac = m_parent->project()->actor(m_page->conditions.actorId);
            std::string actor =
                m_page->conditions.actorValid && ac != nullptr ? ac->name : "##event_page_actor_selection_button_text";
            if (ImGui::Button(actor.c_str(),
                              ImVec2{ImGui::GetContentRegionAvail().x - App::DPIHandler::scale_value(15), 0})) {
              // TODO: Implement actor selection
            }
            ImGui::EndDisabled();
          }
          ImGui::EndGroup();
        }
        ImGui::EndGroup();
      }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Image");
        ImGui::Dummy(ImVec2{104, 128} * App::DPIHandler::get_ui_scale());
      }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Autonomous Movement");
        ImGui::BeginGroup();
        {
          ImGui::Text("Type");
          ImGui::SameLine();
          ImGui::BeginGroup();
          {
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - App::DPIHandler::scale_value(15));
            auto preview = DecodeEnumName(m_page->moveType);
            if (ImGui::BeginCombo("##event_page_auto_movement_type_combo", preview.c_str())) {
              for (auto v : magic_enum::enum_values<MoveType>()) {
                if (ImGui::Selectable(DecodeEnumName(v).c_str(), v == m_page->moveType)) {
                  m_page->moveType = v;
                }
              }
              ImGui::EndCombo();
            }

            ImGui::BeginDisabled(m_page->moveType != MoveType::Custom);
            if (ImGui::Button("Route...")) {
              // TODO: Set Movement Route editor
            }
            ImGui::EndDisabled();
          }
          ImGui::EndGroup();
          ImGui::Text("Speed");
          ImGui::SameLine();
          ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - App::DPIHandler::scale_value(15));
          auto preview = std::format("{}: {}", magic_enum::enum_index(m_page->moveSpeed).value() + 1,
                                     DecodeEnumName(m_page->moveSpeed));
          if (ImGui::BeginCombo("##event_page_auto_movement_speed_combo", preview.c_str())) {
            for (auto v : magic_enum::enum_values<MovementSpeed>()) {
              std::string label = std::format("{}: {}", magic_enum::enum_index(v).value() + 1, DecodeEnumName(v));
              if (ImGui::Selectable(label.c_str(), v == m_page->moveSpeed)) {
                m_page->moveSpeed = v;
              }
            }
            ImGui::EndCombo();
          }
          ImGui::Text("Freq");
          ImGui::SameLine();
          ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - App::DPIHandler::scale_value(15));
          preview = std::format("{}: {}", magic_enum::enum_index(m_page->moveFrequency).value() + 1,
                                DecodeEnumName(m_page->moveFrequency));
          if (ImGui::BeginCombo("##event_page_auto_movement_freq_combo", preview.c_str())) {
            for (auto v : magic_enum::enum_values<MovementFrequency>()) {
              std::string label = std::format("{}: {}", magic_enum::enum_index(v).value() + 1, DecodeEnumName(v));
              if (ImGui::Selectable(label.c_str(), v == m_page->moveFrequency)) {
                m_page->moveFrequency = v;
              }
            }
            ImGui::EndCombo();
          }
        }
        ImGui::EndGroup();
      }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Options");
        ImGui::Checkbox("Walking", &m_page->walkAnime);
        ImGui::Checkbox("Stepping", &m_page->stepAnime);
        ImGui::Checkbox("Direction Fix", &m_page->directionFix);
        ImGui::Checkbox("Through", &m_page->through);
      }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        ImGui::BeginGroup();
        {
          ImGui::SeparatorText("Priority");
          ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - App::DPIHandler::scale_value(15));
          auto preview = DecodeEnumName(m_page->priorityType);
          if (ImGui::BeginCombo("##event_page_auto_priority_type_combo", preview.c_str())) {
            for (auto v : magic_enum::enum_values<EventPriority>()) {
              if (ImGui::Selectable(DecodeEnumName(v).c_str(), v == m_page->priorityType)) {
                m_page->priorityType = v;
              }
            }
            ImGui::EndCombo();
          }
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          ImGui::SeparatorText("Trigger");
          ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - App::DPIHandler::scale_value(15));
          auto preview = DecodeEnumName(m_page->trigger);
          if (ImGui::BeginCombo("##event_page_auto_trigger_type_combo", preview.c_str())) {
            for (auto v : magic_enum::enum_values<EventTriggerType>()) {
              if (ImGui::Selectable(DecodeEnumName(v).c_str(), v == m_page->trigger)) {
                m_page->trigger = v;
              }
            }
            ImGui::EndCombo();
          }
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
