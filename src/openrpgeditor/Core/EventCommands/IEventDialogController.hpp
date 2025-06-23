#pragma once
#include "Core/CommonUI/IDialogController.hpp"
#include "Database/EventCommands/IEventCommand.hpp"
#include <string>

struct IEventDialogController : IDialogController {
  explicit IEventDialogController(const std::string& name)
  : IDialogController(name) {}
  std::tuple<bool, bool> draw() override = 0;
  virtual std::shared_ptr<IEventCommand> getCommand() { return nullptr; }
  virtual std::vector<std::shared_ptr<IEventCommand>> getBatchCommands() { return std::vector<std::shared_ptr<IEventCommand>>{}; }
  virtual std::vector<std::shared_ptr<IEventCommand>> getBatchCommands(std::vector<std::shared_ptr<IEventCommand>>& list) { return std::vector<std::shared_ptr<IEventCommand>>{}; }
  virtual void setParentIndent(int indent) { m_parentIndent.emplace(indent); }
  [[nodiscard]] virtual std::optional<int> getParentIndent() const { return m_parentIndent; }
  virtual std::vector<std::shared_ptr<IEventCommand>> getTemplateCommands(EventCode code, int intParam1 = 0) { return std::vector<std::shared_ptr<IEventCommand>>{}; }
  virtual bool isCurrentElseBranch() { return false; }
  virtual void setCurrentElseBranch() {};

protected:
  std::optional<int> m_parentIndent;
};