#include "Editor/CommonUI/VariableSwitchPicker.hpp"

#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <format>

#include <algorithm>
#include <orei18n.hpp>

static bool ContainsCaseInsensitive(std::string_view str, std::string_view val) {
  return std::ranges::search(str, val, [](const char ch1, const char ch2) { return std::toupper(ch1) == std::toupper(ch2); }).begin() != str.end();
}

VariableSwitchPicker::VariableSwitchPicker(const Type type, const std::vector<std::string>& values, const int initialSelection, const int rangeStart)
: VariableSwitchPicker(type == Type::Variable ? tr("Variable Selection") : tr("Switch Selection"), values, initialSelection, rangeStart) {}

VariableSwitchPicker::VariableSwitchPicker(const std::string_view title, const std::vector<std::string>& values, const int initialSelection, const int rangeStart)
: IDialogController(title)
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
  const std::string title = std::format("{}###variable_switch_picker", m_name);
  if (isOpen()) {
    ImGui::OpenPopup("###variable_switch_picker");
  }

  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImGui::GetDPIScaledSize(1024, 960), ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(ImGui::GetDPIScaledSize(320, 240), {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(title.c_str(), &m_open, ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::BeginVertical("##variable_switch_picker_main_layout", {-1, -1}, 0);
    {
      ImGui::BeginHorizontal("##variable_switch_picker_filter", {-1, 0}, 0);
      {
        ImGui::TextUnformatted(trNOOP("Filter"));
        const auto clearText = trNOOP("Clear");
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(clearText).x + ImGui::GetFrameHeight() + ImGui::GetStyle().ItemSpacing.y * 2));
        ImGui::InputText("##variable_switch_selection_filter_input", &m_filter);
        if (ImGui::Button(clearText, {-1, 0})) {
          m_filter.clear();
        }
      }
      ImGui::EndHorizontal();

      if (ImGui::BeginTable("Objects", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ScrollY,
                            ImVec2{0, ImGui::GetContentRegionAvail().y - (ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y * 2)})) {

        ImGui::TableSetupColumn(trNOOP("ID"), ImGuiTableColumnFlags_PreferSortAscending | ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 0, 1);
        ImGui::TableSetupColumn(trNOOP("Name"), ImGuiTableColumnFlags_WidthStretch, 0, 2);
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        const ImGuiTableSortSpecs* sortSpecs = ImGui::TableGetSortSpecs();

        const auto hasSortSpec = sortSpecs != nullptr && sortSpecs->SpecsCount == 1;

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
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##variable_switch_picker_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##variable_switch_picker_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_open = false;
          ImGui::CloseCurrentPopup();
        } else if (ret == 1) {
          m_confirmed = false;
          m_open = false;
          ImGui::CloseCurrentPopup();
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
