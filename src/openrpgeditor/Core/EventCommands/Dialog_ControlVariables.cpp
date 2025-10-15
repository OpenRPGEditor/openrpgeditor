
#include "Core/EventCommands/Dialog_ControlVariables.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Log.hpp"
#include "Database/Database.hpp"
#include <imgui.h>
#include <imgui_internal.h>

std::tuple<bool, bool> Dialog_ControlVariables::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ControlVariables");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 48) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 19) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###ControlVariables", m_name).c_str(), &m_open)) {
    drawPickers();
    ImGui::BeginVertical("##control_variables_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox variableGroupBox(trNOOP("Variable"), "##control_variables_variable_groupbox", {-1, 0});
      if (variableGroupBox.begin()) {
        ImGui::BeginHorizontal("##control_variables_variable_group_layout", {-1, -1}, 0);
        {
          ImGui::BeginVertical("##control_variables_variable_group_radios", {-1, 0}, 0);
          {
            ImGui::RadioButton(trNOOP("Single"), &m_isRange, 0); // Specific Switch
            ImGui::RadioButton(trNOOP("Range"), &m_isRange, 1);  // Range of Switches
          }
          ImGui::EndVertical();
          ImGui::BeginVertical("##control_variables_variable_group_variables", {-1, 0}, 0);
          {
            ImGui::BeginDisabled(m_isRange != 0);
            {
              ImGui::PushID("##control_variables_variable_group_variable_button");
              if (ImGui::EllipsesButton(m_isRange != 0 ? "" : Database::instance()->variableNameAndId(m_variableVar).c_str(), {-1, 0})) {
                m_singleRequest = true;
                m_variablePicker.emplace(VariableSwitchPicker::Type::Variable, Database::instance()->system->variables(), m_variableVar);
                m_variablePicker->setOpen(true);
              }
              ImGui::PopID();
            }
            ImGui::EndDisabled();
            ImGui::BeginChild("##control_variables_variable_range", {-1, 0},
                              ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoBackground);
            {
              ImGui::BeginDisabled(m_isRange != 1);
              {
                ImGui::SetNextItemWidth((ImGui::GetContentRegionAvail().x * .5f) - (ImGui::CalcTextSize("~").x / 2));
                if (ImGui::SpinInt("##control_variables_variable_range_min", &m_start, 1, 100, m_isRange == 1 ? nullptr : "")) {
                  m_start = std::clamp(m_start, 1, INT_MAX);
                }
                ImGui::SameLine();
                ImGui::Text("~");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                if (ImGui::SpinInt("##control_variables_variable_range_max", &m_end, 1, 100, m_isRange == 1 ? nullptr : "")) {
                  m_end = std::clamp(m_end, 1, INT_MAX);
                }
              }
              ImGui::EndDisabled();
            }
            ImGui::EndChild();
          }
          ImGui::EndVertical();
        }
        ImGui::EndHorizontal();
      }
      variableGroupBox.end();
      GroupBox operationGroupBox(trNOOP("Operation"), "##control_variables_operation_groupbox", {-1, 0});
      if (operationGroupBox.begin()) {
        ImGui::BeginHorizontal("##control_variables_operation_group_layout", {ImGui::GetContentRegionAvail().x, -1}, 0);
        {
          const auto weight = 1.f / (magic_enum::enum_count<VariableControlOperation>());
          ImGui::Spring(0.5);
          for (int i = 0; const auto& op : magic_enum::enum_values<VariableControlOperation>()) {
            ImGui::RadioButton(UndectorateEnumName(op).c_str(), &m_operation, static_cast<int>(op));
            if (i < magic_enum::enum_count<VariableControlOperation>()) {
              ImGui::Spring(weight);
            }
            ++i;
          }
          ImGui::Spring(0.5);
        }
        ImGui::EndHorizontal();
      }
      operationGroupBox.end();
      GroupBox operandGroupBox(trNOOP("Operand"), "##control_variables_operand_groupbox", {-1, ImGui::GetContentRegionAvail().y - (ImGui::GetFrameHeightWithSpacing() * 1.5f)});
      if (operandGroupBox.begin()) {
        ImGui::BeginVertical("##control_variables_operand_group_layout", ImGui::GetContentRegionAvail(), 0);
        {
          ImGui::BeginHorizontal("##control_variables_operand_group_layout_inner", {-1, -1}, 0);
          {
            ImGui::BeginVertical("control_variables_operand_group_radios", {-1, -1}, 0);
            {
              ImGui::RadioButton(trNOOP("Constant"), &m_operandType, 0);
              ImGui::RadioButton(trNOOP("Variable"), &m_operandType, 1);
              ImGui::RadioButton(trNOOP("Random"), &m_operandType, 2);
              ImGui::RadioButton(trNOOP("Game Data"), &m_operandType, 3);
              ImGui::RadioButton(trNOOP("Script:"), &m_operandType, 4);
            }
            ImGui::EndVertical();
            ImGui::BeginVertical("control_variables_operand_group_values", {-1, -1}, 0);
            {
              // Constant
              ImGui::BeginDisabled(m_operandType != 0);
              {
                ImGui::SetNextItemWidth(-1);
                if (ImGui::SpinInt("##control_variables_operand_constant", &m_constant, 1, 100, m_operandType == 0 ? nullptr : "")) {
                  m_constant = std::clamp(m_constant, -99999999, 99999999);
                }
              }
              ImGui::EndDisabled();
              // Variable
              ImGui::BeginDisabled(m_operandType != 1);
              {
                ImGui::PushID("####control_variables_operand_variable");
                if (ImGui::Button(m_operandType != 1 ? "" : Database::instance()->variableNameAndId(m_variableOperand).c_str(), ImVec2{-1, 0})) {
                  m_singleRequest = false;
                  m_variablePicker.emplace(VariableSwitchPicker::Type::Variable, Database::instance()->system->variables(), m_variableOperand);
                  m_variablePicker->setOpen(true);
                }
                ImGui::PopID();
              }
              ImGui::EndDisabled();
              // Random
              ImGui::BeginChild("##control_variables_operand_rand", {-1, 0},
                                ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoBackground);
              {
                ImGui::BeginDisabled(m_operandType != 2);
                {
                  ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * .5f - (ImGui::CalcTextSize("~").x / 2));
                  if (ImGui::SpinInt("##control_variables_operand_rand_min", &m_randMin, 1, 100, m_operandType == 2 ? nullptr : "")) {
                    m_randMin = std::clamp(m_randMin, -99999999, 99999999);
                  }
                  ImGui::SameLine();
                  ImGui::TextUnformatted("~");
                  ImGui::SameLine();
                  ImGui::SetNextItemWidth(-1);
                  if (ImGui::SpinInt("##control_variables_operand_rand_max", &m_randMax, 1, 100, m_operandType == 2 ? nullptr : "")) {
                    m_randMax = std::clamp(m_randMax, -99999999, 99999999);
                  }
                }
                ImGui::EndDisabled();
              }
              ImGui::EndChild();
              // Game Data
              ImGui::BeginDisabled(m_operandType != 3);
              ImGui::SetNextItemWidth(-1);
              ImGui::PushID("##control_variable_game_data");
              if (ImGui::EllipsesButton(m_operandType != 3 ? "" : getGameDataString().c_str(), ImVec2{-1, 0})) {
                if (!m_gameDataDialog) {
                  m_gameDataDialog.emplace(trNOOP("Game Data"), m_gameDataType, m_gameDataRawSource, m_gameDataValue, m_troopId);
                }
                m_gameDataDialog->setOpen(true);
              }
              ImGui::PopID();
              ImGui::EndDisabled();
            }
            ImGui::EndVertical();
          }
          ImGui::EndHorizontal();
          ImGui::BeginDisabled(m_operandType != 4);
          {
            if (ImGui::BeginChild("##source_text_child", ImGui::GetContentRegionAvail(), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                                  ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove)) {
              m_script.SetImGuiChildIgnored(true);
              m_script.SetHandleKeyboardInputs(m_operandType == 4);
              m_script.SetHandleMouseInputs(m_operandType == 4);
              m_script.Render("##source_text", {-1, -1});
            }
            ImGui::EndChild();
          }
          ImGui::EndDisabled();
        }
        ImGui::EndVertical();
      }
      operandGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##control_variables_buttons_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##control_variables_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          if (m_isRange) {
            m_command->start = m_start;
            m_command->end = m_end;
          } else {
            m_command->start = m_variableVar;
            m_command->end = m_variableVar;
          }
          m_command->operation = static_cast<VariableControlOperation>(m_operation);
          m_command->operand = static_cast<VariableControlOperand>(m_operandType);
          if (m_command->operand == VariableControlOperand::Constant) {
            m_command->constant = m_constant;
          } else if (m_command->operand == VariableControlOperand::Variable) {
            m_command->variable = m_variableOperand;
          } else if (m_command->operand == VariableControlOperand::Random) {
            m_command->random.min = m_randMin;
            m_command->random.max = m_randMax;
          } else if (m_command->operand == VariableControlOperand::Script) {
            m_command->script = m_script.GetText();
          } else if (m_command->operand == VariableControlOperand::Game_Data) {
            m_command->gameData.type = static_cast<GameDataType>(m_gameDataType);
            m_command->gameData.rawSource = m_gameDataRawSource;
            m_command->gameData.value = m_gameDataValue;
          }
          ImGui::CloseCurrentPopup();
          setOpen(false);
        } else if (ret == 1) {
          ImGui::CloseCurrentPopup();
          setOpen(false);
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
    ImGui::EndPopup();
  }
  return std::make_tuple(!m_open, m_confirmed);
}

void Dialog_ControlVariables::drawPickers() {
  if (m_variablePicker) {
    if (const auto [closed, confirmed] = m_variablePicker->draw(); closed) {
      if (confirmed) {
        if (m_singleRequest) {
          m_variableVar = m_variablePicker->selection();
        } else {
          m_variableOperand = m_variablePicker->selection();
        }
      }
      m_variablePicker.reset();
    }
  }

  if (m_gameDataDialog) {
    if (const auto [closed, confirmed] = m_gameDataDialog->draw(); confirmed) {
      m_gameDataType = static_cast<int>(m_gameDataDialog->getData().gameData.type);
      m_gameDataRawSource = m_gameDataDialog->getData().gameData.rawSource;
      m_gameDataValue = m_gameDataDialog->getData().gameData.value;
    }
  }
}

std::string Dialog_ControlVariables::getGameDataString() {
  switch (static_cast<GameDataType>(m_gameDataType)) {
  case GameDataType::Item:
    return Database::theNumberOf(Database::instance()->itemNameOrId(m_gameDataValue));
  case GameDataType::Weapon:
    return Database::theNumberOf(Database::instance()->weaponNameOrId(m_gameDataValue));
  case GameDataType::Armor:
    return Database::theNumberOf(Database::instance()->armorNameOrId(m_gameDataValue));
  case GameDataType::Actor:
    return Database::AOfB(DecodeEnumName(static_cast<ActorDataSource>(m_gameDataRawSource)), Database::instance()->actorNameOrId(m_gameDataValue));
  case GameDataType::Enemy: {
    std::string enemyName;
    if (m_troopId == 0) {
      enemyName = std::format("#{}", m_gameDataValue + 1);
    } else {
      enemyName = Database::instance()->troopMemberName(m_troopId, m_gameDataValue);
    }
    return Database::AOfB(DecodeEnumName(static_cast<EnemyDataSource>(m_gameDataRawSource)), enemyName);
  }
  case GameDataType::Character:
    return Database::AOfB(DecodeEnumName(static_cast<CharacterDataSource>(m_gameDataRawSource)), Database::instance()->eventNameOrId(m_gameDataValue));
  case GameDataType::Party:
    return trFormat("Actor ID of the party member #{0}", m_gameDataValue + 1);
  case GameDataType::Other:
    return DecodeEnumName(static_cast<OtherDataSource>(m_gameDataValue));
  }

  return "???";
}
