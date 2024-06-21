#pragma once

struct IDialogController {
  explicit IDialogController(const char* name) : m_open(false), m_name(name) {}
  virtual ~IDialogController() = default;
  virtual void draw() = 0;
  bool IsOpen() const { return m_open; }
  void SetOpen(bool open) { m_open = open; }

protected:
  bool m_open;
  const char* m_name;
};