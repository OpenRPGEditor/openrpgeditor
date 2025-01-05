#include "Core/EventEditor/EVPage.hpp"

#include "Core/EventEditor.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/MainWindow.hpp"

#include <format>
#include <string>

#include "../../../../cmake-build-minsizerel/_deps/imgui-src/imgui_internal.h"
#include "imgui.h"

#include "Database/EventPage.hpp"

EVPage::EVPage(EventPage* page) : IPageEditor(page), m_characterSheet(page->image.characterName) {
  m_commandEditor.setCommands(&m_page->list);
  if (m_page->name) {
    strncpy(m_pageNameBuf, m_page->name->c_str(), 4096);
  }
}

std::tuple<bool, bool> EVPage::draw(bool canDelete, int index, bool isSelected) {
  if (!m_actorButton) {
    m_actorButton.emplace();
    m_actorButton->setSize(80, 102);
  }

  if (!m_actorButton->hasCompositeTextures() && m_characterSheet) {
    const auto [min, max] = m_characterSheet.getRectForCharacter(m_page->image.characterIndex, m_page->image.pattern);
    m_actorButton->setTexturesToComposite({{m_characterSheet.texture(),
                                            {m_characterSheet.characterWidth(), m_characterSheet.characterHeight()},
                                            {static_cast<int>(min.x() * m_characterSheet.texture().width()), static_cast<int>(min.y() * m_characterSheet.texture().height())}}});
  } else if (!m_characterSheet) {
    m_actorButton->clear();
  }

  bool open = true;
  bool* p_open = canDelete ? &open : nullptr;
  std::string title;
  if (m_page->name && !m_page->name->empty()) {
    title = std::format("{}##page_{}", *m_page->name, index + 1, reinterpret_cast<uintptr_t>(this));
  } else {
    title = std::format("Page {0}##page_{1}", index + 1, reinterpret_cast<uintptr_t>(this));
  }
  bool selected = ImGui::BeginTabItem(title.c_str(), p_open);
  if (selected) {
    if (ImGui::BeginChild("##event_page_settings_panel", {ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHI").x + (ImGui::GetStyle().FramePadding.x * 2) + ImGui::GetStyle().ItemSpacing.x, 0},
                          0, 0)) {
      ImGui::LabelOverLineEdit("##event_page_name_edit", "Page Name:", m_pageNameBuf, 4096, 150, "Page names are an Open RPG Editor addition and will not be viewable in RPG Maker MV/MZ",
                               ImGuiInputTextFlags_None);
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        ImGui::NewLine();
        if (ImGui::Button("Apply")) {
          m_page->name = m_pageNameBuf;
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear Page")) {
          m_page->clear();
        }
      }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Conditions");
        ImGui::BeginGroup();
        {
          ImGui::Checkbox("Switch##event_page_switch1_selection_check", &m_page->conditions.switch1Valid);
          ImGui::Checkbox("Switch##event_page_switch2_selection_check", &m_page->conditions.switch2Valid);
          ImGui::Checkbox("Variable##event_page_variable_selection_check", &m_page->conditions.variableValid);
          ImGui::NewLine();
          ImGui::Checkbox("Self Switch", &m_page->conditions.selfSwitchValid);
          ImGui::Checkbox("Item", &m_page->conditions.itemValid);
          ImGui::Checkbox("Actor", &m_page->conditions.actorValid);
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          ImGui::BeginDisabled(!m_page->conditions.switch1Valid);
          {
            const std::string label = m_page->conditions.switch1Valid ? Database::instance()->switchNameOrId(m_page->conditions.switch1Id) : "";
            if (ImGui::Button((label + "##event_page_switch1_selection_button").c_str(), ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
              m_variableSwitchSelection = Switch1;
              m_variableSwitchPicker.emplace("Switch", Database::instance()->system.switches, m_page->conditions.switch1Id);
              m_variableSwitchPicker->setOpen(true);
            }
          }
          ImGui::EndDisabled();
          ImGui::BeginDisabled(!m_page->conditions.switch2Valid);
          {
            const std::string label = m_page->conditions.switch2Valid ? Database::instance()->switchNameOrId(m_page->conditions.switch2Id) : "";
            if (ImGui::Button((label + "##event_page_switch2_selection_button").c_str(), ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
              m_variableSwitchSelection = Switch2;
              m_variableSwitchPicker.emplace("Switch", Database::instance()->system.switches, m_page->conditions.switch2Id);
              m_variableSwitchPicker->setOpen(true);
            }
          }
          ImGui::EndDisabled();
          ImGui::BeginGroup();
          {
            ImGui::BeginDisabled(!m_page->conditions.variableValid);
            {
              const std::string label = m_page->conditions.variableValid ? Database::instance()->variableNameOrId(m_page->conditions.variableId) : "";
              if (ImGui::Button((label + "##event_page_variable_selection_button").c_str(), ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
                m_variableSwitchSelection = Variable;
                m_variableSwitchPicker.emplace("Variable", Database::instance()->system.variables, m_page->conditions.variableId);
                m_variableSwitchPicker->setOpen(true);
              }
              auto oldX = ImGui::GetCursorPosX();
              ImGui::SetCursorPosX(oldX - (ImGui::CalcTextSize("≥").x + ImGui::GetStyle().ItemSpacing.x));
              ImGui::Text("≥");
              ImGui::SetCursorPosX(oldX);
              ImGui::SameLine();
              if (ImGui::InputInt("##event_page_variable_value_input_text", &m_page->conditions.variableValue)) {
                m_page->conditions.variableValue = std::clamp(m_page->conditions.variableValue, 0, 99999999);
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(!m_page->conditions.selfSwitchValid);
            {
              const std::string sw = m_page->conditions.selfSwitchValid ? m_page->conditions.selfSwitchCh : "";
              if (ImGui::BeginCombo("##event_page_self_switch_selection_combo", sw.c_str())) {
                for (int i = 0; i < 4; ++i) {
                  std::string label = std::format("{}", static_cast<char>('A' + i));
                  if (ImGui::Selectable(label.c_str(), !m_page->conditions.selfSwitchCh.compare(label))) {
                    m_page->conditions.selfSwitchCh = label;
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(!m_page->conditions.itemValid);
            {
              auto it = Database::instance()->items.item(m_page->conditions.itemId);
              const std::string item = m_page->conditions.itemValid && it != nullptr ? it->name() : "##event_page_item_selection_button_text";
              if (ImGui::Button(item.c_str(), ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
                m_itemPicker.emplace("Items"sv, Database::instance()->items.items(), m_page->conditions.itemId);
                m_itemPicker->setOpen(true);
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(!m_page->conditions.actorValid);
            {
              auto ac = Database::instance()->actors.actor(m_page->conditions.actorId);
              const std::string actor = m_page->conditions.actorValid && ac != nullptr ? ac->name() : "##event_page_actor_selection_button_text";
              if (ImGui::Button(actor.c_str(), ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
                m_actorPicker.emplace("Actors"sv, Database::instance()->actors.actorList(), m_page->conditions.actorId);
                m_actorPicker->setOpen(true);
              }
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
        if (ImGui::ImageButton("##event_image", m_actorButton->get(), static_cast<ImVec2>(m_actorButton->size()))) {
          m_characterPicker.setCharacterInfo(m_page->image.characterName, m_page->image.characterIndex, m_page->image.pattern, m_page->image.direction);
          m_characterPicker.setOpen(true);
        }
      }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Autonomous Movement");
        ImGui::BeginGroup();
        {
          ImGui::BeginGroup();
          {
            ImGui::Text("Type");
            ImGui::NewLine();
            ImGui::NewLine();
            ImGui::Text("Speed");
            ImGui::Text("Freq");
          }
          ImGui::EndGroup();
          ImGui::SameLine();
          ImGui::BeginGroup();
          {
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
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
            if (ImGui::Button("Route...", {ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
              // TODO: Set Movement Route editor
            }
            ImGui::EndDisabled();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
            preview = std::format("{}: {}", magic_enum::enum_index(m_page->moveSpeed).value() + 1, DecodeEnumName(m_page->moveSpeed));
            if (ImGui::BeginCombo("##event_page_auto_movement_speed_combo", preview.c_str())) {
              for (auto v : magic_enum::enum_values<MovementSpeed>()) {
                std::string label = std::format("{}: {}", magic_enum::enum_index(v).value() + 1, DecodeEnumName(v));
                if (ImGui::Selectable(label.c_str(), v == m_page->moveSpeed)) {
                  m_page->moveSpeed = v;
                }
              }
              ImGui::EndCombo();
            }
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
            preview = std::format("{}: {}", magic_enum::enum_index(m_page->moveFrequency).value() + 1, DecodeEnumName(m_page->moveFrequency));
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
          ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
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
          ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
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
    if (const auto [closed, confirmed] = m_characterPicker.draw(); closed) {
      if (confirmed) {
        m_characterPicker.accept();
        m_page->image.characterIndex = m_characterPicker.character();
        const auto tmpName = m_characterPicker.selectedSheet();
        m_page->image.pattern = m_characterPicker.selectedPattern();
        m_page->image.direction = m_characterPicker.selectedDirection();
        if (m_page->image.characterName != tmpName) {
          m_page->image.characterName = tmpName;
          m_characterSheet = CharacterSheet(m_page->image.characterName);
        }
        m_actorButton->clear();
      }
    }

    if (m_variableSwitchPicker) {
      if (auto [closed, confirmed] = m_variableSwitchPicker->draw(); closed) {
        if (confirmed) {
          switch (m_variableSwitchSelection) {
          case Switch1:
            m_page->conditions.switch1Id = m_variableSwitchPicker->selection();
            break;
          case Switch2:
            m_page->conditions.switch2Id = m_variableSwitchPicker->selection();
            break;
          case Variable:
            m_page->conditions.variableId = m_variableSwitchPicker->selection();
            break;
          }
        }
        m_variableSwitchPicker.reset();
      }
    }
    if (m_itemPicker) {
      if (auto [closed, confirmed] = m_itemPicker->draw(); closed) {
        if (confirmed) {
          m_page->conditions.itemId = m_itemPicker->selection();
        }
        m_itemPicker.reset();
      }
    }
    if (m_actorPicker) {
      if (auto [closed, confirmed] = m_actorPicker->draw(); closed) {
        if (confirmed) {
          m_page->conditions.actorId = m_actorPicker->selection();
        }
        m_actorPicker.reset();
      }
    }
    ImGui::EndTabItem();
  }

  return {!open, selected};
}

void EVPage::clearPage() const { m_page->clear(); }

void EVPage::pagePointerInvalidated() {
  m_commandEditor.setCommands(&m_page->list);
  m_actorButton->clear();
}

IPageEditor* IPageEditor::create(EventPage* page) { return new EVPage(page); }