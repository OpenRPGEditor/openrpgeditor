#include "Editor/Translator/TranslatorDialog.hpp"

#include "Database/Database.hpp"
#include "Editor/CommonUI/GroupBox.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "orei18n.hpp"
#include "Translator.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

bool TranslatorDialog::closeAllDocuments(const bool saveDocuments) {
  bool ret = true;
  if (saveDocuments) {
    for (const auto& document : Translator::instance().documentsForLocale()) {
      if (!document->save()) {
        ret = false;
      }
    }
  }

  m_currentDocument = nullptr;
  m_highlightedDocument = nullptr;
  m_currentLocale.clear();

  return ret;
}

std::tuple<bool, bool> TranslatorDialog::draw() {
  if (!m_open) {
    return {true, m_confirmed};
  }

  // Default state
  if (m_currentLocale.empty() && Translator::instance().hasTranslations()) {
    m_currentLocale = Translator::instance().currentLocale();
  }

  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, {0.5f, 0.5f});
  ImGui::SetNextWindowSizeConstraints(ImGui::GetDPIScaledSize(1024, 512), {FLT_MAX, FLT_MAX});

  if (ImGui::Begin(std::format("{}###{}", trNOOP("Translator"), m_dialogId).c_str(), &m_open)) {
    const bool isDocked = ImGui::IsWindowDocked();
    ImGui::BeginVertical("##translator_main_layout", ImGui::GetContentRegionAvail(), 0.f);
    {
      if (ImGui::BeginChild("##translator_main_child", {-1, ImGui::GetContentRegionAvail().y - (ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y)}, 0,
                            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NavFlattened)) {
        ImGui::BeginVertical("##translator_main_child_layout", ImGui::GetContentRegionAvail(), 0.f);
        {
          ImGui::BeginHorizontal("##translator_translations_locale_and_documents_layout", {-1, 0}, 0.f);
          {
            GroupBox localesComboGroup(trNOOP("Locales"), "##translator_locales_combo_group", {std::max(ImGui::GetContentRegionAvail().x / 2, ImGui::GetDPIScaledValue(150.f)), 0});
            if (localesComboGroup.begin()) {
              if (ImGui::BeginListBox("##translator_locales_list", {-1, 0})) {
                const auto locales = Translator::instance().locales();
                for (const auto& locale : locales) {
                  if (ImGui::Selectable(std::format("{}##{}", Translator::instance().localeDescription(locale), locale).c_str(), m_currentLocale == locale)) {
                    m_currentLocale = locale;
                  }
                }
                ImGui::EndListBox();
              }
            }
            localesComboGroup.end();
            GroupBox documentsComboGroup(trNOOP("Documents"), "##translator_documents_combo_group", {std::max(ImGui::GetContentRegionAvail().x, ImGui::GetDPIScaledValue(150.f)), 0});
            if (documentsComboGroup.begin()) {
              if (ImGui::BeginListBox("##translator_documents_list", {-1, 0})) {
                const auto& documents = Translator::instance().documentsForLocale(m_currentLocale);
                for (const auto& document : documents) {
                  if (ImGui::Selectable(std::format("{}{}", document->filenameNoExtension(), document->isModified() ? "*" : "").c_str(), m_currentDocument == document,
                                        ImGuiSelectableFlags_AllowDoubleClick | (m_highlightedDocument == document && m_currentDocument != document ? ImGuiSelectableFlags_Highlight : 0))) {
                    m_highlightedDocument = document;
                    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) || ImGui::IsKeyChordPressed(ImGuiKey_Enter) || ImGui::IsKeyChordPressed(ImGuiKey_KeypadEnter)) {
                      if (m_currentDocument && !m_currentDocument->isModified()) {
                        m_currentDocument->close(false);
                      }
                      m_currentDocument = document;
                      m_currentDocument->open();
                      m_currentTranslation = -1;
                    }
                  }
                }
                ImGui::EndListBox();
              }
            }
            documentsComboGroup.end();
          }
          ImGui::EndHorizontal();
          ImGui::BeginDisabled(!m_currentDocument);
          {
            GroupBox translations(trNOOP("Translations"), "##translator_translations_group", {-1, -1}, nullptr, 0, ImGuiWindowFlags_HorizontalScrollbar);
            if (translations.begin()) {
              if (ImGui::BeginChild("##translator_translations_table_child", {ImGui::GetContentRegionAvail().x * .5f, ImGui::GetContentRegionAvail().y}, ImGuiChildFlags_ResizeX,
                                    ImGuiWindowFlags_NoBackground)) {
                if (ImGui::BeginTable("##translations_table", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY)) {
                  ImGui::TableSetupColumn(trNOOP("Key"));
                  ImGui::TableSetupColumn(trNOOP("Value"), ImGuiTableColumnFlags_NoResize);
                  ImGui::TableSetupScrollFreeze(2, 1);
                  ImGui::TableHeadersRow();

                  if (m_currentDocument) {
                    ImGuiListClipper clipper;
                    clipper.Begin(m_currentDocument->translationCount());
                    while (clipper.Step()) {
                      for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {
                        if (i < 0) {
                          continue;
                        }

                        const auto& translation = m_currentDocument->translations()[i];
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        const auto key = translation.key();
                        const auto firstKeyNewlinePos = key.find_first_of('\n');
                        const auto maxKeyLen = std::min<size_t>(128, firstKeyNewlinePos);
                        ImGui::Text("%s%s", key.substr(0, maxKeyLen).c_str(), maxKeyLen < key.length() ? "..." : "");
                        ImGui::TableNextColumn();
                        const auto value = translation.value();
                        const auto firstValueNewlinePos = value.find_first_of('\n');
                        const auto maxValueLen = std::min<size_t>(128, firstValueNewlinePos);
                        if (ImGui::Selectable(std::format("{}{}##{}", value.substr(0, maxValueLen), maxValueLen < value.length() ? "..." : "", i).c_str(), m_currentTranslation == i,
                                              ImGuiSelectableFlags_SpanAllColumns)) {
                          m_currentTranslation = i;
                        }
                        if (ImGui::BeginItemTooltip()) {
                          ImGui::TextUnformatted(key.c_str());
                          ImGui::Separator();
                          ImGui::TextUnformatted(value.c_str());
                          ImGui::EndTooltip();
                        }
                      }
                    }
                    clipper.End();
                  }
                  ImGui::EndTable();
                }
              }
              ImGui::EndChild();
              ImGui::SameLine();
              ImGui::AlignTextToFramePadding();
              const auto revertStr = trNOOP("Revert");
              const auto buttonWidth = ImGui::CalcItemSize(ImGui::CalcTextSize(revertStr), 0.f, 0.f).x + ImGui::GetStyle().FramePadding.x * 2;
              if (ImGui::BeginChild("##translator_translations_edit_panel", {-1, 0}, 0, ImGuiWindowFlags_NoBackground)) {
                ImGui::BeginDisabled(m_currentTranslation == -1);
                {
                  GroupBox keyGroup(trNOOP("Key"), "##translator_translation_key_group", {-1, 0});
                  if (keyGroup.begin()) {
                    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - buttonWidth);
                    std::string tmp = m_currentDocument && m_currentTranslation != -1 ? m_currentDocument->key(m_currentTranslation) : "";
                    ImGui::InputTextMultiline("##translation_key", &tmp);
                    if (m_currentDocument && m_currentTranslation != -1 && ImGui::IsItemDeactivatedAfterEdit()) {
                      m_currentDocument->setKey(m_currentTranslation, tmp);
                    }
                    ImGui::SameLine();
                    ImGui::AlignTextToFramePadding();
                    ImGui::BeginDisabled(!m_currentDocument || !m_currentDocument->keyModified(m_currentTranslation));
                    {
                      if (ImGui::Button(revertStr, {buttonWidth, 0})) {
                        m_currentDocument->revertKey(m_currentTranslation);
                      }
                    }
                    ImGui::EndDisabled();
                  }
                  keyGroup.end();
                  GroupBox valueGroup(trNOOP("Value"), "##translator_translation_value_group", {-1, 0});
                  if (valueGroup.begin()) {
                    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - buttonWidth);
                    std::string tmp = m_currentDocument && m_currentTranslation != -1 ? m_currentDocument->value(m_currentTranslation) : "";
                    ImGui::InputTextMultiline("##translation_value", &tmp);
                    if (m_currentDocument && m_currentTranslation != -1 && ImGui::IsItemDeactivatedAfterEdit()) {
                      m_currentDocument->setValue(m_currentTranslation, tmp);
                    }
                    ImGui::SameLine();
                    ImGui::AlignTextToFramePadding();
                    ImGui::BeginDisabled(!m_currentDocument || !m_currentDocument->valueModified(m_currentTranslation));
                    {
                      if (ImGui::Button(revertStr, {buttonWidth, 0})) {
                        m_currentDocument->revertValue(m_currentTranslation);
                      }
                    }
                    ImGui::EndDisabled();
                  }
                  valueGroup.end();
                }
                ImGui::EndDisabled();
              }
              ImGui::EndChild();
            }
            translations.end();
          }
          ImGui::EndDisabled();
        }
        ImGui::EndVertical();
      }
      ImGui::EndChild();

      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##translator_button_layout", {-1, 0}, 0.f);
      {
        ImGui::Spring();

        if (const auto ret = ImGui::ButtonGroup("##translator_dialog_button_layout", {trNOOP("OK"), trNOOP("Apply"), !isDocked ? trNOOP("Cancel") : trNOOP("Revert")}, {}, {},
                                                {false, m_currentDocument && !m_currentDocument->isModified(), isDocked ? m_currentDocument && !m_currentDocument->isModified() : false}, {!isDocked});
            ret == 0) { // OK
          if (m_currentDocument) {
            m_currentDocument->save();
          }
          m_open = false;
          m_confirmed = true;
        } else if (ret == 1) { // Apply
          if (m_currentDocument) {
            m_currentDocument->save();
          }
        } else if (ret == 2) { // Cancel/Revert
          if (m_currentDocument) {
            m_currentDocument->revert();
          }

          if (!isDocked) {
            m_open = false;
            m_confirmed = false;
          }
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
  }
  ImGui::End();

  return {!m_open, m_confirmed};
}
