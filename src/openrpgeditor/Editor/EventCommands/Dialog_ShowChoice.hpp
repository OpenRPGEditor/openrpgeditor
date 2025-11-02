#pragma once
#include "Database/EventCommands/EventDummy.hpp"
#include "Database/EventCommands/ShowChoice.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"
#include "Editor/Log.hpp"

struct Dialog_ShowChoice : IEventDialogController {
  Dialog_ShowChoice() = delete;
  explicit Dialog_ShowChoice(const std::string& name, const std::shared_ptr<ShowChoiceCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ShowChoiceCommand());
    }

    for (int index{0}; const auto& string : m_command->choices) {
      m_choices[index] = string;
      index++;
    }

    m_background = static_cast<int>(m_command->background);
    m_position = static_cast<int>(m_command->positionType);
    m_cancelType = m_command->cancelType;
    m_defaultType = m_command->defaultType;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

  std::vector<std::shared_ptr<IEventCommand>> getTemplateCommands(const EventCode code, const int intParam1 = 0) override {
    std::vector<std::shared_ptr<IEventCommand>> commandList;
    if (code == EventCode::When_Selected) {
      commandList.push_back(std::make_shared<WhenSelectedCommand>());
      commandList.back()->setIndent(getParentIndent().value());
      const auto when = static_pointer_cast<WhenSelectedCommand>(commandList.back());
      when->choice = m_command->choices.at(intParam1);
      when->param1 = intParam1;
      commandList.push_back(std::make_shared<EventDummy>());
      commandList.back()->setIndent(getParentIndent().value() + 1);
    }
    return commandList;
  }

  std::vector<std::shared_ptr<IEventCommand>> getBatchCommands() override {

    const std::shared_ptr<IEventCommand> sharedCommand = getCommand();
    m_eventCommands.push_back(sharedCommand);
    m_eventCommands.back()->setIndent(getParentIndent().value());

    for (int i{0}; i < m_command->choices.size(); i++) {
      // When "Choice"
      m_eventCommands.push_back(std::make_shared<WhenSelectedCommand>());
      m_eventCommands.back()->setIndent(getParentIndent().value());
      const auto when = static_pointer_cast<WhenSelectedCommand>(m_eventCommands.back());
      when->choice = m_command->choices.at(i);
      when->param1 = i;
      m_eventCommands.push_back(std::make_shared<EventDummy>());
      // When "Cancel"
      m_eventCommands.back()->setIndent(getParentIndent().value() + 1);
      if (m_command->cancelType == -2 && i == m_command->choices.size() - 1) {
        m_eventCommands.push_back(std::make_shared<WhenCancelCommand>());
        m_eventCommands.back()->setIndent(getParentIndent().value());
        m_eventCommands.push_back(std::make_shared<EventDummy>());
        m_eventCommands.back()->setIndent(getParentIndent().value() + 1);
      }
      // Show Choices End
      if (i == m_command->choices.size() - 1) {
        m_eventCommands.push_back(std::make_shared<ShowChoicesEndCommand>());
        m_eventCommands.back()->setIndent(getParentIndent().value());
      }
    }
    return m_eventCommands;
  };

private:
  std::array<std::string, 6> m_choices;
  int m_background;
  int m_position;
  int m_defaultType;
  int m_cancelType;

  std::vector<std::shared_ptr<IEventCommand>> m_eventCommands;

  std::shared_ptr<ShowChoiceCommand> m_command;
};