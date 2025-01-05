#include "Core/FirstBootWizard/UISettingsPage.hpp"

#include "Core/Application.hpp"
#include "Core/Settings.hpp"

#include <imgui.h>

static inline float roundToNearestQuarter(float num) { return static_cast<float>(static_cast<int>(num * 4)) / 4; }

void UISettingsPage::draw() {
  ImGui::BeginChild("UISettingsPage", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiWindowFlags_AlwaysAutoResize, ImGuiWindowFlags_NoBackground);
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
  ImGui::EndChild();
}
