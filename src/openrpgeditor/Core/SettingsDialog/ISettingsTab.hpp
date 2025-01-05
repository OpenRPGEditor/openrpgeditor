#pragma once
#include "Utils/SignalSlot.hpp"

class SettingsDialog;
class ISettingsTab {
public:
  virtual ~ISettingsTab() = default;
  virtual void draw() = 0;

  rpgmutils::signal<void()> onValueChanged;
  rpgmutils::signal<void()> onUIRefreshRequested;
};