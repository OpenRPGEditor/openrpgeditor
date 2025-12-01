#include "Editor/JSONInspector.hpp"

#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "orei18n.hpp"

#include <fstream>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include <athena/Utility.hpp>

void JSONInspector::DrawJsonPrimitive(const std::string& name, nlohmann::ordered_json& element) {
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
  case nlohmann::ordered_json::value_t::number_unsigned: {
    std::string s = element.dump();
    ImGui::SetNextItemWidth(-1);
    if (ImGui::InputText("##value", &s, ImGuiInputTextFlags_EnterReturnsTrue)) {
      try {
        if (element.is_number_integer() || element.is_number_unsigned()) {
          element = std::stoll(s.data());
        }
      } catch (...) {}
    }
    break;
  }
  case nlohmann::ordered_json::value_t::number_float: {
    auto val = element.get<double>();
    if (ImGui::SpinDouble("##value", &val)) {
      element = val;
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

bool changeKey(nlohmann::ordered_json& container, const std::string& oldKey, const std::string& newKey) {
  if (oldKey == newKey) {
    return false;
  }
  if (!container.is_object() || container.find(oldKey) == container.end() || container.count(newKey)) {
    return false;
  }

  nlohmann::ordered_json newContainer = nlohmann::ordered_json::object();

  for (auto it = container.begin(); it != container.end(); ++it) {
    if (it.key() == oldKey) {
      newContainer[newKey] = std::move(it.value());
    } else {
      newContainer[it.key()] = std::move(it.value());
    }
  }
  container = std::move(newContainer);
  return true;
}

static constexpr std::array kJSONTypeNames{
    "Null", "Object", "Array", "String", "Boolean", "Signed", "Unsigned", "Float", "Binary",
};
static constexpr std::array kJSONTypes{
    nlohmann::ordered_json::value_t::null,    nlohmann::ordered_json::value_t::object,         nlohmann::ordered_json::value_t::array,           nlohmann::ordered_json::value_t::string,
    nlohmann::ordered_json::value_t::boolean, nlohmann::ordered_json::value_t::number_integer, nlohmann::ordered_json::value_t::number_unsigned, nlohmann::ordered_json::value_t::number_float,
    nlohmann::ordered_json::value_t::binary,
};

// Draws a JSON object or array in a table format
void JSONInspector::DrawJsonContainer(const std::string& name, nlohmann::ordered_json& container) {
  ImGui::PushID(name.c_str());

  if (ImGui::TreeNodeEx("##tree_node", ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth, "%s (%s, %zu items)", name.c_str(),
                        container.is_object() ? "object" : "array", container.size())) {
    if (ImGui::BeginTable("##json_table", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg)) {
      ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Value");
      ImGui::TableSetupColumn("Type");
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

      for (auto& [key, value] : items) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        std::string tmp = key;
        ImGui::PushID(value);
        ImGui::SetNextItemWidth(-1);
        ImGui::InputText("##key", &tmp);
        if (ImGui::IsItemDeactivatedAfterEdit() && changeKey(container, key, tmp)) {
          // Break out of the loop early if the key was successfully changed
          ImGui::PopID();
          break;
        }
        ImGui::PopID();

        ImGui::TableNextColumn();
        DrawJsonElement(key, *value);

        ImGui::TableNextColumn();

        int type = static_cast<int>(value->type());
        ImGui::PushID(key.c_str());
        ImGui::SetNextItemWidth(-1);
        if (ImGui::Combo("", &type, kJSONTypeNames.data(), kJSONTypeNames.size())) {
          if (const auto t = kJSONTypes[type]; value->type() != t) {
            switch (t) {
            case nlohmann::ordered_json::value_t::null:
              *value = nullptr;
              break;
            case nlohmann::ordered_json::value_t::object:
              if (value->type() == nlohmann::ordered_json::value_t::string) {
                // first attempt to parse the string just in case it's valid json
                try {
                  if (const auto tmpContainer = nlohmann::ordered_json::parse(value->get<std::string>()); tmpContainer.is_object()) {
                    *value = tmpContainer;
                  } else {
                    *value = nlohmann::ordered_json::object();
                  }
                } catch (...) { *value = nlohmann::ordered_json::object(); }
              } else {
                *value = nlohmann::ordered_json::object();
              }
              break;
            case nlohmann::ordered_json::value_t::array:
              if (value->type() == nlohmann::ordered_json::value_t::string) {
                // first attempt to parse the string just in case it's valid json
                try {
                  if (const auto tmpArray = nlohmann::ordered_json::parse(value->get<std::string>()); tmpArray.is_array()) {
                    *value = tmpArray;
                  } else {
                    *value = nlohmann::ordered_json::array();
                  }
                } catch (...) { *value = nlohmann::ordered_json::array(); }
              } else {
                *value = nlohmann::ordered_json::array();
              }
              break;
            case nlohmann::ordered_json::value_t::string: {
              switch (value->type()) {
              case nlohmann::ordered_json::value_t::boolean:
                *value = value->get<bool>() ? "true" : "false";
                break;
              case nlohmann::ordered_json::value_t::number_integer:
                *value = std::to_string(value->get<int64_t>());
                break;
              case nlohmann::ordered_json::value_t::number_unsigned:
                *value = std::to_string(value->get<uint64_t>());
                break;
              case nlohmann::ordered_json::value_t::number_float:
                *value = std::to_string(value->get<double>());
                break;
              default:
                *value = value->dump();
                break;
              }
              break;
            }
            case nlohmann::ordered_json::value_t::boolean:
              if (value->type() == nlohmann::ordered_json::value_t::string) {
                *value = athena::utility::parseBool(value->get<std::string>());
              } else {
                *value = nlohmann::ordered_json::boolean_t();
              }
              break;
            case nlohmann::ordered_json::value_t::number_integer:
              if (value->type() == nlohmann::ordered_json::value_t::number_float || value->type() == nlohmann::ordered_json::value_t::number_unsigned) {
                // JSON allows implicit conversion for Number types
                *value = value->get<int64_t>();
              } else {
                *value = nlohmann::ordered_json::number_integer_t();
              }
              break;
            case nlohmann::ordered_json::value_t::number_unsigned:
              if (value->type() == nlohmann::ordered_json::value_t::number_float || value->type() == nlohmann::ordered_json::value_t::number_integer) {
                // JSON allows implicit conversion for Number types
                *value = value->get<uint64_t>();
              } else {
                *value = nlohmann::ordered_json::number_unsigned_t();
              }
              break;
            case nlohmann::ordered_json::value_t::number_float:
              if (value->type() == nlohmann::ordered_json::value_t::number_unsigned || value->type() == nlohmann::ordered_json::value_t::number_integer) {
                // JSON allows implicit conversion for Number types
                *value = value->get<double>();
              } else {
                *value = nlohmann::ordered_json::number_float_t();
              }
              break;
            case nlohmann::ordered_json::value_t::binary:
              *value = nlohmann::ordered_json::binary_t();
              break;
            default:
              break;
            }
          }
        }
        ImGui::PopID();
      }
      // Add new entry
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::TableNextColumn();
      if (ImGui::Button(trNOOP("Add"), {-1, 0})) {
        m_targetContainer = &container;
      }
      ImGui::EndTable();
    }
    ImGui::TreePop();
  }
  ImGui::PopID();
}

void JSONInspector::DrawJsonElement(const std::string& name, nlohmann::ordered_json& element) {
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

  if (m_targetContainer) {
    ImGui::OpenPopup("###JSONInspector_AddNewPopup");
  }

  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Once, {0.5f, 0.5f});
  if (ImGui::BeginPopupModal(std::format("{}###JSONInspector_AddNewPopup", trNOOP("Add New Value")).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    m_newKeyInvalid = m_newKey.empty() || m_targetContainer->contains(m_newKey);
    ImGui::BeginVertical("##add_new_main_layout", {-1, 0});
    {
      GroupBox keyGroup(trNOOP("Key"), "##add_new_key", {-1, 0});
      if (keyGroup.begin()) {
        ImGui::SetNextItemWidth(-1);
        const auto borderCol = ImGui::GetStyleColorVec4(ImGuiCol_Border);
        ImGui::PushStyleColor(ImGuiCol_Border, !m_newKeyInvalid ?  borderCol : ImVec4(1, 0, 0, borderCol.w));
        ImGui::InputText("##key", &m_newKey);
        ImGui::PopStyleColor();
        // Double check just in case the text got changed
        m_newKeyInvalid = m_newKey.empty() || m_targetContainer->contains(m_newKey);
        if (m_newKeyInvalid) {
          ImGui::SetItemTooltip("%s", m_newKey.empty() ? trNOOP("Key is empty!") : trNOOP("Key already exists!"));
        }
      }
      keyGroup.end();
      GroupBox typeGroup(trNOOP("Type"), "##add_new_type", {-1, 0});
      if (typeGroup.begin()) {
        ImGui::SetNextItemWidth(-1);
        int t = static_cast<int>(m_newType);
        if (ImGui::Combo("##type", &t, kJSONTypeNames.data(), kJSONTypeNames.size())) {
          m_newType = static_cast<nlohmann::ordered_json::value_t>(t);
        }
      }
      typeGroup.end();
      ImGui::Spring();
      ImGui::Separator();
      ImGui::BeginHorizontal("##add_new_button_layout", {-1, 0});
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##add_new_buttons", {trNOOP("OK"), trNOOP("Cancel")}, false, {}, {m_newKeyInvalid}); ret == 0) {
          if (!m_newKey.empty() && !m_targetContainer->contains(m_newKey)) {
            switch (m_newType) {
            case nlohmann::ordered_json::value_t::null:
              (*m_targetContainer)[m_newKey] = nullptr;
              break;
            case nlohmann::ordered_json::value_t::object:
              (*m_targetContainer)[m_newKey] = nlohmann::ordered_json::object();
              break;
            case nlohmann::ordered_json::value_t::array:
              (*m_targetContainer)[m_newKey] = nlohmann::ordered_json::object();
              break;
            case nlohmann::ordered_json::value_t::string:
              (*m_targetContainer)[m_newKey] = nlohmann::ordered_json::string_t();
              break;
            case nlohmann::ordered_json::value_t::boolean:
              (*m_targetContainer)[m_newKey] = nlohmann::ordered_json::boolean_t();
              break;
            case nlohmann::ordered_json::value_t::number_integer:
              (*m_targetContainer)[m_newKey] = nlohmann::ordered_json::number_integer_t();
              break;
            case nlohmann::ordered_json::value_t::number_unsigned:
              (*m_targetContainer)[m_newKey] = nlohmann::ordered_json::number_unsigned_t();
              break;
            case nlohmann::ordered_json::value_t::number_float:
              (*m_targetContainer)[m_newKey] = nlohmann::ordered_json::number_float_t();
              break;
            case nlohmann::ordered_json::value_t::binary:
              (*m_targetContainer)[m_newKey] = nlohmann::ordered_json::binary_t();
              break;
            default:
              break;
            }
          }
          m_targetContainer = nullptr;
          ImGui::CloseCurrentPopup();
        } else if (ret == 1) {
          m_targetContainer = nullptr;
          ImGui::CloseCurrentPopup();
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
    ImGui::EndPopup();
  }

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