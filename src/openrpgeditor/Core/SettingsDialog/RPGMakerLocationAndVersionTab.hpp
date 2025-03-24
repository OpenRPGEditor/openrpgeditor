#pragma once

#include "Core/SettingsDialog/ISettingsTab.hpp"

class RPGMakerLocationAndVersionTab final : public ISettingsTab {
public:
  void draw() override;
};
