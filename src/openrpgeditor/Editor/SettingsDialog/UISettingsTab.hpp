#pragma once

#include "Editor/SettingsDialog/ISettingsTab.hpp"

class UISettingsTab final : public ISettingsTab {
public:
  void draw() override;
  std::string title() const override { return tr("UI"); }
  constexpr std::string_view id() const override { return "settings_tab_ui"; };

private:
  bool m_uiScaleChanged{false};
};
