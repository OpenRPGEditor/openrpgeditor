#pragma once

#include "Core/SettingsDialog/ISettingsTab.hpp"

class UISettingsTab final : public ISettingsTab {
public:
  void draw() override;

private:
  bool m_fontSizeChanged{false};
};
