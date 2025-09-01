#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <format>

static bool ContainsCaseInsensitive(std::string_view str, std::string_view val) {
  return std::ranges::search(str, val, [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }).begin() != str.end();
}

template <typename T>
std::tuple<bool, bool> ObjectPicker<T>::draw() {
  const std::string id = std::format("###ObjectPicker{}", std::string(Name));
  const std::string title = std::format("{} Selection{}", m_name, id);
  if (isOpen()) {
    ImGui::OpenPopup(id.c_str());
  }

  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size / 2, ImGuiCond_Once);
  if (ImGui::BeginPopupModal(title.c_str(), &m_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::Text("Filter");
    ImGui::SameLine();
    ImGui::InputText("##object_selection_filter_input", &m_filter);
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
      m_filter.clear();
    }

    if (ImGui::BeginTable("Objects", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ScrollY,
                          ImVec2{0, ImGui::GetContentRegionAvail().y - 32 - ImGui::GetStyle().FramePadding.y})) {

      ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_PreferSortAscending | ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 0, 1);
      ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, 0, 2);
      ImGui::TableSetupScrollFreeze(0, 1);
      ImGui::TableHeadersRow();

      ImGuiTableSortSpecs* sortSpecs = ImGui::TableGetSortSpecs();

      bool hasSortSpec = sortSpecs != nullptr && sortSpecs->SpecsCount == 1;

      std::vector<T*> sortedList;
      for (T& value : *m_list) {
        if (getId(value) == 0) {
          continue;
        }
        if (!m_filter.empty()) {
          if (ContainsCaseInsensitive(getName(value), m_filter) || ContainsCaseInsensitive(std::to_string(getId(value)), m_filter)) {
            sortedList.push_back(&value);
          }
        } else {
          sortedList.push_back(&value);
        }
      }

      if (hasSortSpec) {
        const auto& spec = sortSpecs->Specs[0];
        if (spec.ColumnUserID == 1) {
          std::sort(sortedList.begin(), sortedList.end(), [&](T* a, T* b) { return spec.SortDirection == ImGuiSortDirection_Ascending ? getId(*a) < getId(*b) : getId(*a) > getId(*b); });
        } else if (spec.ColumnUserID == 2) {
          std::sort(sortedList.begin(), sortedList.end(), [&](T* a, T* b) {
            int compare = getName(*a).compare(getName(*b));
            return spec.SortDirection == ImGuiSortDirection_Ascending ? compare < 0 : compare > 0;
          });
        }
      }

      if (m_useDummyEntry) {
        ImGui::TableNextRow();
        if (ImGui::TableNextColumn()) {
          if (ImGui::Selectable("##object_picker_dummy_entry", m_selection == 0, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
            m_selection = 0;
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_open = false;
              m_confirmed = true;
            }
          }
        }
        if (ImGui::TableNextColumn()) {
          ImGui::Text("%s", m_entireText.c_str());
        }
      }

      for (T* value : sortedList) {
        ImGui::PushID(value);
        ImGui::TableNextRow();

        /* ID */
        if (ImGui::TableNextColumn()) {
          std::string id = std::format("{:04}", getId(*value));
          if (ImGui::Selectable(id.c_str(), m_selection == getId(*value), ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
            m_selection = getId(*value);
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_open = false;
              m_confirmed = true;
            }
          }
          if (m_selection == getId(*value)) {
            ImGui::SetItemDefaultFocus();
            if (m_navigateOnOpen) {
              ImGui::SetScrollHereY();
              m_navigateOnOpen = false;
            }
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
      if (ImGui::Button(m_noSelectionMeansAdd && m_selection <= 0 ? "Add" : "OK")) {
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
