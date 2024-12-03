#pragma once
#include "Core/SettingsDialog//ISettingsTab.hpp"

class GeneralSettingsTab final : public ISettingsTab {
public:
  void draw() override;
};
