#pragma once
#include "Core/CommonUI/IDialogController.hpp"
#include "Database/EventCommands/IEventCommand.hpp"
#include <string>

struct IEventDialogController : IDialogController {
  explicit IEventDialogController(const std::string& name)
  : IDialogController(name) {}
  std::tuple<bool, bool> draw() override = 0;
  virtual bool ssNextFunc() const { return m_isNext; }
  virtual void setNext(const bool open) { m_isNext = open; }
  virtual std::shared_ptr<IEventCommand> getCommand() { return nullptr; }
  virtual std::vector<std::shared_ptr<IEventCommand>> getBatchCommands() { return std::vector<std::shared_ptr<IEventCommand>>{}; }
  virtual std::string getUIString() { return ""; }
  virtual void setParentIndent(int indent) { m_parentIndent.emplace(indent); }
  [[nodiscard]] virtual std::optional<int> getParentIndent() const { return m_parentIndent; }
  virtual std::vector<std::shared_ptr<IEventCommand>> getTemplateCommands(EventCode code, int intParam1 = 0) { return std::vector<std::shared_ptr<IEventCommand>>{}; }

protected:
  std::optional<int> m_parentIndent;
  bool m_isNext{false};
};