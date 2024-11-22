#pragma once

#include <tuple>
#include <string>

struct IDialogController {
  IDialogController() = delete;
  explicit IDialogController(const std::string_view name) : m_name(name) {}
  virtual ~IDialogController() = default;
  virtual std::tuple<bool, bool> draw() = 0;
  virtual bool IsOpen() const { return m_open; }
  virtual bool IsConfirmed() const { return m_confirmed; }
  virtual void SetOpen(const bool open) { m_open = open; }
  virtual void SetParentIndent(const int indent) { m_parentIndent = indent; }
  virtual void Accept() {
    m_open = false;
    m_confirmed = false;
  }

protected:
  int m_parentIndent{0};
  bool m_confirmed{false};
  bool m_open{false};
  std::string m_name;
};