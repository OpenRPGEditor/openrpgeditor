#pragma once

#include <string>
#include <tuple>

struct IDialogController {
  IDialogController() = delete;
  explicit IDialogController(const std::string_view name)
  : m_name(name) {}
  virtual ~IDialogController() = default;
  virtual std::tuple<bool, bool> draw() = 0;
  [[nodiscard]] virtual bool isOpen() const { return m_open; }
  [[nodiscard]] virtual bool isConfirmed() const { return m_confirmed; }
  virtual void setOpen(const bool open) { m_open = open; }
  virtual void accept() {
    m_open = false;
    m_confirmed = false;
  }

protected:
  bool m_confirmed{false};
  bool m_open{false};
  std::string m_name;
};