#include "Core/DatabaseEditor/DBLocaleTab.hpp"
#include "Core/Application.hpp"
#include "Core/DatabaseEditor.hpp"
#include "Core/ImGuiExt/ImGuiNotify.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/MainWindow.hpp"
#include "imgui.h"
#include <cstring>
#include <misc/cpp/imgui_stdlib.h>
#include <numeric>
DBLocaleTab::DBLocaleTab(Locales& locales, DatabaseEditor* parent)
: IDBEditorTab(parent)
, m_locales(&locales) {
  std::string path = Database::instance()->basePath + "locales/"; // Replace with your directory path

  try {
    if (fs::exists(path) && fs::is_directory(path)) {
      for (const auto& entry : fs::directory_iterator(path)) {
        if (is_directory(entry.status())) {
          m_localeList.push_back(entry.path().filename().generic_string());
        }
      }
    }
  } catch (const fs::filesystem_error& e) { std::cout << "Error: " << e.what() << std::endl; }

  m_jpLocales.loadMap(Database::instance()->basePath + std::format("locales/ja/Map0{:03}.json", m_selectedMap));

  for (int i = 0; i < m_locales->locales.size(); i++) {
    if (m_locales->locales.at(i).second.empty() || m_jpLocales.locales.at(i).second.empty()) {
      m_untranslatedIndices.push_back(i);
    }
  }
}

void DBLocaleTab::draw() {
  const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");

  ImGui::BeginChild("##orpg_locales_navigation", ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0}, 0, ImGuiWindowFlags_NoScrollbar);
  {
    ImGui::BeginGroup();
    {
      ImGui::BeginChild("##orpg_locales_map_navigation_bar", ImVec2{0, 30.f});
      {
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::BeginCombo("##orpg_locales_map_combo", Database::instance()->mapInfos.map(m_selectedMap)->name().c_str())) {
          for (int i = 1; i < Database::instance()->mapInfos.mapInfos().size(); i++) {
            bool is_selected = m_selectedMap == i;
            if (ImGui::Selectable(std::format("{}##orpg_locales_map_selectable", Database::instance()->mapInfos.map(i)->name()).c_str(), is_selected)) {
              m_selectedMap = i;
              m_editingIndex = -1;
              m_selectedIndex = -1;
              m_locales->loadMap(Database::instance()->basePath + std::format("locales/en/Map0{:03}.json", m_selectedMap));
              m_jpLocales.loadMap(Database::instance()->basePath + std::format("locales/ja/Map0{:03}.json", m_selectedMap));

              m_untranslatedIndices.clear();
              for (int i = 0; i < m_locales->locales.size(); i++) {
                if (m_locales->locales.at(i).second.empty() || m_jpLocales.locales.at(i).second.empty()) {
                  m_untranslatedIndices.push_back(i);
                }
              }
              if (is_selected) {
                ImGui::SetItemDefaultFocus();
              }
            }
          }
          ImGui::EndCombo();
        }
        ImGui::EndChild();
      }
      ImGui::BeginChild("##orpg_locales_map_navigation_subbar", ImVec2{ImGui::GetContentRegionAvail().x - (ImGui::GetStyle().FramePadding.x * 55), 80.f});
      {
        ImGui::BeginDisabled(m_editingIndex == -1);
        if (ImGui::Button("\u274C", {30, 30})) {
          m_editingIndex = -1;
        }
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        if (ImGui::Button("\u002B", {30, 30})) {
          m_locales->locales.push_back(std::make_pair(std::to_string(m_locales->locales.size() - 1), ""));
          m_jpLocales.locales.push_back(std::make_pair(std::to_string(m_locales->locales.size() - 1), ""));
          m_selectedUntranslated = m_locales->locales.size() - 1;
          m_navigator = true;
        }
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        ImGui::BeginDisabled(m_locales->locales.empty());
        if (ImGui::Button(ICON_FA_FLOPPY_DISK, {30, 30})) {
          m_editingIndex = -1;

          if (m_mainLanguageDirty) {
            m_locales->saveCurrentLocale();
          } else {
            m_jpLocales.saveCurrentLocale();
          }
          m_compareLanguageDirty = false;
          m_mainLanguageDirty = false;
        }
        ImGui::SetItemTooltip("%s", trNOOP("Save pending changes if you are editing text."));
        ImGui::SameLine();
        if (ImGui::Button("S", {30, 30})) {
          if (m_mainLanguageEdit && m_editingIndex > -1) {
            m_locales->locales.at(m_editingIndex).second += "\\S[value]\\S[value]";
          } else if (m_mainLanguageEdit == false && m_editingIndex > -1) {
            m_jpLocales.locales.at(m_editingIndex).second += "\\S[value]\\S[value]";
          }
        }
        ImGui::SetItemTooltip("%s", trNOOP("Sets the speed at which the text displays."));
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        if (ImGui::Button("V", {30, 30})) {
          if (m_mainLanguageEdit && m_editingIndex > -1) {
            m_locales->locales.at(m_editingIndex).second += "\\V[value]";
          } else if (m_mainLanguageEdit == false && m_editingIndex > -1) {
            m_jpLocales.locales.at(m_editingIndex).second += "\\V[value]";
          }
        }
        ImGui::SetItemTooltip("%s", trNOOP("Displays the value of a variable."));
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        if (ImGui::Button("N", {30, 30})) {
          if (m_mainLanguageEdit && m_editingIndex > -1) {
            m_locales->locales.at(m_editingIndex).second += "\\N[value]";
          } else if (m_mainLanguageEdit == false && m_editingIndex > -1) {
            m_jpLocales.locales.at(m_editingIndex).second += "\\N[value]";
          }
        }
        ImGui::SetItemTooltip("%s", trNOOP("Display the name of the actor by index."));
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        if (ImGui::Button("P", {30, 30})) {
          if (m_mainLanguageEdit && m_editingIndex > -1) {
            m_locales->locales.at(m_editingIndex).second += "\\P[value]";
          } else if (m_mainLanguageEdit == false && m_editingIndex > -1) {
            m_jpLocales.locales.at(m_editingIndex).second += "\\P[value]";
          }
        }
        ImGui::SetItemTooltip("%s", trNOOP("Display the name of the party member by index."));
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        if (ImGui::Button("C", {30, 30})) {
          if (m_mainLanguageEdit && m_editingIndex > -1) {
            m_locales->locales.at(m_editingIndex).second += "\\C[value]";
          } else if (m_mainLanguageEdit == false && m_editingIndex > -1) {
            m_jpLocales.locales.at(m_editingIndex).second += "\\C[value]";
          }
        }
        ImGui::SetItemTooltip("%s", trNOOP("Draw the text following in the specified color index. Text color is based off Window.png system image."));
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        if (ImGui::Button("G", {30, 30})) {
          if (m_mainLanguageEdit && m_editingIndex > -1) {
            m_locales->locales.at(m_editingIndex).second += "\\G[value]";
          } else if (m_mainLanguageEdit == false && m_editingIndex > -1) {
            m_jpLocales.locales.at(m_editingIndex).second += "\\G[value]";
          }
        }
        ImGui::SetItemTooltip("%s", trNOOP("Shows the currency unit."));
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        if (ImGui::Button("I", {30, 30})) {
          if (m_mainLanguageEdit && m_editingIndex > -1) {
            m_locales->locales.at(m_editingIndex).second += "\\I[value]";
          } else if (m_mainLanguageEdit == false && m_editingIndex > -1) {
            m_jpLocales.locales.at(m_editingIndex).second += "\\I[value]";
          }
        }
        ImGui::SetItemTooltip("%s", trNOOP("Displays an icon by its index."));
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        if (ImGui::Button("{", {30, 30})) {
          if (m_mainLanguageEdit && m_editingIndex > -1) {
            m_locales->locales.at(m_editingIndex).second += "\\{";
          } else if (m_mainLanguageEdit == false && m_editingIndex > -1) {
            m_jpLocales.locales.at(m_editingIndex).second += "\\{";
          }
        }
        ImGui::SetItemTooltip("%s", trNOOP("Increase the text by 1 step."));
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        if (ImGui::Button("}", {30, 30})) {
          if (m_mainLanguageEdit && m_editingIndex > -1) {
            m_locales->locales.at(m_editingIndex).second += "\\}";
          } else if (m_mainLanguageEdit == false && m_editingIndex > -1) {
            m_jpLocales.locales.at(m_editingIndex).second += "\\}";
          }
        }
        ImGui::SetItemTooltip("%s", trNOOP("Decrease the text by 1 step."));
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        if (ImGui::Button("$", {30, 30})) {
          if (m_mainLanguageEdit && m_editingIndex > -1) {
            m_locales->locales.at(m_editingIndex).second += "\\$";
          } else if (m_mainLanguageEdit == false && m_editingIndex > -1) {
            m_jpLocales.locales.at(m_editingIndex).second += "\\$";
          }
        }
        ImGui::SetItemTooltip("%s", trNOOP("Open the gold window."));
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        if (ImGui::Button(".", {30, 30})) {
          if (m_mainLanguageEdit && m_editingIndex > -1) {
            m_locales->locales.at(m_editingIndex).second += "\\.";
          } else if (m_mainLanguageEdit == false && m_editingIndex > -1) {
            m_jpLocales.locales.at(m_editingIndex).second += "\\.";
          }
        }
        ImGui::SetItemTooltip("%s", trNOOP("Wait for 1/4 second."));
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        if (ImGui::Button("|", {30, 30})) {
          if (m_mainLanguageEdit && m_editingIndex > -1) {
            m_locales->locales.at(m_editingIndex).second += "\\|";
          } else if (m_mainLanguageEdit == false && m_editingIndex > -1) {
            m_jpLocales.locales.at(m_editingIndex).second += "\\|";
          }
        }
        ImGui::SetItemTooltip("%s", trNOOP("Wait for 1 second."));
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        if (ImGui::Button("!", {30, 30})) {
          if (m_mainLanguageEdit && m_editingIndex > -1) {
            m_locales->locales.at(m_editingIndex).second += "\\!";
          } else if (m_mainLanguageEdit == false && m_editingIndex > -1) {
            m_jpLocales.locales.at(m_editingIndex).second += "\\!";
          }
        }
        ImGui::SetItemTooltip("%s", trNOOP("Wait for button input."));
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        if (ImGui::Button(">", {30, 30})) {
          if (m_mainLanguageEdit && m_editingIndex > -1) {
            m_locales->locales.at(m_editingIndex).second += "\\>";
          } else if (m_mainLanguageEdit == false && m_editingIndex > -1) {
            m_jpLocales.locales.at(m_editingIndex).second += "\\>";
          }
        }
        ImGui::SetItemTooltip("%s", trNOOP("Display remaining text on the same line all at once."));
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        if (ImGui::Button("<", {30, 30})) {
          if (m_mainLanguageEdit && m_editingIndex > -1) {
            m_locales->locales.at(m_editingIndex).second += "\\<";
          } else if (m_mainLanguageEdit == false && m_editingIndex > -1) {
            m_jpLocales.locales.at(m_editingIndex).second += "\\<";
          }
        }
        ImGui::SetItemTooltip("%s", trNOOP("Cancels display all effect."));
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        if (ImGui::Button("^", {30, 30})) {
          if (m_mainLanguageEdit && m_editingIndex > -1) {
            m_locales->locales.at(m_editingIndex).second += "\\^";
          } else if (m_mainLanguageEdit == false && m_editingIndex > -1) {
            m_jpLocales.locales.at(m_editingIndex).second += "\\^";
          }
        }
        ImGui::SetItemTooltip("%s", trNOOP("Do not wait for input after text display."));
        ImGui::SameLine();

        ImGui::SetNextItemWidth(100);
        if (ImGui::BeginCombo("##orpg_locales_language_selection", m_localeList.empty() || m_selectedLanguage >= m_localeList.size() ? "" : m_localeList.at(m_selectedLanguage).c_str())) {
          int localeIndex{0};
          for (auto& locale : m_localeList) {
            bool is_selected = m_selectedLanguage == localeIndex;
            if (ImGui::Selectable(locale.c_str(), is_selected)) {
              m_selectedLanguage = localeIndex;
              // TODO: Load selected locale
              if (is_selected) {
                ImGui::SetItemDefaultFocus();
              }
            }
            localeIndex++;
          }
          ImGui::EndCombo();
        }
        ImGui::EndDisabled();
        // Second line
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.f);
        if (ImGui::Button("\u21E6", {30, 30})) {
          if (!m_untranslatedIndices.empty()) {
            if (m_selectedUntranslated == 0) {
              m_selectedUntranslated = m_untranslatedIndices.size() - 1;
            } else {
              m_selectedUntranslated--;
            }
            m_selectedIndex = m_untranslatedIndices.at(m_selectedUntranslated);
            m_navigator = true;
          }
        }
        ImGui::SetItemTooltip("%s", trNOOP("Navigates to the previous untranslated pair."));
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        if (ImGui::Button("\u21E8", {30, 30})) {
          if (!m_untranslatedIndices.empty()) {
            if (m_selectedUntranslated == m_untranslatedIndices.size() - 1) {
              m_selectedUntranslated = 0;
            } else {
              m_selectedUntranslated++;
            }
            m_selectedIndex = m_untranslatedIndices.at(m_selectedUntranslated);
            m_navigator = true;
          }
        }
        ImGui::SetItemTooltip("%s", trNOOP("Navigates to the next untranslated pair."));
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        if (ImGui::Button("\u24E4", {30, 30})) {
          m_filterUntranslated = !m_filterUntranslated;
          m_selectedUntranslated = -1;
          m_selectedIndex = -1;
          m_navigator = false;
        }
        ImGui::SetItemTooltip("%s", trNOOP("Shows only untranslated pairs or cancels untranslated navigation."));
        ImGui::SameLine();
        ImGui::SetNextItemWidth(400);
        ImGui::InputText("##orpg_locales_search_bar", &m_searchString);

        ImGui::EndChild();
      }
      ImGui::SameLine();
      ImVec2 cursorPos = ImGui::GetCursorPos();
      ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{0.2f, 0.3f, 0.6f, 1.0f});
      ImGui::BeginChild("##orpg_locales_text_background", ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 80}, ImGuiChildFlags_FrameStyle);
      { ImGui::EndChild(); }
      ImGui::PopStyleColor();
      if (m_selectedIndex > -1) {
        ImGui::SetCursorPos(ImVec2{cursorPos.x + 10.f, cursorPos.y});
        if (m_mainLanguageEdit) {
          ImGui::TextUnformatted(m_locales->locales.at(m_selectedIndex).second.c_str());
        } else {
          ImGui::TextUnformatted(m_jpLocales.locales.at(m_selectedIndex).second.c_str());
        }
      }
    }
    ImGui::EndGroup();

    ImGui::BeginChild("##orpg_locales_map_navigation_table",
                      ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y});
    {
      if (ImGui::BeginTable("##orpg_locale_text_table", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableRowFlags_Headers | ImGuiTableFlags_ScrollY | ImGuiTableColumnFlags_WidthFixed,
                            ImVec2{ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y})) {

        ImGui::TableSetupColumn("English");
        ImGui::TableSetupColumn("Japanese");
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        int index{0};
        for (auto& pair : m_locales->locales) {

          if (!m_searchString.empty()) {
            if (pair.second.contains(m_searchString)) {
              drawTableOne(pair.second, index);
              ImGui::TableNextColumn();

              drawTableTwo(index);
              ImGui::TableNextColumn();
            } else {
              if (m_filterUntranslated) {
                if ((pair.second.empty() || m_jpLocales.locales.at(index).second.empty()) && m_filterUntranslated) {
                  drawTableOne(pair.second, index);
                  ImGui::TableNextColumn();

                  drawTableTwo(index);
                  ImGui::TableNextColumn();
                }
              }
            }
          } else {
            drawTableOne(pair.second, index);
            ImGui::TableNextColumn();
          }
          if (index < m_jpLocales.locales.size()) {
            if (m_searchString.empty() && m_filterUntranslated == false) {
              drawTableTwo(index);
              ImGui::TableNextColumn();
            }
          } else {
            // Do we add the empty indexes?
            m_jpLocales.locales.push_back(std::make_pair(std::to_string(m_jpLocales.locales.size()), "Begin editing text!"));
          }
          index++;
        }
        ImGui::EndTable();
      }
      ImGui::EndChild();
    }
    ImGui::EndChild();
  }
}

void DBLocaleTab::drawTableOne(std::string& text, int index) {
  if (m_editingIndex == index && m_mainLanguageEdit == true) {
    if (ImGui::InputTextMultiline(std::format("##orpg_locale_text_input_{}_{}", index, 1).c_str(), &text, ImVec2{600, 100}), ImGuiInputTextFlags_NoHorizontalScroll) {
      m_mainLanguageDirty = true;
    }
  } else {

    bool is_selected = m_selectedIndex == index;
    if (ImGui::Selectable(std::format("{}##orpg_locale_text_{}_{}", text, index, 1).c_str(), is_selected, 0, ImVec2{600, 80})) {
      m_mainLanguageEdit = true;
      m_selectedIndex = index;
    }
    if (!m_untranslatedIndices.empty() && m_selectedUntranslated > -1) {
      if (m_untranslatedIndices.at(m_selectedUntranslated) == index) {
        if (m_navigator) {
          ImGui::SetScrollHereY();
          m_navigator = false;
        }
      }
    }
    if (ImGui::IsItemHovered() && ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
      m_editingIndex = index;
      m_mainLanguageEdit = true;
      m_selectedIndex = -1;
    }
  }
}

void DBLocaleTab::drawTableTwo(int index) {
  if (m_editingIndex == index && m_mainLanguageEdit == false) {
    if (ImGui::InputTextMultiline(std::format("##orpg_locale_text_input_{}_{}", index, 2).c_str(), &m_jpLocales.locales.at(index).second, ImVec2{600, 100}), ImGuiInputTextFlags_NoHorizontalScroll) {
      m_compareLanguageDirty = true;
    }
  } else {
    bool is_selected = m_selectedIndex == index;
    if (ImGui::Selectable(std::format("{}##orpg_locale_text_{}_{}", m_jpLocales.locales.at(index).second, index, 2).c_str(), is_selected, 0, ImVec2{600, 80})) {
      m_selectedIndex = index;
      m_mainLanguageEdit = false;
    }
    if (ImGui::IsItemHovered() && ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
      m_editingIndex = index;
      m_selectedIndex = -1;
    }
  }
}