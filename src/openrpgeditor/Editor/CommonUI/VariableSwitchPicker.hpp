#pragma once

#include "IDialogController.hpp"

#include <string>
#include <vector>

class VariableSwitchPicker : public IDialogController {
public:
  enum class Type {
    Variable,
    Switch,
  };
  VariableSwitchPicker(Type, const std::vector<std::string>& values, int initialSelection, int rangeStart = 1);
  VariableSwitchPicker(std::string_view title, const std::vector<std::string>& values, int initialSelection, int rangeStart = 1);

  int selection() const { return m_selection; }
  void setSelection(const int selection) { m_selection = selection; }

  std::tuple<bool, bool> draw() override;

private:
  const std::vector<std::string>* m_list;
  std::vector<std::pair<int, const std::string*>> m_trackedValues;
  int m_selection{1};
  bool m_navigateOnOpen{false};
  std::string m_filter;
};
