#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

#include <optional>
#include <string>

struct IDialogController {
  explicit IDialogController(const std::string& name) : m_open(false), m_name(name) {}
  virtual ~IDialogController() = default;
  virtual std::tuple<bool, bool> draw() = 0;
  bool IsOpen() const { return m_open; }
  bool IsConfirmed() const { return m_confirmed; }
  bool IsNextFunc() const { return m_isNext; }
  void SetOpen(bool open) { m_open = open; }
  virtual std::shared_ptr<IEventCommand> getCommand() { return nullptr; }
  virtual std::shared_ptr<std::vector<std::optional<IEventCommand>>> getNextCommands() { return std::shared_ptr<std::vector<std::optional<IEventCommand>>>(); }

protected:
  bool m_isNext{false};
  bool m_confirmed{false};
  bool m_open;
  std::string m_name;
};