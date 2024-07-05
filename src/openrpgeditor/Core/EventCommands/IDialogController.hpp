#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <string>

struct IDialogController {
  explicit IDialogController(const std::string& name) : m_open(false), m_name(name) {}
  virtual ~IDialogController() = default;
  virtual std::tuple<bool, bool> draw() = 0;
  bool IsOpen() const { return m_open; }
  bool IsConfirmed() const { return m_confirmed; }
  bool IsNextFunc() const { return m_isNext; }
  void SetOpen(bool open) { m_open = open; }
  void SetNext(bool open) { m_isNext = open; }
  virtual std::shared_ptr<IEventCommand> getCommand() { return nullptr; }

protected:
  bool m_confirmed{false};
  bool m_open;
  bool m_isNext{false};
  std::string m_name;
};