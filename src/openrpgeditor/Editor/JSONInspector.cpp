#include "Editor/JSONInspector.hpp"

#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "orei18n.hpp"

#include <fstream>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>


void DrawJsonElement(const std::string& name, nlohmann::ordered_json& element);

void DrawJsonPrimitive(const std::string& name, nlohmann::ordered_json& element) {
  ImGui::PushID(name.c_str());

  switch (element.type()) {
  case nlohmann::ordered_json::value_t::boolean: {
    bool val = element.get<bool>();
    if (ImGui::Checkbox("##value", &val)) {
      element = val;
    }
    break;
  }
  case nlohmann::ordered_json::value_t::number_integer:
  case nlohmann::ordered_json::value_t::number_unsigned:
  case nlohmann::ordered_json::value_t::number_float: {
    std::string s = element.dump();
    ImGui::SetNextItemWidth(-1);
    if (ImGui::InputText("##value", &s, ImGuiInputTextFlags_EnterReturnsTrue)) {
      try {
        if (element.is_number_integer() || element.is_number_unsigned()) {
          element = std::stoll(s.data());
        } else {
          element = std::stod(s.data());
        }
      } catch (...) {
      }
    }
    break;
  }
  case nlohmann::ordered_json::value_t::string: {
    std::string s = element.get<std::string>();
    ImGui::SetNextItemWidth(-1);
    if (ImGui::InputText("##value", &s, ImGuiInputTextFlags_EnterReturnsTrue)) {
      element = std::string(s);
    }
    break;
  }
  case nlohmann::ordered_json::value_t::null: {
    ImGui::Text("null");
    break;
  }
  default:
    ImGui::Text("Unknown type");
    break;
  }

  ImGui::PopID();
}

// Draws a JSON object or array in a table format
void DrawJsonContainer(const std::string& name, nlohmann::ordered_json& container) {
  ImGui::PushID(name.c_str());

  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
  bool open = ImGui::TreeNodeEx("##tree_node", flags, "%s (%s, %zu items)", name.c_str(), container.is_object() ? "object" : "array", container.size());

  if (open) {
    if (ImGui::BeginTable("##json_table", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg)) {
      ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Value");
      ImGui::TableHeadersRow();

      std::vector<std::pair<std::string, nlohmann::ordered_json*>> items;
      if (container.is_object()) {
        for (auto it = container.begin(); it != container.end(); ++it) {
          items.emplace_back(it.key(), &(*it));
        }
      } else if (container.is_array()) {
        for (size_t i = 0; i < container.size(); ++i) {
          items.emplace_back(std::to_string(i), &container[i]);
        }
      }

      for (auto& item : items) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("%s", item.first.c_str());

        ImGui::TableNextColumn();
        DrawJsonElement(item.first, *item.second);
      }
      ImGui::EndTable();
    }
    ImGui::TreePop();
  }
  ImGui::PopID();
}

void DrawJsonElement(const std::string& name, nlohmann::ordered_json& element) {
  if (element.is_primitive()) {
    DrawJsonPrimitive(name, element);
  } else {
    DrawJsonContainer(name, element);
  }
}

std::tuple<bool, bool> JSONInspector::draw() {
  if (!isOpen()) {
    return {!m_open, m_confirmed};
  }

  if (ImGui::Begin(std::format("{}###JSONInspector", trNOOP("JSON Inspector")).c_str())) {
    // Determine how to display the root element based on its type
    if (m_inspectedJson.is_object() || m_inspectedJson.is_array()) {
      DrawJsonContainer("<root>", m_inspectedJson);
    } else if (m_inspectedJson.is_primitive()) {
      // For a primitive root, just show the editable value directly
      DrawJsonPrimitive("<root>", m_inspectedJson);
    }
  }
  ImGui::End();

  return {!m_open, m_confirmed};
}

bool JSONInspector::openFile(const std::filesystem::path& toInspect) {
  if (toInspect == m_path) {
    return true;
  }

  try {
    std::ifstream file{toInspect};
    m_inspectedJson = nlohmann::ordered_json::parse(file);
    m_path = toInspect;
    return true;
  } catch (...) { return false; }
}

bool JSONInspector::saveFile(const std::filesystem::path& toSave) const {
  std::ofstream file{toSave.empty() ? m_path : toSave};

  try {
    file << m_inspectedJson.dump();
    return true;
  } catch (...) { return false; }
}

void JSONInspector::closeFile(const bool promptSave) {
  m_askForSave = promptSave;

  if (m_askForSave) {
    return;
  }

  m_inspectedJson.clear();
  m_path.clear();
}