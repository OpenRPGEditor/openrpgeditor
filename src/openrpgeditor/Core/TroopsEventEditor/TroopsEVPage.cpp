#include "Core/TroopsEventEditor/TroopsEVPage.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/MainWindow.hpp"
#include "Database/Globals.hpp"

#include <format>
#include <string>

#include "imgui.h"

TroopsEVPage::TroopsEVPage(TroopsEVEditor* parent, Troop::Page* page) : m_parent(parent), m_page(page), m_pageNameBuf{}, m_page_id(0) { m_commandEditor.setCommands(&m_page->list); }

std::tuple<bool, bool> TroopsEVPage::draw(bool canDelete, int index) {

  // return std::make_tuple(false, true);
  bool open = true;
  m_page_id = index + 1;

  if (m_actorPicker) {
    if (const auto [closed, confirmed] = m_actorPicker->draw(); closed) {
      if (closed) {
        if (confirmed) {
          m_page->conditions.actorId = m_actorPicker->selection();
        }
        m_actorPicker.reset();
      }
    }
  }
  if (picker) {
    auto [closed, confirmed] = picker->draw();
    if (closed) {
      if (confirmed) {
        m_page->conditions.switchId = m_actorPicker->selection();
      }
      picker.reset();
    }
  }

  ImGui::BeginChild("##orpg_troops_panel_bottom_commands", ImVec2{0, ImGui::GetContentRegionMax().y - 250});
  {
    m_commandEditor.draw();
    ImGui::EndChild();
  }
  ImGui::BeginGroup();
  {
    ImGui::Checkbox("Turn", &m_page->conditions.turnValid);
    ImGui::EndGroup();
  }
  ImGui::BeginGroup();
  {
    ImGui::Checkbox("Turn End", &m_page->conditions.turnEnding);
    ImGui::Checkbox("Enemy HP", &m_page->conditions.enemyValid);
    ImGui::Checkbox("Actor HP", &m_page->conditions.actorValid);
    ImGui::Checkbox("Switch", &m_page->conditions.switchValid);
    ImGui::EndGroup();
  }
  ImGui::SameLine();
  // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 100);
  ImGui::BeginGroup();
  {
    ImGui::BeginDisabled(!m_page->conditions.turnEnding);
    {
      ImGui::SetNextItemWidth(150);
      if (ImGui::InputInt("##orpg_troops_panel_conditions_turnA", &m_page->conditions.turnA)) {
        m_page->conditions.turnA = std::clamp(m_page->conditions.turnA, 0, 9999);
      }
      ImGui::SameLine();
      ImGui::SetNextItemWidth(150);
      if (ImGui::InputInt("##orpg_troops_panel_conditions_turnB", &m_page->conditions.turnB)) {
        m_page->conditions.turnB = std::clamp(m_page->conditions.turnB, 0, 9999);
      }
    }
    ImGui::EndDisabled();
    // Enemy condition
    ImGui::BeginDisabled(!m_page->conditions.enemyValid);
    {
      ImGui::SetNextItemWidth(150);
      std::string title = "#" + std::to_string(m_page->conditions.enemyIndex + 1) + " " +
                          std::string((m_parent->event()->m_members.size() <= m_page->conditions.enemyIndex)
                                          ? "?"
                                          : Database::instance()->enemyNameOrId(m_parent->event()->m_members.at(m_page->conditions.enemyIndex).enemyId));

      if (ImGui::BeginCombo("##orpg_troops_panel_conditions_enemylist", m_page->conditions.enemyValid ? title.c_str() : "")) {
        for (int i = 0; i < 8; ++i) {
          std::string text =
              "#" + std::to_string(i + 1) + " " + std::string((m_parent->event()->m_members.size() <= i) ? "?" : Database::instance()->enemyNameOrId(m_parent->event()->m_members.at(i).enemyId));
          if (ImGui::Selectable(text.c_str(), i == m_page->conditions.enemyIndex)) {
            m_page->conditions.enemyIndex = i;
          }
        }
        ImGui::EndCombo();
      }
      ImGui::SameLine();
      ImGui::SetNextItemWidth(150);
      ImGui::SliderInt("##orpg_troops_panel_conditions_enemyhp", &m_page->conditions.enemyHp, 0, 100, m_page->conditions.enemyValid ? " %d%%" : "");
      ImGui::SameLine();
      ImGui::Text("or below");
    }
    ImGui::EndDisabled();
    // Actor condition
    ImGui::BeginDisabled(!m_page->conditions.actorValid);
    {
      if (ImGui::Button(
              std::string(std::format("{}##orpg_troops_conditions_actor_page_{}", m_page->conditions.actorValid ? Database::instance()->actorNameOrId(m_page->conditions.actorId) : "", m_page_id))
                  .c_str(),
              {(150), 0})) {
        m_actorPicker = ObjectPicker<Actor>("Actor"sv, Database::instance()->actors.actorList(), m_page->conditions.actorId);
        m_actorPicker->setOpen(true);
      }
      ImGui::SameLine();
      ImGui::SetNextItemWidth(150);
      ImGui::SliderInt("##orpg_troops_panel_conditions_actorhp", &m_page->conditions.actorHp, 0, 100, m_page->conditions.actorId > 0 ? " %d%%" : "");
      ImGui::SameLine();
      ImGui::Text("or below");
    }
    ImGui::EndDisabled();
    // Switch condition
    ImGui::BeginDisabled(!m_page->conditions.switchValid);
    {
      if (ImGui::Button(
              std::string(std::format("{}##orpg_troops_conditions_switch_page_{}", m_page->conditions.switchValid ? Database::instance()->switchNameOrId(m_page->conditions.switchId) : "", m_page_id))
                  .c_str(),
              ImVec2{(150), 0})) {
        picker.emplace("Switches", Database::instance()->system.switches, m_page->conditions.switchId);
        picker->setOpen(true);
      }
    }
    ImGui::EndDisabled();
  }
  ImGui::EndGroup();

  ImGui::BeginGroup();
  {
    const auto preview = DecodeEnumName(static_cast<Span>(m_page->span));
    ImGui::SetNextItemWidth(300);
    if (ImGui::BeginCombo("##trait_editor_party_ability_combo", preview.c_str())) {
      for (const auto& type : magic_enum::enum_values<Span>()) {
        if (ImGui::Selectable(DecodeEnumName(type).c_str(), m_page->span == static_cast<int>(type))) {
          m_page->span = static_cast<int>(type);
        }
      }
      ImGui::EndCombo();
    }
    ImGui::EndGroup();
  }

  open ^= 1;
  return std::make_tuple(open, true);
}

void TroopsEVPage::clearPage() {
  m_page->list.clear();
  m_page->list.emplace_back(new EventDummy())->setIndent(0);
}
