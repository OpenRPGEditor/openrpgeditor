#include "Core/SettingsDialog/UISettingsTab.hpp"
#include "Core/Application.hpp"
#include "Core/Settings.hpp"
#include <imgui.h>
static inline float roundToNearestQuarter(float num) { return static_cast<float>(static_cast<int>(num * 4)) / 4; }

void UISettingsTab::draw() {
  if (ImGui::BeginTabItem("UI")) {
    ImGui::Text("Font Size");
    ImGui::SliderInt("##ui_font_size", &Settings::instance()->fontSize, 8, 48);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
      App::APP->requestFontUpdate();
      m_fontSizeChanged = true;
      onValueChanged.fire();
    }
    ImGui::PushFont(App::APP->getMonoFont());
    ImGui::Text("Monospace Font Size");
    ImGui::SliderInt("##ui_mono_font_size", &Settings::instance()->monoFontSize, 8, 48);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
      App::APP->requestFontUpdate();
      m_fontSizeChanged = true;
      onValueChanged.fire();
    }
    ImGui::PopFont();
    if (m_fontSizeChanged) {
      ImGui::Text("Reloading fonts....");
      m_fontSizeChanged = !App::APP->fontUpdateRequestPerformed();
      onValueChanged.fire();
    }
    ImGui::EndTabItem();
  }
}
