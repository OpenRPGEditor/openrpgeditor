#include "Editor/DatabaseEditor/DBCommonEventsTab.hpp"

#include "Editor/Application.hpp"

#include "Editor/DatabaseEditor.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "Editor/Utils.hpp"

#include "Database/CommonEvents.hpp"

#include "imgui.h"

#include <algorithm>
#include <cctype>
#include <format>
#include <string>
#include <vector>

bool containsInsensitive(std::string_view pool, std::string_view search) {
  if (search.empty()) {
    return true;
  }

  return !std::ranges::search(pool, search, [](const char a, const char b) {
           return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b));
         }).empty();
}

bool matchesCommonEventFilter(const CommonEvent& commonEvent, std::string_view searchString) {
  if (searchString.empty()) {
    return true;
  }

  const int id = commonEvent.id();
  const std::string idText = std::to_string(id);
  return containsInsensitive(commonEvent.name(), searchString) || containsInsensitive(Database::instance()->commonEventNameAndId(id), searchString) || containsInsensitive(idText, searchString);
}

void DBCommonEventsTab::draw() {
  if (picker) {
    auto [closed, confirmed] = picker->draw();
    if (closed) {
      if (confirmed) {
        m_selectedCommonEvent->setSwitchId(picker->selection());
      }
      picker.reset();
    }
  }
  if (preview) {
    auto [closed, confirmed] = preview->draw();
    if (closed) {
      if (confirmed) {}
      preview.reset();
    }
  }

  ImGui::BeginChild("##orpg_commonevents_editor");
  {
    const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
    ImGui::BeginChild("##orpg_commonevents_editor_commonevents", ImVec2{calc.x + (ImGui::GetStyle().ItemSpacing.x * 2), 0}, 0, ImGuiWindowFlags_NoScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Common Events");
        const float footerHeight = ImGui::GetFrameHeightWithSpacing() + ImGui::GetTextLineHeightWithSpacing() * 2.f;
        ImGui::BeginChild("##orpg_commonevents_editor_commonevent_list", ImVec2{0.f, -footerHeight});
        {
          const bool filteredByCategory = DatabaseEditor::instance()->isFilteredByCategory();
          const std::string& searchString = DatabaseEditor::instance()->filterString();
          const std::string listFilterKey = filteredByCategory ? std::format("header:{}:{}", m_categoryStart, m_categoryEnd) : std::format("search:{}", searchString);
          const bool listFilterChanged = listFilterKey != m_lastListFilterKey;

          std::vector<int> visibleIndices;
          if (filteredByCategory) {
            visibleIndices.reserve(m_categoryEnd - m_categoryStart);
            for (int i = m_categoryStart; i < m_categoryEnd; ++i) {
              if (i < 1 || i > m_events->count()) {
                continue;
              }
              if (!m_events->events()[i]) {
                continue;
              }
              visibleIndices.push_back(i);
            }
          } else {
            if (m_categoryStart > 0 || m_categoryEnd > 0) {
              m_categoryEnd = 0;
              m_categoryStart = 0;
            }

            visibleIndices.reserve(m_events->count());
            for (int i = 1; i <= m_events->count(); ++i) {
              const auto& event = m_events->events()[i];
              if (!event) {
                continue;
              }
              if (!matchesCommonEventFilter(event.value(), searchString)) {
                continue;
              }
              visibleIndices.push_back(i);
            }
          }

          for (const int index : visibleIndices) {
            CommonEvent& commonEvent = m_events->events()[index].value();

            std::string id = "##orpg_commonevent_editor_unnamed_commonevent_" + std::to_string(commonEvent.id());
            ImGui::PushID(id.c_str());

            bool isFormatted{false};
            if (hasUnicodeFormatting(commonEvent.name())) {
              auto col = ImGui::GetStyleColorVec4(ImGuiCol_Text);
              ImGui::PushStyleColor(ImGuiCol_Text, ImColor(1.f - col.x, 1.f - col.y, 1.f - col.z, col.w).Value);
              col = ImGui::GetStyleColorVec4(ImGuiCol_Header);
              ImGui::PushStyleColor(ImGuiCol_Header, ImColor(1.f - col.x, 1.f - col.y, 1.f - col.z, col.w * .75f).Value);
              col = ImGui::GetStyleColorVec4(ImGuiCol_HeaderHovered);
              ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImColor(1.f - col.x, 1.f - col.y, 1.f - col.z, col.w * .75f).Value);
              col = ImGui::GetStyleColorVec4(ImGuiCol_HeaderActive);
              ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImColor(1.f - col.x, 1.f - col.y, 1.f - col.z, col.w * .75f).Value);
              isFormatted = true;
            }
            if (ImGui::Selectable(Database::instance()->commonEventNameAndId(commonEvent.id()).c_str(), &commonEvent == m_selectedCommonEvent || isFormatted) ||
                (ImGui::IsItemFocused() && m_selectedCommonEvent != &commonEvent)) {
              m_selectedCommonEvent = &commonEvent;
              m_commandEditor.setCommands(&m_selectedCommonEvent->commands());
            }
            if (isFormatted) {
              ImGui::PopStyleColor(4);
            }

            ImGui::PopID();
          }

          if (listFilterChanged) {
            ImGui::SetScrollY(0.f);
            m_lastListFilterKey = listFilterKey;
          }
        }
        ImGui::EndChild();

        char str[4096];
        snprintf(str, 4096, "Max Common Events %i", m_events->count());
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
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
            if (ImGui::LabelOverLineEdit("##orpg_commonevents_editor_commonevents_commonevent_name", "Name:", name, 4096, ImGui::GetContentRegionMax().x / 2 / 2 - 16, nullptr,
                                         ImGuiInputTextFlags_None)) {
              m_selectedCommonEvent->setName(name);
            }
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
            ImGui::BeginGroup();
            {
              ImGui::Text("Trigger:");
              char buf[4096];
              strncpy(buf, DecodeEnumName(m_selectedCommonEvent->trigger()).c_str(), 4096);
              ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x / 2 / 2 - 16);
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
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
            ImGui::BeginGroup();
            {
              ImGui::Text("Switch:");
              const bool isSwitchEnabled = m_selectedCommonEvent->trigger() == CommonEventTriggerType::None;
              const std::string text = isSwitchEnabled ? "##commonevent_switch_empty" : Database::instance()->system->switche(m_selectedCommonEvent->switchId());
              ImGui::PushID("##commonevent_button");
              ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x / 2 / 2 - 16);
              ImGui::BeginDisabled(isSwitchEnabled);
              if (ImGui::Button(text.c_str(), ImVec2{ImGui::GetWindowContentRegionMax().x / 2 / 2 - 15, 0})) {
                picker.emplace(VariableSwitchPicker::Type::Switch, Database::instance()->system->switches(), m_selectedCommonEvent->switchId());
                picker->setOpen(true);
              }
              ImGui::PopID();
              ImGui::EndDisabled();
            }
            ImGui::SameLine();
            // ImGui::SetCursorPosY(ImGui::GetCursorPosY());
            ImGui::BeginGroup();
            {
              if (ImGui::Button("\u25B6", ImVec2{50.f, 0})) {
                preview.emplace(m_selectedCommonEvent->id());
                preview->setOpen(true);
              }
            }
            ImGui::EndGroup();
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
          m_events->resize(m_editMaxCommonEvents);
          m_selectedCommonEvent = m_events->event(tmpId);
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