#pragma once

#include <string>
#include <vector>

struct VariableSwitchPicker {
  VariableSwitchPicker(const std::string_view objectType, std::vector<std::string>& values);

  int selection() const { return m_selection; }
  void setSelection(const int selection) { m_selection = selection; }

  std::tuple<bool, bool> draw();

private:
  std::string m_objectType;
  std::vector<std::string>* m_list;
  std::vector<std::pair<int, std::string*>> m_trackedValues;

  bool m_confirmed{false};
  int m_selection{1};
  bool m_open = true;
  std::string m_filter;
};
