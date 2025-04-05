#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Log.hpp"
#include "Database/EventCommands/EventDummy.hpp"
#include "Database/EventCommands/ShowChoice.hpp"

struct Dialog_ShowChoice : IEventDialogController {
  Dialog_ShowChoice() = delete;
  explicit Dialog_ShowChoice(const std::string& name, const std::shared_ptr<ShowChoiceCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ShowChoiceCommand());
    }

    int index{0};
    for (auto& str : command->choices) {
      m_choices[index] = str;
      index++;
    }
    m_choice_1 = m_choices.at(0);
    m_choice_2 = m_choices.at(1);
    m_choice_3 = m_choices.at(2);
    m_choice_4 = m_choices.at(3);
    m_choice_5 = m_choices.at(4);
    m_choice_6 = m_choices.at(5);

    m_background = static_cast<int>(command->background);
    m_position = static_cast<int>(command->positionType);
    m_cancelType = command->cancelType;
    m_defaultType = command->defaultType;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

  std::vector<std::shared_ptr<IEventCommand>> getTemplateCommands(EventCode code, int intParam1 = 0) override {
    std::vector<std::shared_ptr<IEventCommand>> commandList;
    if (code == EventCode::When_Selected) {
      commandList.push_back(std::make_shared<WhenSelectedCommand>());
      commandList.back()->setIndent(getParentIndent().value());
      const auto when = static_pointer_cast<WhenSelectedCommand>(commandList.back());
      when->choice = command->choices.at(intParam1);
      when->param1 = intParam1;
      commandList.push_back(std::make_shared<EventDummy>());
      commandList.back()->setIndent(getParentIndent().value() + 1);
    }
    return commandList;
  }

  std::vector<std::shared_ptr<IEventCommand>> getBatchCommands() override {

    std::shared_ptr<WhenSelectedCommand> when;

    std::shared_ptr<IEventCommand> sharedCommand = getCommand();
    eventCommands.push_back(sharedCommand);
    eventCommands.back()->setIndent(getParentIndent().value());

    for (int i{0}; i < command->choices.size(); i++) {
      // When "Choice"
      eventCommands.push_back(std::make_shared<WhenSelectedCommand>());
      eventCommands.back()->setIndent(getParentIndent().value());
      when = static_pointer_cast<WhenSelectedCommand>(eventCommands.back());
      when->choice = command->choices.at(i);
      when->param1 = i;
      eventCommands.push_back(std::make_shared<EventDummy>());
      // When "Cancel"
      eventCommands.back()->setIndent(getParentIndent().value() + 1);
      if (command->cancelType == -2 && i == command->choices.size() - 1) {
        eventCommands.push_back(std::make_shared<WhenCancelCommand>());
        eventCommands.back()->setIndent(getParentIndent().value());
        eventCommands.push_back(std::make_shared<EventDummy>());
        eventCommands.back()->setIndent(getParentIndent().value() + 1);
      }
      // Show Choices End
      if (i == command->choices.size() - 1) {
        eventCommands.push_back(std::make_shared<ShowChoicesEndCommand>());
        eventCommands.back()->setIndent(getParentIndent().value());
      }
    }
    return eventCommands;
  };

private:
  std::vector<std::string> m_choices{6};
  int m_background;
  int m_position;
  int m_defaultType;
  int m_cancelType;

  std::string m_choice_1;
  std::string m_choice_2;
  std::string m_choice_3;
  std::string m_choice_4;
  std::string m_choice_5;
  std::string m_choice_6;

  bool m_batchEntry{false};
  std::vector<std::shared_ptr<IEventCommand>> eventCommands;

  bool m_confirmed{false};
  std::shared_ptr<ShowChoiceCommand> command;
  std::tuple<bool, bool> result;
};