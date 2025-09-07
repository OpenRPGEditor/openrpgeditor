#pragma once

#include "Core/SettingsDialog/ISettingsTab.hpp"

class RPGMakerLocationAndVersionTab final : public ISettingsTab {
public:
  void draw() override;

  std::string title() const override { return tr("RPG Maker Location & Version"); }
  constexpr std::string_view id() const override { return "settings_tab_rpg_maker_location_and_version"; };
};
