#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeParameter.hpp"
struct Project;
struct Dialog_ChangeParameter : IEventDialogController {
  Dialog_ChangeParameter() = delete;
  explicit Dialog_ChangeParameter(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new ChangeParameterCommand());

    m_comparison = static_cast<int>(command->comparison);
    m_quantityOp = static_cast<int>(command->quantityOp);
    m_quantitySource = static_cast<int>(command->quantitySource);
    m_parameterSource = static_cast<int>(command->param);

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
  Project* m_project = nullptr;

private:


  int m_comparison;
  int m_value{1};
  int m_value_var{1};
  int m_quantityOp;
  int m_quantitySource;
  int m_quantity{1};
  int m_quantity_var{1};
  bool isOperand{false};
  int m_parameterSource;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::optional<VariableSwitchPicker> picker;
  std::shared_ptr<ChangeParameterCommand> command;
  std::tuple<bool, bool> result;
};