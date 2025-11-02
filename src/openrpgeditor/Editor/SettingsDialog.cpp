#include "Editor/SettingsDialog.hpp"

#include "Editor/Application.hpp"
#include "Editor/Settings.hpp"
#include "ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>

std::tuple<bool, bool> SettingsDialog::draw() {
  if (m_uiRefreshRequested) {
    App::APP->requestScaleUpdate();
    m_uiRefreshRequested = false;
    m_wasUIRefreshRequested = true;
  }

  if (isOpen()) {
    if (m_tempSettings.empty()) {
      m_tempSettings = Settings::instance()->serializeToJson();
    }
    ImGui::OpenPopup(m_name.c_str());
  }

  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImGui::GetDPIScaledSize(512, 500), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::BeginVertical("##settings_dialog_main_layout", {-1, -1}, 0);
    {
      if (ImGui::BeginTabBar("##settings_tabbar")) {
        for (const auto& tab : m_tabs) {
          if (ImGui::BeginTabItem(std::format("{}###{}", tab->title(), tab->id()).c_str())) {
            ImVec2 tmpSize = {-1, ImGui::GetDPIScaledValue(1.5f)};
            ImGui::ItemSize(tmpSize);
            if (ImGui::BeginChild(std::format("##{}", tab->id()).c_str(), {-1, 0}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                                  ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoBackground)) {
              tab->draw();
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
          }
        }
        ImGui::EndTabBar();
      }
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##settings_dialog_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##settings_dialog_buttons", {trNOOP("OK"), trNOOP("Apply"), trNOOP("Cancel")}, false, {}, {false, !m_hasChangedSettings, false}); ret == 0) {
          m_confirmed = true;
          m_open = false;

          m_wasUIRefreshRequested = false;
          m_tempSettings.clear();
          ImGui::CloseCurrentPopup();
          if (m_hasChangedSettings) {
            App::APP->serializeSettings();
          }
          m_uiRefreshRequested = false;
          m_hasChangedSettings = false;
          m_closedHere = true;
        } else if (ret == 1) {
          m_confirmed = false;
          m_open = true;

          m_tempSettings = Settings::instance()->serializeToJson();
          if (m_wasUIRefreshRequested) {
            App::APP->requestScaleUpdate();
            m_wasUIRefreshRequested = false;
          }

          if (m_hasChangedSettings) {
            App::APP->serializeSettings();
            m_hasChangedSettings = false;
          }
        } else if (ret == 2) {
          m_confirmed = false;
          m_open = false;
          m_hasChangedSettings = false;
          Settings::instance()->loadFromJson(m_tempSettings);
          m_uiRefreshRequested = true;
          m_tempSettings.clear();
          if (m_wasUIRefreshRequested) {
            App::APP->requestScaleUpdate();
            m_wasUIRefreshRequested = false;
          }
          ImGui::CloseCurrentPopup();
          m_closedHere = true;
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
    ImGui::EndPopup();

    if (!m_open && !m_closedHere) {
      Settings::instance()->loadFromJson(m_tempSettings);
      m_tempSettings.clear();
      if (m_wasUIRefreshRequested) {
        App::APP->requestScaleUpdate();
        m_wasUIRefreshRequested = false;
      }
      m_closedHere = false;
    }
  }
  return {!m_open, m_confirmed};
}

void SettingsDialog::onValueChanged() { m_hasChangedSettings = true; }
void SettingsDialog::onUIRefreshRequested() { m_uiRefreshRequested = true; }
