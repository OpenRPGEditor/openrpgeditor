#include "Core/SettingsDialog/UISettingsTab.hpp"
#include "Core/Application.hpp"
#include "Core/Settings.hpp"
#include <imgui.h>
#include <imgui_internal.h>

void UISettingsTab::draw() {
  if (ImGui::BeginTabItem("UI")) {
    ImGui::SliderFloat("##ui_scale", &Settings::instance()->uiScale, 0.5, 5);
    ImGui::PushFont(App::APP->getMainFont(), Settings::instance()->fontSize);
    ImGui::Text("Font Size");
    ImGui::SliderInt("##ui_font_size", &Settings::instance()->fontSize, 8, 48);
    // Settings::instance()->fontSize = nextMultipleOf8(Settings::instance()->fontSize);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
      App::APP->requestFontUpdate();
      m_fontSizeChanged = true;
      emit_signal(onValueChanged);
    }
    ImGui::PopFont();
    ImGui::PushFont(App::APP->getMonoFont(), Settings::instance()->monoFontSize);
    ImGui::Text("Monospace Font Size");
    ImGui::SliderInt("##ui_mono_font_size", &Settings::instance()->monoFontSize, 8, 48);
    // Settings::instance()->monoFontSize = nextMultipleOf8(Settings::instance()->monoFontSize);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
      App::APP->requestFontUpdate();
      m_fontSizeChanged = true;
      emit_signal(onValueChanged);
    }
    ImGui::PopFont();
    if (m_fontSizeChanged) {
      m_fontSizeChanged = !App::APP->fontUpdateRequestPerformed();
      emit_signal(onValueChanged);
    }
    ImGui::EndTabItem();
  }
}
