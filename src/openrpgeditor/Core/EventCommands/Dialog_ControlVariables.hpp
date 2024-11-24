#pragma once
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/Dialog_GameData.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Log.hpp"
#include "Database/CommandParser.hpp"
#include "Database/Database.hpp"

struct Dialog_ControlVariables : IEventDialogController {
  Dialog_ControlVariables() = delete;
  explicit Dialog_ControlVariables(const std::string& name, const std::shared_ptr<ControlVariables>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ControlVariables());
    }
    m_operation = static_cast<int>(command->operation);
    m_operand = static_cast<int>(command->operand);

    if (command->start == command->end) {
      m_operation_var = 0;
      m_start = command->start;
      m_end = command->end;
      m_variable_var = m_start;
    } else {
      m_operation_var = 1;
      m_rand_1 = command->start;
      m_rand_2 = command->end;
    }
    if (command->operand == VariableControlOperand::Constant) {
      m_constant = command->constant;
    } else if (command->operand == VariableControlOperand::Variable) {
      m_variable = command->variable;
    } else if (command->operand == VariableControlOperand::Random) {
      m_rand_operand_1 = command->random.min;
      m_rand_operand_2 = command->random.max;
    } else if (command->operand == VariableControlOperand::Script) {
      m_script = command->script;
    } else if (command->operand == VariableControlOperand::Game_Data) {
      m_gameData_type = static_cast<int>(command->gameData.type);
      m_gameData_rawSource = command->gameData.rawSource;
      m_gameData_value = command->gameData.value;
    }
    m_rand_operand_1 = command->random.min;
    m_rand_operand_2 = command->random.max;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; }

  std::string getUIString() override {
    if (m_gameData_type == 0) { // GameDataType::Item
      return std::string("The number of " + formatString(Database::instance()->itemNameOrId(m_gameData_rawSource), m_gameData_rawSource));
    }
    if (m_gameData_type == 1) { // GameDataType::Weapon
      return std::string("The number of " + formatString(Database::instance()->weaponNameOrId(m_gameData_rawSource), m_gameData_rawSource));
    }
    if (m_gameData_type == 2) { // GameDataType::Armor
      return std::string("The number of " + formatString(Database::instance()->armorNameOrId(m_gameData_rawSource), m_gameData_rawSource));
    }
    if (m_gameData_type == 3) { // GameDataType::Actor
      return std::string(magic_enum::enum_name(static_cast<ActorDataSource>(m_gameData_value)).data()) + " of " +
             formatString(Database::instance()->actorNameOrId(m_gameData_rawSource), m_gameData_rawSource);
    }
    if (m_gameData_type == 4) { // GameDataType::Enemy
      return std::string(magic_enum::enum_name(static_cast<EnemyDataSource>(m_gameData_value)).data()) + " of #" + std::to_string(m_gameData_rawSource) + " " +
             Database::instance()->troopMemberName(0, m_gameData_rawSource);
    }
    if (m_gameData_type == 5) { // GameDataType::Character
      std::string str = m_gameData_rawSource == -1 ? "Player" : m_gameData_rawSource == 0 ? "This Event" : Database::instance()->eventNameOrId(m_gameData_rawSource);
      return std::string(magic_enum::enum_name(static_cast<CharacterDataSource>(m_gameData_value)).data()) + " of " + formatString(str, m_gameData_rawSource);
    }
    if (m_gameData_type == 6) { // GameDataType::Party
      return "Actor ID of the party member #" + std::to_string(m_gameData_rawSource);
    }
    if (m_gameData_type == 7) { // GameDataType::Other
      return std::string(magic_enum::enum_name(static_cast<OtherDataSource>(m_gameData_rawSource)).data());
    }
    return "";
  }

private:
  // Command scope
  int m_start;
  int m_end;
  int m_constant{0};
  int m_variable{1};
  int m_operation;
  int m_operand;
  int m_gameData_type{7};
  int m_gameData_rawSource{0};
  int m_gameData_value{0};
  std::string m_script;

  // Outside of command scope
  int m_rand_1{1};
  int m_rand_2{1};
  int m_rand_operand_1{0};
  int m_rand_operand_2{0};
  int m_variable_var{1};
  int m_operation_var; // Radio button (Single / Range)

  // Other
  std::shared_ptr<ControlVariables> command;
  std::optional<VariableSwitchPicker> picker;
  std::tuple<bool, bool> result;
  bool singleRequest = false;
  bool m_confirmed{false};
  std::optional<Dialog_GameData> gameDataDialog;

  static std::string formatString(std::string str, int source) {
    if (str == "") {
      return std::format("#{:04} ", source);
    } else {
      return str;
    }
  }
};