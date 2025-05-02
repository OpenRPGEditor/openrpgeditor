#include "Core/CommonUI/VariableSwitchPicker.hpp"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <format>

#include <algorithm>

static bool ContainsCaseInsensitive(std::string_view str, std::string_view val) {
  return std::search(str.begin(), str.end(), val.begin(), val.end(), [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }) != str.end();
}

VariableSwitchPicker::VariableSwitchPicker(const std::string_view name, const std::vector<std::string>& values, const int initialSelection, int rangeStart)
: IDialogController(name)
, m_list(&values)
, m_selection(initialSelection) {
  for (int i = rangeStart; i < m_list->size(); ++i) {
    m_trackedValues.emplace_back(i, &(*m_list)[i]);
  }
  if (m_selection > 0) {
    m_navigateOnOpen = true;
  }
}

std::tuple<bool, bool> VariableSwitchPicker::draw() {
  const std::string title = std::format("{} Selection###variable_switch_picker", m_name);
  if (isOpen()) {
    ImGui::OpenPopup("###variable_switch_picker");
  }

  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size / 2, ImGuiCond_Once);
  if (ImGui::BeginPopupModal(title.c_str(), &m_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::Text("Filter");
    ImGui::SameLine();
    ImGui::InputText("##variable_switch_selection_filter_input", &m_filter);
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
      m_filter.clear();
    }
    const auto calc = ImGui::CalcTextSize("OKCANCEL");
    if (ImGui::BeginTable("Objects", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ScrollY,
                          ImVec2{0, ImGui::GetContentRegionAvail().y - (calc.y + (ImGui::GetStyle().ItemSpacing.y * 3) + ImGui::GetStyle().FramePadding.y)})) {

      ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_PreferSortAscending | ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 0, 1);
      ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, 0, 2);
      ImGui::TableSetupScrollFreeze(0, 1);
      ImGui::TableHeadersRow();

      ImGuiTableSortSpecs* sortSpecs = ImGui::TableGetSortSpecs();

      bool hasSortSpec = sortSpecs != nullptr && sortSpecs->SpecsCount == 1;

      std::vector<std::pair<int, const std::string*>> sortedList;
      for (auto& [id, name] : m_trackedValues) {
        if (!m_filter.empty()) {
          if (ContainsCaseInsensitive(*name, m_filter) || ContainsCaseInsensitive(std::to_string(id), m_filter)) {
            sortedList.emplace_back(id, name);
          }
        } else {
          sortedList.emplace_back(id, name);
        }
      }

      if (hasSortSpec) {
        const auto& spec = sortSpecs->Specs[0];
        if (spec.ColumnUserID == 1) {
          std::sort(sortedList.begin(), sortedList.end(), [&](auto& a, auto& b) { return spec.SortDirection == ImGuiSortDirection_Ascending ? a.first < b.first : a.first > b.first; });
        } else if (spec.ColumnUserID == 2) {
          std::sort(sortedList.begin(), sortedList.end(), [&](auto& a, auto& b) {
            int compare = a.second->compare(*b.second);
            return spec.SortDirection == ImGuiSortDirection_Ascending ? compare < 0 : compare > 0;
          });
        }
      }

      for (auto& [id, value] : sortedList) {
        ImGui::PushID(value);
        ImGui::TableNextRow();

        /* ID */
        if (ImGui::TableNextColumn()) {
          std::string idStr = std::format("{:04}", id);
          if (ImGui::Selectable(idStr.c_str(), m_selection == id, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
            m_selection = id;
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_open = false;
              m_confirmed = true;
            }
          }
          if (m_selection == id) {
            ImGui::SetItemDefaultFocus();
            if (m_navigateOnOpen) {
              ImGui::SetScrollHereY();
              m_navigateOnOpen = false;
            }
          }
        }
        /* Name */
        if (ImGui::TableNextColumn()) {
          ImGui::Text("%s", value->c_str());
        }
        ImGui::PopID();
      }

      ImGui::EndTable();
    }
    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        m_confirmed = true;
        m_open = false;
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        m_confirmed = false;
        m_open = false;
        ImGui::CloseCurrentPopup();
      }
    }
    ImGui::EndGroup();
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
