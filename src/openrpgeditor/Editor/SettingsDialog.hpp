#pragma once
#include "CommonUI/IDialogController.hpp"
#include "SettingsDialog/ISettingsTab.hpp"

#include <memory>
#include <nlohmann/json.hpp>
#include <vector>

class SettingsDialog final : public IDialogController {
public:
  SettingsDialog()
  : IDialogController("Settings") {}

  std::tuple<bool, bool> draw() override;

  void addTab(ISettingsTab* settingsTab) {
    if (!settingsTab) {
      return;
    }
    m_tabs.emplace_back(settingsTab);
    m_tabs.back()->onValueChanged.connect<&SettingsDialog::onValueChanged>(this);
    m_tabs.back()->onUIRefreshRequested.connect<&SettingsDialog::onUIRefreshRequested>(this);
  }

private:
  void onValueChanged();
  void onUIRefreshRequested();
  nlohmann::ordered_json m_tempSettings;
  std::vector<std::unique_ptr<ISettingsTab>> m_tabs;
  bool m_hasChangedSettings{false};
  bool m_uiRefreshRequested{false};
  bool m_wasUIRefreshRequested{false};
  bool m_closedHere{false};
};
