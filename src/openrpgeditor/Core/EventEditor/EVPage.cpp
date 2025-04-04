#include "Core/EventEditor/EVPage.hpp"

#include "Core/EventEditor.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/MainWindow.hpp"

#include <format>
#include <string>

#include "imgui.h"
#include "imgui_internal.h"

#include "Database/EventPage.hpp"

int EVPage::mNextID = 0;

EVPage::EVPage(EventPage* page) : IPageEditor(page) {
  m_commandEditor.setCommands(&m_page->list());
  if (m_page->name().empty()) {
    strncpy(m_pageNameBuf, m_page->name().data(), 4096);
  }
  if (page->image().tileId() != 0) {
    m_characterSheet = CharacterSheet("", true, page->image().tileId());
  } else if (!page->image().characterName().empty()) {
    m_characterSheet = CharacterSheet(page->image().characterName());
  }
  m_uid = mNextID++;
}

std::tuple<bool, bool> EVPage::draw(bool canDelete, int index) {
  if (m_routeDialog) {
    m_routeDialog->draw();
  }

  if (!m_actorButton) {
    m_actorButton.emplace();
    m_actorButton->setSize(80, 102);
  }

  if (!m_actorButton->hasCompositeTextures() && m_characterSheet) {
    auto [min, _] = m_characterSheet.getRectForCharacter(m_page->image().characterIndex(), m_page->image().pattern(), m_page->image().direction());

    if (m_characterSheet.isTileSheet()) {
      const auto [newMin, _] = m_characterSheet.getRectForTile(m_page->image().tileId());
      min = newMin;
    }
    const auto size = !m_characterSheet.isTileSheet() ? Point{m_characterSheet.characterWidth(), m_characterSheet.characterHeight()} : Point{48, 48};
    m_actorButton->setTexturesToComposite(
        {{m_characterSheet.texture(), size, {static_cast<int>(min.x() * m_characterSheet.texture().width()), static_cast<int>(min.y() * m_characterSheet.texture().height())}}});
  } else if (!m_characterSheet) {
    m_actorButton->clear();
  }

  bool open = true;
  bool* p_open = canDelete ? &open : nullptr;
  std::string title;
  if (!m_page->name().empty()) {
    title = std::format("{}###page_{}", m_page->name(), m_uid);
  } else {
    title = std::format("Page {0}###page_{1}", index + 1, m_uid);
  }

  int flags = 0;
  if (m_selected && m_selectedDirty) {
    flags |= ImGuiTabItemFlags_SetSelected;
    m_selectedDirty = false;
  }
  bool selected = ImGui::BeginTabItem(title.c_str(), p_open, flags);
  m_tabItem = ImGui::TabBarGetCurrentTab(ImGui::GetCurrentTabBar());
  m_layoutIndex = m_tabItem->IndexDuringLayout;
  if (selected) {
    if (ImGui::BeginChild("##event_page_settings_panel", {ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHI").x + (ImGui::GetStyle().FramePadding.x * 2) + ImGui::GetStyle().ItemSpacing.x, 0},
                          ImGuiChildFlags_ResizeX, 0)) {
      if (ImGui::LabelOverLineEdit("##event_page_name_edit", trNOOP("Page Name:"), m_pageNameBuf, 4096, 150,
                                   trNOOP("Page names are an Open RPG Editor addition and will not be viewable in RPG Maker MV/MZ"), ImGuiInputTextFlags_None)) {
        m_page->setName(m_pageNameBuf);
      }
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        ImGui::NewLine();
        if (ImGui::Button(trNOOP("Clear Page"))) {
          m_page->clear();
        }
      }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText(trNOOP("Conditions"));
        ImGui::BeginGroup();
        {
          bool v = m_page->conditions().switch1Valid();
          if (ImGui::Checkbox(std::format("{}##event_page_switch1_selection_check", trNOOP("Switch")).c_str(), &v)) {
            m_page->conditions().setSwitch1Valid(v);
          }
          v = m_page->conditions().switch2Valid();
          if (ImGui::Checkbox(std::format("{}##event_page_switch2_selection_check", trNOOP("Switch")).c_str(), &v)) {
            m_page->conditions().setSwitch2Valid(v);
          }
          v = m_page->conditions().variableValid();
          if (ImGui::Checkbox(std::format("{}##event_page_variable_selection_check", trNOOP("Variable")).c_str(), &v)) {
            m_page->conditions().setVariableValid(v);
          }
          ImGui::NewLine();
          v = m_page->conditions().selfSwitchValid();
          if (ImGui::Checkbox(trNOOP("Self Switch"), &v)) {
            m_page->conditions().setSelfSwitchValid(v);
          }
          v = m_page->conditions().itemValid();
          if (ImGui::Checkbox(trNOOP("Item"), &v)) {
            m_page->conditions().setItemValid(v);
          }
          v = m_page->conditions().actorValid();
          if (ImGui::Checkbox(trNOOP("Actor"), &v)) {
            m_page->conditions().setActorValid(v);
          }
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          ImGui::BeginDisabled(!m_page->conditions().switch1Valid());
          {
            const std::string label = m_page->conditions().switch1Valid() ? Database::instance()->switchNameOrId(m_page->conditions().switch1Id()) : "";
            if (ImGui::Button((label + "##event_page_switch1_selection_button").c_str(), ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
              m_variableSwitchSelection = Switch1;
              m_variableSwitchPicker.emplace(trNOOP("Switch"), Database::instance()->system.switches(), m_page->conditions().switch1Id());
              m_variableSwitchPicker->setOpen(true);
            }
          }
          ImGui::EndDisabled();
          ImGui::BeginDisabled(!m_page->conditions().switch2Valid());
          {
            const std::string label = m_page->conditions().switch2Valid() ? Database::instance()->switchNameOrId(m_page->conditions().switch2Id()) : "";
            if (ImGui::Button((label + "##event_page_switch2_selection_button").c_str(), ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
              m_variableSwitchSelection = Switch2;
              m_variableSwitchPicker.emplace(trNOOP("Switch"), Database::instance()->system.switches(), m_page->conditions().switch2Id());
              m_variableSwitchPicker->setOpen(true);
            }
          }
          ImGui::EndDisabled();
          ImGui::BeginGroup();
          {
            ImGui::BeginDisabled(!m_page->conditions().variableValid());
            {
              const std::string label = m_page->conditions().variableValid() ? Database::instance()->variableNameOrId(m_page->conditions().variableId()) : "";
              if (ImGui::Button((label + "##event_page_variable_selection_button").c_str(), ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
                m_variableSwitchSelection = Variable;
                m_variableSwitchPicker.emplace(trNOOP("Variable"), Database::instance()->system.variables(), m_page->conditions().variableId());
                m_variableSwitchPicker->setOpen(true);
              }
              auto oldX = ImGui::GetCursorPosX();
              ImGui::SetCursorPosX(oldX - (ImGui::CalcTextSize("≥").x + ImGui::GetStyle().ItemSpacing.x));
              ImGui::Text("≥");
              ImGui::SetCursorPosX(oldX);
              ImGui::SameLine();
              int v = m_page->conditions().variableValue();
              ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
              if (ImGui::InputInt("##event_page_variable_value_input_text", &v)) {
                m_page->conditions().setVariableValue(std::clamp(v, 0, 99999999));
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(!m_page->conditions().selfSwitchValid());
            {
              const std::string sw = m_page->conditions().selfSwitchValid() ? m_page->conditions().selfSwitchCh().data() : "";
              ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
              if (ImGui::BeginCombo("##event_page_self_switch_selection_combo", sw.c_str())) {
                for (int i = 0; i < 4; ++i) {
                  std::string label = std::format("{}", static_cast<char>('A' + i));
                  if (ImGui::Selectable(label.c_str(), !m_page->conditions().selfSwitchCh().compare(label))) {
                    m_page->conditions().setSelfSwitchCh(label);
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(!m_page->conditions().itemValid());
            {
              auto it = Database::instance()->items.item(m_page->conditions().itemId());
              const std::string item = m_page->conditions().itemValid() && it != nullptr ? it->name() : "";
              if (ImGui::Button(std::format("{}##event_page_item_selection_button_text", item).c_str(), ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
                m_itemPicker.emplace(trNOOP("Items"), Database::instance()->items.items(), m_page->conditions().itemId());
                m_itemPicker->setOpen(true);
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(!m_page->conditions().actorValid());
            {
              auto ac = Database::instance()->actors.actor(m_page->conditions().actorId());
              const std::string actor = m_page->conditions().actorValid() && ac != nullptr ? ac->name() : "";
              if (ImGui::Button(std::format("{}##event_page_actor_selection_button_text", actor).c_str(), ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
                m_actorPicker.emplace(trNOOP("Actors"), Database::instance()->actors.actorList(), m_page->conditions().actorId());
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
        ImGui::SeparatorText(trNOOP("Image"));
        if (ImGui::ImageButton("##event_image", m_actorButton->get(), static_cast<ImVec2>(m_actorButton->size()))) {
          if (m_page->image().tileId() > 0) {
            m_characterPicker.setTileId(m_page->image().tileId());
          } else {
            m_characterPicker.setCharacterInfo(m_page->image().characterName(), m_page->image().characterIndex(), m_page->image().pattern(), m_page->image().direction());
          }
          m_characterPicker.setOpen(true);
        }
      }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText(trNOOP("Autonomous Movement"));
        ImGui::BeginGroup();
        {
          ImGui::BeginGroup();
          {
            ImGui::TextUnformatted(trNOOP("Type"));
            ImGui::NewLine();
            ImGui::NewLine();
            ImGui::TextUnformatted(trNOOP("Speed"));
            ImGui::TextUnformatted(trNOOP("Freq"));
          }
          ImGui::EndGroup();
          ImGui::SameLine();
          ImGui::BeginGroup();
          {
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
            auto preview = DecodeEnumName(m_page->moveType());
            if (ImGui::BeginCombo("##event_page_auto_movement_type_combo", preview.c_str())) {
              for (auto v : magic_enum::enum_values<MoveType>()) {
                if (ImGui::Selectable(DecodeEnumName(v).c_str(), v == m_page->moveType())) {
                  m_page->setMoveType(v);
                }
              }
              ImGui::EndCombo();
            }

            ImGui::BeginDisabled(m_page->moveType() != MoveType::Custom);
            if (ImGui::Button(trNOOP("Route..."), {ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
              if (!m_routeDialog) {
                m_routeDialog.emplace(DecodeEnumName(EventCode::Set_Movement_Route), m_page);
              }
              m_routeDialog->setOpen(true);
              m_routeDialog->setPage(m_page);
            }
            ImGui::EndDisabled();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
            preview = std::format("{}: {}", magic_enum::enum_index(m_page->moveSpeed()).value() + 1, DecodeEnumName(m_page->moveSpeed()));
            if (ImGui::BeginCombo("##event_page_auto_movement_speed_combo", preview.c_str())) {
              for (auto v : magic_enum::enum_values<MovementSpeed>()) {
                std::string label = std::format("{}: {}", magic_enum::enum_index(v).value() + 1, DecodeEnumName(v));
                if (ImGui::Selectable(label.c_str(), v == m_page->moveSpeed())) {
                  m_page->setMoveSpeed(v);
                }
              }
              ImGui::EndCombo();
            }
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
            preview = std::format("{}: {}", magic_enum::enum_index(m_page->moveFrequency()).value() + 1, DecodeEnumName(m_page->moveFrequency()));
            if (ImGui::BeginCombo("##event_page_auto_movement_freq_combo", preview.c_str())) {
              for (auto v : magic_enum::enum_values<MovementFrequency>()) {
                std::string label = std::format("{}: {}", magic_enum::enum_index(v).value() + 1, DecodeEnumName(v));
                if (ImGui::Selectable(label.c_str(), v == m_page->moveFrequency())) {
                  m_page->setMoveFrequency(v);
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
        ImGui::SeparatorText(trNOOP("Options"));
        bool v = m_page->walkAnime();
        if (ImGui::Checkbox(trNOOP("Walking"), &v)) {
          m_page->setWalkAnime(v);
        }
        v = m_page->stepAnime();
        if (ImGui::Checkbox(trNOOP("Stepping"), &v)) {
          m_page->setStepAnime(v);
        }
        v = m_page->directionFix();
        if (ImGui::Checkbox(trNOOP("Direction Fix"), &v)) {
          m_page->setDirectionFix(v);
        }
        v = m_page->through();
        if (ImGui::Checkbox(trNOOP("Through"), &v)) {
          m_page->setThrough(v);
        }
      }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        ImGui::BeginGroup();
        {
          ImGui::SeparatorText(trNOOP("Priority"));
          ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
          auto preview = DecodeEnumName(m_page->priorityType());
          if (ImGui::BeginCombo("##event_page_auto_priority_type_combo", preview.c_str())) {
            for (auto v : magic_enum::enum_values<EventPriority>()) {
              if (ImGui::Selectable(DecodeEnumName(v).c_str(), v == m_page->priorityType())) {
                m_page->setPriorityType(v);
              }
            }
            ImGui::EndCombo();
          }
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          ImGui::SeparatorText(trNOOP("Trigger"));
          ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
          auto preview = DecodeEnumName(m_page->trigger());
          if (ImGui::BeginCombo("##event_page_auto_trigger_type_combo", preview.c_str())) {
            for (auto v : magic_enum::enum_values<EventTriggerType>()) {
              if (ImGui::Selectable(DecodeEnumName(v).c_str(), v == m_page->trigger())) {
                m_page->setTrigger(v);
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
        if (m_characterPicker.isTilePick()) {
          m_page->image().setTileId(m_characterPicker.tileId());
          m_page->image().setCharacterName("");
          m_page->image().setPattern(m_characterPicker.selectedPattern());
          m_page->image().setDirection(m_characterPicker.selectedDirection());
          m_characterSheet = CharacterSheet(m_characterPicker.selectedTileSheet(), true, m_page->image().tileId());
        } else {
          m_page->image().setCharacterIndex(m_characterPicker.character());
          const auto tmpName = m_characterPicker.selectedSheet();
          m_page->image().setPattern(m_characterPicker.selectedPattern());
          m_page->image().setDirection(m_characterPicker.selectedDirection());
          if (m_page->image().characterName() != tmpName) {
            m_page->image().setCharacterName(tmpName);
            m_characterSheet = CharacterSheet(m_page->image().characterName());
          }
        }
        m_actorButton->clear();
      }
    }

    if (m_variableSwitchPicker) {
      if (auto [closed, confirmed] = m_variableSwitchPicker->draw(); closed) {
        if (confirmed) {
          switch (m_variableSwitchSelection) {
          case Switch1:
            m_page->conditions().setSwitch1Id(m_variableSwitchPicker->selection());
            break;
          case Switch2:
            m_page->conditions().setSwitch2Id(m_variableSwitchPicker->selection());
            break;
          case Variable:
            m_page->conditions().setVariableId(m_variableSwitchPicker->selection());
            break;
          }
        }
        m_variableSwitchPicker.reset();
      }
    }
    if (m_itemPicker) {
      if (auto [closed, confirmed] = m_itemPicker->draw(); closed) {
        if (confirmed) {
          m_page->conditions().setItemId(m_itemPicker->selection());
        }
        m_itemPicker.reset();
      }
    }
    if (m_actorPicker) {
      if (auto [closed, confirmed] = m_actorPicker->draw(); closed) {
        if (confirmed) {
          m_page->conditions().setActorId(m_actorPicker->selection());
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
  m_commandEditor.setCommands(&m_page->list());
  m_actorButton->clear();
}

IPageEditor* IPageEditor::create(EventPage* page) { return new EVPage(page); }
