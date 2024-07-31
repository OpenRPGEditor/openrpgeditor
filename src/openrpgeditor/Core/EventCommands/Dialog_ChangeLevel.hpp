#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Skills.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeHP.hpp"
#include "Database/EventCommands/ChangeLevel.hpp"

struct Dialog_ChangeLevel : IEventDialogController {
  Dialog_ChangeLevel() = delete;
  explicit Dialog_ChangeLevel(const std::string& name,
                              const std::shared_ptr<ChangeLevelCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeLevelCommand());
    }

    m_comparison = static_cast<int>(command->comparison);
    m_quantityOp = static_cast<int>(command->quantityOp);
    m_quantitySource = static_cast<int>(command->quantitySource);
    m_showLevelUp = command->showLevelUp;

    if (command->comparison == ActorComparisonSource::Variable)
      m_value_var = command->value;
    else
      m_value = command->value;

    if (command->quantitySource == QuantityChangeSource::Variable)
      m_quantity_var = command->quantity;
    else
      m_quantity = command->quantity;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:


  int m_comparison;
  int m_value;
  int m_value_var{1};
  int m_quantityOp;
  int m_quantitySource;
  int m_quantity;
  int m_quantity_var{1};
  bool m_showLevelUp{false};
  bool isOperand{false};

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::optional<VariableSwitchPicker> picker;
  std::shared_ptr<ChangeLevelCommand> command;
  std::tuple<bool, bool> result;
};