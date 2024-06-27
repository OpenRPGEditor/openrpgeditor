#pragma once
#include <string>

struct IDialogController {
  explicit IDialogController(const std::string& name) : m_open(false), m_name(name) {}
  virtual ~IDialogController() = default;
  virtual std::tuple<bool, bool> draw() = 0;
  bool IsOpen() const { return m_open; }
  void SetOpen(bool open) { m_open = open; }

protected:
  bool m_confirmed{false};
  bool m_open;
  std::string m_name;
};