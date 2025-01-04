#include "Core/SettingsDialog/UISettingsTab.hpp"
#include "Core/Application.hpp"
#include "Core/Settings.hpp"
#include <imgui.h>
static inline float roundToNearestQuarter(float num) { return static_cast<float>(static_cast<int>(num * 4)) / 4; }

void UISettingsTab::draw() {
  if (ImGui::BeginTabItem("UI")) {
    if (ImGui::SliderFloat("##ui_scale", &Settings::instance()->uiScale, 1.0f, 2.0f, "%.2f")) {
      Settings::instance()->uiScale = roundToNearestQuarter(Settings::instance()->uiScale);
    }
    if (ImGui::IsItemDeactivatedAfterEdit()) {
      App::APP->updateScale();
    }
    ImGui::Text("Font Size");
    ImGui::SliderInt("##ui_font_size", &Settings::instance()->fontSize, 8, 21);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
      App::APP->requestFontUpdate();
      m_fontSizeChanged = true;
    }
    ImGui::PushFont(App::APP->getMonoFont());
    ImGui::Text("Monospace Font Size");
    ImGui::SliderInt("##ui_mono_font_size", &Settings::instance()->monoFontSize, 7, 20);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
      App::APP->requestFontUpdate();
      m_fontSizeChanged = true;
    }
    ImGui::PopFont();
    if (m_fontSizeChanged) {
      ImGui::Text("Reloading fonts....");
      m_fontSizeChanged = !App::APP->fontUpdateRequestPerformed();
    }
    ImGui::EndTabItem();
  }
}
