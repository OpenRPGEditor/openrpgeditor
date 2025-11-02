#pragma once
#include "Database/CommandParser.hpp"
#include "Database/Database.hpp"
#include "Editor/CommonUI/TextEditor.hpp"
#include "Editor/CommonUI/VariableSwitchPicker.hpp"
#include "Editor/EventCommands/Dialog_GameData.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"
#include "Editor/Log.hpp"

struct Dialog_ControlVariables : IEventDialogController {
  Dialog_ControlVariables() = delete;
  explicit Dialog_ControlVariables(const std::string& name, const std::shared_ptr<ControlVariables>& cmd = nullptr, const int troopId = 0)
  : IEventDialogController(name)
  , m_troopId(troopId)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command = std::make_shared<ControlVariables>();
    }
    m_operation = static_cast<int>(m_command->operation);
    m_operandType = static_cast<int>(m_command->operand);

    if (m_command->start == m_command->end) {
      m_isRange = 0;
      m_start = m_command->start;
      m_end = m_command->end;
      m_variableVar = m_start;
    } else {
      m_isRange = 1;
      m_start = m_command->start;
      m_end = m_command->end;
    }
    if (m_command->operand == VariableControlOperand::Constant) {
      m_constant = m_command->constant;
    } else if (m_command->operand == VariableControlOperand::Variable) {
      m_variableOperand = m_command->variable;
    } else if (m_command->operand == VariableControlOperand::Random) {
      m_randMin = m_command->random.min;
      m_randMax = m_command->random.max;
    } else if (m_command->operand == VariableControlOperand::Script) {
      m_script.SetTextLines(splitString(m_command->script, '\n'));
    } else if (m_command->operand == VariableControlOperand::Game_Data) {
      m_gameDataType = static_cast<int>(m_command->gameData.type);
      m_gameDataRawSource = m_command->gameData.rawSource;
      m_gameDataValue = m_command->gameData.value;
    }
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; }

private:
  void drawPickers();
  // Command scope
  int m_troopId{0};
  int m_start{1};
  int m_end{1};
  int m_constant{0};
  int m_variableOperand{1};
  int m_operation{};
  int m_operandType{0};
  int m_gameDataType{7};
  int m_gameDataRawSource{0};
  int m_gameDataValue{0};
  TextEditor m_script;

  // Outside of command scope
  int m_randMin{0};
  int m_randMax{0};
  int m_variableVar{1};
  int m_isRange{0}; // Radio button (Single / Range)

  // Other
  std::shared_ptr<ControlVariables> m_command;
  std::optional<VariableSwitchPicker> m_variablePicker;
  bool m_singleRequest = false;
  bool m_confirmed{false};
  std::optional<Dialog_GameData> m_gameDataDialog;

  std::string getGameDataString();
};