#include "Core/FirstBootWizard/UISettingsPage.hpp"

#include "Core/Application.hpp"
#include "Core/Settings.hpp"

#include <imgui.h>

static inline float roundToNearestQuarter(float num) { return static_cast<float>(static_cast<int>(num * 4)) / 4; }

void UISettingsPage::draw() {
  ImGui::BeginChild("UISettingsPage", ImVec2(0, 0),
                    ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiWindowFlags_AlwaysAutoResize,
                    ImGuiWindowFlags_NoBackground);
  ImGui::Text("UI Scale");
  if (ImGui::SliderFloat("##ui_scale", &Settings::instance()->uiScale, 1.0f, 2.0f, "%.2f")) {
    Settings::instance()->uiScale = roundToNearestQuarter(Settings::instance()->uiScale);
  }
  if (ImGui::IsItemDeactivatedAfterEdit()) {
    App::APP->requestFontUpdate();
    m_fontSizeChanged = true;
  }
  ImGui::Text("Font Size");
  ImGui::InputInt("##ui_font_size", &Settings::instance()->fontSize, 1, 1);
  if (ImGui::IsItemDeactivatedAfterEdit()) {
    App::APP->requestFontUpdate();
    m_fontSizeChanged = true;
  }
  ImGui::PushFont(App::APP->getMonoFont());
  ImGui::Text("Monospace Font Size");
  ImGui::InputInt("##ui_mono_font_size", &Settings::instance()->monoFontSize, 1, 1, ImGuiInputTextFlags_AllowTabInput);
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
