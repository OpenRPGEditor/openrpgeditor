#pragma once
#include "Core/SettingsDialog/ISettingsTab.hpp"

class GeneralSettingsTab final : public ISettingsTab {
public:
  void draw() override;
  std::string title() const override { return tr("General"); }
  constexpr std::string_view id() const override { return "settings_tab_general"; };

private:
  char m_confirmationText[22] = {};
};
