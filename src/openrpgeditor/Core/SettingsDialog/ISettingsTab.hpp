#pragma once
#include "Utils/SignalSlot.hpp"

#include <orei18n.hpp>

class SettingsDialog;
class ISettingsTab {
public:
  virtual ~ISettingsTab() = default;
  virtual void draw() = 0;
  virtual std::string title() const = 0;
  virtual constexpr std::string_view id() const = 0;

  rpgmutils::signal<void()> onValueChanged;
  rpgmutils::signal<void()> onUIRefreshRequested;
};