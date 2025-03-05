#include "Core/Interpreter.hpp"

#include "Database/EventCommands/ConditionalBranch.hpp"
#include "Database/EventCommands/ControlSwitches.hpp"
#include "Database/EventCommands/Script.hpp"
#include "Database/EventCommands/ShowPicture.hpp"
#include "Database/EventCommands/Wait.hpp"

#include <regex>

Interpreter::Interpreter(std::vector<std::shared_ptr<IEventCommand>> commands) { m_commands = commands; }

bool Interpreter::update() {
  if (m_wait > 0) {
    m_wait--;
  } else {
    if (m_index < m_commands.size() - 1) {
      if (m_commands.at(m_index)->code() == EventCode::Conditional_Branch) {
        std::shared_ptr<ConditionalBranchCommand> cmd = std::dynamic_pointer_cast<ConditionalBranchCommand>(m_commands.at(m_index));
        if (cmd->type == ConditionType::Switch && m_switchMap.contains(cmd->globalSwitch.switchIdx)) {
          if (m_switchMap[cmd->globalSwitch.switchIdx] == false && cmd->globalSwitch.checkIfOn == ValueControl::ON) {
            m_isSkip = true;
          }
          if (m_switchMap[cmd->globalSwitch.switchIdx] == true && cmd->globalSwitch.checkIfOn == ValueControl::OFF) {
            m_isSkip = true;
          }
        }
        if (cmd->type == ConditionType::Variable && m_variableMap.contains(cmd->variable.id) && cmd->variable.source == VariableComparisonSource::Constant) {
          // This condition is satisfied for CONSTANT commands only. Variable commands are not supported.
          if (m_variableMap[cmd->variable.id] != cmd->variable.constant) {
            m_isSkip = true;
          }
        }
      }
      if (!m_isSkip) {
        if (m_commands.at(m_index)->code() == EventCode::Show_Picture) {
          std::shared_ptr<ShowPictureCommand> cmd = std::dynamic_pointer_cast<ShowPictureCommand>(m_commands.at(m_index));
          m_image.emplace(cmd->imageName, 2, false);
          m_keyFrame++;
        }
        if (m_commands.at(m_index)->code() == EventCode::Script) {
          std::shared_ptr<ScriptCommand> cmd = std::dynamic_pointer_cast<ScriptCommand>(m_commands.at(m_index));
          m_image.emplace(findStringMatch(cmd->script), 2, false);
          m_keyFrame++;
        }
        if (m_commands.at(m_index)->code() == EventCode::Wait) {
          std::shared_ptr<WaitCommand> wait = std::dynamic_pointer_cast<WaitCommand>(m_commands.at(m_index));
          m_wait = wait->duration;
          if (m_isFastForward) {
            m_wait = m_wait / 2;
          }
        }
      } else {
        // Skip all commands until we reach an else or an end. This signals that the branch condition has been processed.
        if (m_commands.at(m_index)->code() == EventCode::End) {
          m_isSkip = false;
        }
        if (m_commands.at(m_index)->code() == EventCode::Else) {
          m_isSkip = false; // If => Else, this should all run
        }
      }

      m_index++;
    } else {
      m_index = 0;
      m_wait = 0;
      m_keyFrame = 0;
    }
  }
  return true;
}
std::vector<int> Interpreter::getKeyFrames() const {
  std::vector<int> keys;
  for (int i = 0; i < m_commands.size(); i++) {
    if (m_commands.at(i)->code() == EventCode::Show_Picture) {
      keys.push_back(i);
    }
    if (m_commands.at(i)->code() == EventCode::Script) {
      std::shared_ptr<ScriptCommand> cmd = std::dynamic_pointer_cast<ScriptCommand>(m_commands.at(i));
      if (isStringMatch(cmd->script)) {
        keys.push_back(i);
      }
    }
  }
  return keys;
}
std::vector<int> Interpreter::getSwitches() {
  std::vector<int> switches;
  for (int i = 0; i < m_commands.size(); i++) {
    if (m_commands.at(i)->code() == EventCode::Conditional_Branch) {
      std::shared_ptr<ConditionalBranchCommand> cmd = std::dynamic_pointer_cast<ConditionalBranchCommand>(m_commands.at(i));
      if (cmd->type == ConditionType::Switch) {
        if (!m_switchMap.contains(cmd->globalSwitch.switchIdx)) {
          switches.push_back(cmd->globalSwitch.switchIdx);
          m_switchMap[cmd->globalSwitch.switchIdx] = false;
        }
      }
    }
  }
  return switches;
}
std::vector<int> Interpreter::getVariables() {
  std::vector<int> variables;
  for (int i = 0; i < m_commands.size(); i++) {
    if (m_commands.at(i)->code() == EventCode::Conditional_Branch) {
      std::shared_ptr<ConditionalBranchCommand> cmd = std::dynamic_pointer_cast<ConditionalBranchCommand>(m_commands.at(i));
      if (cmd->type == ConditionType::Variable && cmd->variable.source == VariableComparisonSource::Constant) {
        if (!m_variableMap.contains(cmd->variable.id)) {
          m_variableMap[cmd->variable.id] = 0;
          variables.push_back(cmd->variable.id);
        }
      } else {
        // Variable comparison... we need another variable check?
        // Unlikely that we'll ever need to compare this, but the section exists just in case
      }
    }
  }
  return variables;
}
void Interpreter::setVariable(int index, int value) { m_variableMap[index] = value; }
void Interpreter::setSwitch(int index, bool value) { m_switchMap[index] = value; }
void Interpreter::setIndex(int index, int keyFrame) {
  // Sets the interpreter to a keyframe index and emplaces the image.
  // Keyframe indexes are built in Preview from a compiled vector based on image results in the command list
  m_index = index;
  m_keyFrame = keyFrame;
  if (m_commands.at(m_index)->code() == EventCode::Show_Picture) {
    std::shared_ptr<ShowPictureCommand> cmd = std::dynamic_pointer_cast<ShowPictureCommand>(m_commands.at(m_index));
    m_image.emplace(cmd->imageName, 2, false);
  }
  if (m_commands.at(m_index)->code() == EventCode::Script) {
    std::shared_ptr<ScriptCommand> cmd = std::dynamic_pointer_cast<ScriptCommand>(m_commands.at(m_index));
    m_image.emplace(findStringMatch(cmd->script), 2, false);
  }
}
std::string Interpreter::findStringMatch(std::string text) {
  std::regex re("\"([^\"]*)\""); // Regex to match anything inside quotes
  std::smatch match;
  if (std::regex_search(text, match, re) && match.size() > 1) {
    return match.str(1);
  }
  return ""; // String not found?
}
bool Interpreter::isStringMatch(std::string text) const {
  std::regex re("\"([^\"]*)\""); // Regex to match anything inside quotes
  std::smatch match;
  if (std::regex_search(text, match, re) && match.size() > 1) {
    return true;
  }
  return false; // String not found?
}
