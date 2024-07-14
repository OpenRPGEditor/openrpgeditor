#pragma once
#include "Core/CommonUI/IDialogController.hpp"
#include "Database/EventCommands/IEventCommand.hpp"
#include <string>

struct IEventDialogController : IDialogController {
  explicit IEventDialogController(const std::string& name) : IDialogController(name) {}
  std::tuple<bool, bool> draw() override = 0;
  virtual bool IsNextFunc() const { return m_isNext; }
  virtual void SetNext(const bool open) { m_isNext = open; }
  virtual std::shared_ptr<IEventCommand> getCommand() { return nullptr; }
  virtual std::vector<std::shared_ptr<IEventCommand>> getBatchCommands() { return std::vector<std::shared_ptr<IEventCommand>>{}; }

protected:
  bool m_isNext{false};
};