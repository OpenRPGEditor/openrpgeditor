#include <format>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "misc/cpp/imgui_stdlib.h"

static bool ContainsCaseInsensitive(std::string_view str, std::string_view val) {
  return std::search(str.begin(), str.end(), val.begin(), val.end(),
                     [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }) != str.end();
}

template <typename T>
std::tuple<bool, bool> ObjectPicker<T>::draw() {
  std::string title = std::format("{} Selection##{}", m_objectType, reinterpret_cast<uintptr_t>(this));
  ImGui::SetNextWindowSize(ImVec2{720, 640} * App::DPIHandler::get_ui_scale(), ImGuiCond_Once);
  if (ImGui::Begin(title.c_str(), &m_open,
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings)) {

    ImGui::Text("Filter");
    ImGui::SameLine();
    ImGui::InputText("##object_selection_filter_input", &m_filter);
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
      m_filter.clear();
    }

    if (ImGui::BeginTable("Objects", 2,
                          ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable | ImGuiTableFlags_BordersOuter |
                              ImGuiTableFlags_BordersV | ImGuiTableFlags_ScrollY,
                          ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32)})) {

      ImGui::TableSetupColumn("ID",
                              ImGuiTableColumnFlags_PreferSortAscending | ImGuiTableColumnFlags_DefaultSort |
                                  ImGuiTableColumnFlags_WidthFixed,
                              0, 1);
      ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, 0, 2);
      ImGui::TableSetupScrollFreeze(0, 1);
      ImGui::TableHeadersRow();

      ImGuiTableSortSpecs* sortSpecs = ImGui::TableGetSortSpecs();

      bool hasSortSpec = sortSpecs != nullptr && sortSpecs->SpecsCount == 1;

      std::vector<T*> sortedList;
      for (T& value : *m_list) {
        if (!m_filter.empty()) {
          if (ContainsCaseInsensitive(getName(value), m_filter) ||
              ContainsCaseInsensitive(std::to_string(getId(value)), m_filter)) {
            sortedList.push_back(&value);
          }
        } else {
          sortedList.push_back(&value);
        }
      }

      if (hasSortSpec) {
        const auto& spec = sortSpecs->Specs[0];
        if (spec.ColumnUserID == 1) {
          std::sort(sortedList.begin(), sortedList.end(), [&](T* a, T* b) {
            return spec.SortDirection == ImGuiSortDirection_Ascending ? getId(*a) < getId(*b) : getId(*a) > getId(*b);
          });
        } else if (spec.ColumnUserID == 2) {
          std::sort(sortedList.begin(), sortedList.end(), [&](T* a, T* b) {
            int compare = getName(*a).compare(getName(*b));
            return spec.SortDirection == ImGuiSortDirection_Ascending ? compare < 0 : compare > 0;
          });
        }
      }

      for (T* value : sortedList) {
        ImGui::PushID(value);
        ImGui::TableNextRow();

        /* ID */
        if (ImGui::TableNextColumn()) {
          std::string id = std::format("{:04}", getId(*value));
          if (ImGui::Selectable(id.c_str(), m_selection == getId(*value), ImGuiSelectableFlags_SpanAllColumns)) {
            m_selection = getId(*value);
          }
        }
        /* Name */
        if (ImGui::TableNextColumn()) {
          ImGui::Text("%s", getName(*value).c_str());
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
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        m_open = false;
      }
    }
    ImGui::EndGroup();
  }
  ImGui::End();
  return std::make_tuple(!m_open, m_confirmed);
}