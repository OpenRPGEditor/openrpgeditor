#include "Editor/Translator/TranslatorDialog.hpp"

#include "Database/Database.hpp"
#include "Editor/CommonUI/GroupBox.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "Editor/Translator/TranslationDocument.hpp"
#include "Editor/Translator/Translator.hpp"

#include "Editor/MainWindow.hpp"
#include "IconsFontAwesome6.h"
#include "orei18n.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include <OREMath/Color.hpp>

static constexpr int kStateColorAlpha = 255;
static const std::array<Color, magic_enum::enum_count<TranslationDocument::State>()> kStateColors = {{
    {255, 255, 0, kStateColorAlpha},   // Unknown
    {255, 0, 0, kStateColorAlpha},     // Untranslated
    {127, 127, 127, kStateColorAlpha}, // Draft
    {144, 238, 144, kStateColorAlpha}, // Translated light green
    {102, 255, 0, kStateColorAlpha},   // Translated and approved bright green
}};

std::tuple<bool, bool> ConfirmationDialog::draw() {
  const std::string id = std::format("###{}", m_dialogId);
  if (m_open) {
    ImGui::OpenPopup(id.c_str());
  }

  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, {0.5f, 0.5f});
  if (ImGui::BeginPopupModal(std::format("{}{}", m_title, id).c_str(), &m_open, ImGuiWindowFlags_NoResize)) {
    ImGui::BeginVertical("##confirmation_dialog_layout", {-1, -1}, 0.f);
    {
      ImGui::TextUnformatted(m_message.c_str());
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##confirmation_dialog_layout");
      {
        ImGui::Spring();
        switch (m_buttons) {
        case Buttons::YesNo:
          if (const auto ret = ImGui::ButtonGroup("##confirmation_dialog_buttons", {trNOOP("Yes"), trNOOP("No")}); ret == 0) {
            m_result = Result::Confirmed;
            setOpen(false);
            m_confirmed = true;
          } else if (ret == 1) {
            m_result = Result::Rejected;
          }
          break;
        case Buttons::YesNoCancel:
          if (const auto ret = ImGui::ButtonGroup("##confirmation_dialog_buttons", {trNOOP("Yes"), trNOOP("No"), trNOOP("Cancel")}); ret == 0) {
            m_result = Result::Confirmed;
            setOpen(false);
            m_confirmed = true;
          } else if (ret == 1 || ret == 2) {
            setOpen(false);
            m_result = ret == 1 ? Result::Rejected : Result::Cancelled;
            m_confirmed = false;
          }
          break;
        case Buttons::Ok:
          if (const auto ret = ImGui::ButtonGroup("##confirmation_dialog_buttons", {trNOOP("OK")}); ret == 0) {
            m_result = Result::Confirmed;
            setOpen(false);
            m_confirmed = true;
          }
          break;
        case Buttons::OkCancel:
          if (const auto ret = ImGui::ButtonGroup("##confirmation_dialog_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
            m_result = Result::Confirmed;
            setOpen(false);
            m_confirmed = true;
          } else if (ret == 1) {
            m_result = Result::Cancelled;
          }
          break;
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
    ImGui::EndPopup();
  }

  return {!m_open, m_confirmed};
}

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

void escapeNewlines(std::string& str) {
  size_t pos = 0;

  while ((pos = str.find("\\n", pos)) != std::string::npos) {
    str.replace(pos, 2, "\\\\n");
    pos += 4;
  }

  pos = 0;
  while ((pos = str.find('\n', pos)) != std::string::npos) {
    str.replace(pos, 1, "\\n");
    pos += 2;
  }
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
  const auto buttonSize = ImGui::CalcItemSize({ImGui::GetFontSize(), ImGui::GetFontSize()}, 0.f, 0.f).y + (ImGui::GetStyle().FramePadding.y + ImGui::GetStyle().ItemSpacing.y) * 2;

  if (ImGui::Begin(std::format("{}###{}", trNOOP("Translator"), m_dialogId).c_str(), &m_open)) {
    const bool isDocked = ImGui::IsWindowDocked();
    ImGui::BeginVertical("##translator_main_layout", ImGui::GetContentRegionAvail(), 0.f);
    {
      if (ImGui::BeginChild("##translator_main_child", {-1, ImGui::GetContentRegionAvail().y - (ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y)}, 0,
                            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NavFlattened | ImGuiWindowFlags_HorizontalScrollbar)) {
        ImGui::BeginVertical("##translator_main_child_layout", ImGui::GetContentRegionAvail(), 0.f);
        {
          ImGui::BeginHorizontal("##translator_translations_locale_and_documents_layout", {-1, 0}, 0.f);
          {
            GroupBox localesComboGroup(trNOOP("Locales"), "##translator_locales_list_group", {std::max(ImGui::GetContentRegionAvail().x / 3, ImGui::GetDPIScaledValue(150.f)), 0});
            if (localesComboGroup.begin()) {
              if (ImGui::BeginListBox("##translator_locales_list", {-1, 0})) {
                for (const auto locales = Translator::instance().locales(); const auto& locale : locales) {
                  if (ImGui::Selectable(
                          std::format("{}{}##{}", Translator::instance().localeDescription(locale), Translator::instance().defaultLocale() == locale ? trNOOP(" - [Default]") : "", locale).c_str(),
                          m_currentLocale == locale)) {
                    m_currentLocale = locale;
                  }
                }
                ImGui::EndListBox();
              }
              ImGui::BeginDisabled(Translator::instance().defaultLocale() == m_currentLocale);
              {
                if (ImGui::Button(trNOOP("Set as Default"), {-1, 0})) {
                  Translator::instance().setDefaultLocale(m_currentLocale);
                }
              }
              ImGui::EndDisabled();
            }
            localesComboGroup.end();
            GroupBox checkFirstGroup(trNOOP("Check First"), "##translator_check_first_list_group", {std::max(ImGui::GetContentRegionAvail().x / 2, ImGui::GetDPIScaledValue(150.f)), 0.f});
            if (checkFirstGroup.begin()) {
              ImGui::BeginHorizontal("##translator_check_first_list_group_layout", {-1, -1}, 0.f);
              {
                if (ImGui::BeginListBox("##translator_check_first_glob_list", {ImGui::GetContentRegionAvail().x - (buttonSize + ImGui::GetStyle().FramePadding.x), 0})) {
                  ImGui::EndListBox();
                }
                ImGui::BeginVertical("##translator_check_first_list_group_buttons");
                {
                  ImGui::TextDisabled("(?)");
                  ImGui::SetItemTooltip("%s", trNOOP(R"(Check first globs can be used to prioritize files.
This allows common strings to be quickly resolved automatically.
You can use typical regex glob syntax:
* (any text)
** (subfolders)
? (one character)
[a-z] (character range)
[!abc] (exclude)
{json,js} (options).
)"));
                  ImGui::Spring(0.5f);
                  ImGui::BeginDisabled(m_currentCheckFirst == 0 || m_currentCheckFirst == -1);
                  {
                    if (ImGui::CenteredButton(ICON_FA_SORT_UP, {buttonSize, buttonSize})) {
                      m_currentCheckFirst = Translator::instance().moveCheckFirst(m_currentCheckFirst, m_currentCheckFirst - 1);
                    }
                  }
                  ImGui::EndDisabled();
                  if (ImGui::CenteredButton("+", {buttonSize, buttonSize})) {
                    // m_showAddCheckFirstDialog = true;
                  }
                  ImGui::BeginDisabled(m_currentCheckFirst == -1);
                  {
                    if (ImGui::CenteredButton("-", {buttonSize, buttonSize})) {
                      m_removeCheckFirstConfirmationDialog.emplace(
                          trNOOP("Remove check first glob?"),
                          trFormat("Are you sure you want to remove the selected check first glob?\n\"{}\"", Translator::instance().checkFirst(m_currentCheckFirst)).c_str());
                    }
                  }
                  ImGui::EndDisabled();

                  ImGui::BeginDisabled(m_currentCheckFirst == -1 || m_currentCheckFirst >= Translator::instance().checkFirstCount());
                  {
                    if (ImGui::CenteredButton(ICON_FA_SORT_DOWN, {buttonSize, buttonSize})) {
                      m_currentCheckFirst = Translator::instance().moveCheckFirst(m_currentCheckFirst, m_currentCheckFirst + 1);
                    }
                  }
                  ImGui::EndDisabled();
                  ImGui::Spring(0.5f);
                }
                ImGui::EndVertical();
              }
              ImGui::EndHorizontal();
            }
            checkFirstGroup.end();
            GroupBox documentsComboGroup(trNOOP("Documents"), "##translator_documents_list_group", {std::max(ImGui::GetContentRegionAvail().x, ImGui::GetDPIScaledValue(150.f)), 0});
            if (documentsComboGroup.begin()) {
              if (ImGui::BeginListBox("##translator_documents_list", {-1, 0})) {
                for (const auto& documents = Translator::instance().documentsForLocale(m_currentLocale); const auto& document : documents) {
                  if (ImGui::Selectable(std::format("{}{}", document->filenameNoExtension(), document->isModified() ? "*" : "").c_str(), m_currentDocument == document,
                                        ImGuiSelectableFlags_AllowDoubleClick | (m_highlightedDocument == document && m_currentDocument != document ? ImGuiSelectableFlags_Highlight : 0))) {
                    m_highlightedDocument = document;
                    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) || ImGui::IsKeyChordPressed(ImGuiKey_Enter) || ImGui::IsKeyChordPressed(ImGuiKey_KeypadEnter)) {
                      if (m_currentDocument && !m_currentDocument->isModified()) {
                        m_currentDocument->close(false);
                      }
                      m_currentDocument = document;
                      m_currentDocument->open();

                      if (!Translator::instance().documentHasStates(m_currentLocale, m_currentDocument->filenameNoExtension()) ||
                          Translator::instance().documentStateCount(m_currentLocale, m_currentDocument->filenameNoExtension()) != m_currentDocument->translationCount()) {
                        Translator::instance().initializeDocumentStates(m_currentLocale, m_currentDocument->filenameNoExtension(), m_currentDocument->translationCount());
                      }
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
          GroupBox translations(trNOOP("Translations"), "##translator_translations_group", {-1, -1}, nullptr, 0, ImGuiWindowFlags_HorizontalScrollbar);
          if (translations.begin()) {
            ImGui::BeginHorizontal("##translator_translations_toolbar", {ImGui::GetContentRegionAvail().x, 0}, 0.f);
            {
              ImGui::BeginDisabled(!m_currentDocument);
              {
                ImGui::Spring(0.5f);
                ImGui::BeginDisabled(m_currentTranslation == -1 || !m_currentDocument || m_currentTranslation == 0);
                {
                  if (ImGui::CenteredButton(ICON_FA_BACKWARD_FAST, {buttonSize, buttonSize})) {
                    m_currentTranslation = 0;
                  }
                  ImGui::SetItemTooltip("%s", trNOOP("Go to first string"));
                }
                ImGui::EndDisabled();
                ImGui::BeginDisabled(m_currentTranslation == -1 || !m_currentDocument || m_currentTranslation == 0);
                {
                  if (ImGui::CenteredButton(ICON_FA_BACKWARD_STEP, {buttonSize, buttonSize})) {
                    m_currentTranslation = m_currentDocument->prevUntranslated(m_currentTranslation);
                  }
                  ImGui::SetItemTooltip("%s", trNOOP("Find previous untranslated string"));
                }
                ImGui::EndDisabled();
                ImGui::BeginDisabled(m_currentTranslation == -1 || !m_currentDocument || m_currentTranslation == 0);
                {
                  if (ImGui::CenteredButton(ICON_FA_BACKWARD, {buttonSize, buttonSize})) {
                    --m_currentTranslation;
                  }
                  ImGui::SetItemTooltip("%s", trNOOP("Previous string"));
                }
                ImGui::EndDisabled();
                ImGui::BeginDisabled(m_currentTranslation == -1 || !m_currentDocument || m_currentTranslation >= m_currentDocument->translationCount() - 1);
                {
                  if (ImGui::CenteredButton(ICON_FA_FORWARD, {buttonSize, buttonSize})) {
                    ++m_currentTranslation;
                  }
                  ImGui::SetItemTooltip("%s", trNOOP("Next string"));
                }
                ImGui::EndDisabled();
                ImGui::BeginDisabled(m_currentTranslation == -1 || !m_currentDocument || m_currentTranslation >= m_currentDocument->translationCount() - 1);
                {
                  if (ImGui::CenteredButton(ICON_FA_FORWARD_STEP, {buttonSize, buttonSize})) {
                    m_currentTranslation = m_currentDocument->nextUntranslated(m_currentTranslation);
                  }
                  ImGui::SetItemTooltip("%s", trNOOP("Find next untranslated string"));
                }
                ImGui::EndDisabled();
                ImGui::BeginDisabled(m_currentTranslation == -1 || !m_currentDocument || m_currentTranslation >= m_currentDocument->translationCount() - 1);
                {
                  if (ImGui::CenteredButton(ICON_FA_FORWARD_FAST, {buttonSize, buttonSize})) {
                    m_currentTranslation = m_currentDocument->translationCount() - 1;
                  }
                  ImGui::SetItemTooltip("%s", trNOOP("Go to last string"));
                }
                ImGui::EndDisabled();
                ImGui::Spring(0.5f);
              }
              ImGui::EndDisabled();
            }
            ImGui::EndHorizontal();
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
            if (ImGui::BeginChild("##translator_translations_table_child", {ImGui::GetContentRegionAvail().x * .5f, ImGui::GetContentRegionAvail().y}, ImGuiChildFlags_ResizeX,
                                  ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar)) {
              ImGui::BeginDisabled(!m_currentDocument);
              {
                if (ImGui::BeginTable("##translations_table", 2,
                                      ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                                      ImGui::GetContentRegionAvail())) {
                  ImGui::TableSetupColumn(trNOOP("Key"));
                  ImGui::TableSetupColumn(trNOOP("Value"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize);
                  ImGui::TableSetupScrollFreeze(2, 1);
                  ImGui::TableHeadersRow();

                  if (m_currentDocument) {
                    ImGuiListClipper clipper;
                    clipper.Begin(static_cast<int>(m_currentDocument->translationCount()));
                    while (clipper.Step()) {
                      for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
                        auto* translation = m_currentDocument->translation(i);
                        if (!translation) {
                          continue;
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        auto key = translation->key();
                        escapeNewlines(key);
                        ImGui::TextUnformatted(key.c_str());

                        ImGui::TableNextColumn();
                        auto value = translation->value();
                        escapeNewlines(value);

                        if (ImGui::SelectableWithBorder(std::format("{}##{}", value, i).c_str(), m_currentTranslation == i, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap)) {
                          m_currentTranslation = i;
                        }
                        if (ImGui::BeginItemTooltip()) {
                          ImGui::TextUnformatted(translation->key().c_str());
                          ImGui::Separator();
                          ImGui::TextUnformatted(translation->value().c_str());
                          ImGui::EndTooltip();
                        }
                      }
                    }
                    clipper.End();
                  }
                  ImGui::EndTable();
                }
              }
              ImGui::EndDisabled();
            }
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
            const auto revertStr = trNOOP("Revert");
            const auto buttonWidth = ImGui::CalcItemSize(ImGui::CalcTextSize(revertStr), 0.f, 0.f).x + (ImGui::GetStyle().FramePadding.x + ImGui::GetStyle().ItemSpacing.x) * 2;
            if (ImGui::BeginChild("##translator_translations_edit_panel", {-1, 0}, 0, ImGuiWindowFlags_NoBackground)) {
              ImGui::BeginDisabled(!m_currentDocument || m_currentTranslation == -1);
              {
                GroupBox statusGroup(trNOOP("Status"), "##translator_status_group", {-1, 0});
                if (statusGroup.begin()) {
                  const auto state = m_currentDocument ? m_currentDocument->state(m_currentTranslation) : TranslationDocument::State::Unknown;
                  const std::string preview = m_currentDocument && m_currentTranslation != -1 ? DecodeEnumName(state) : "";
                  ImGui::SetNextItemWidth(std::max(ImGui::GetContentRegionAvail().x - buttonWidth, ImGui::GetDPIScaledValue(100) + buttonWidth));
                  const auto oldTextColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);
                  if (m_currentDocument && m_currentTranslation != -1) {
                    ImGui::PushStyleColor(ImGuiCol_Text, kStateColors[static_cast<int>(state)].textColor().toImGuiColor());
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, kStateColors[static_cast<int>(state)].multAlpha(0.5f).toImGuiColor());
                    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, kStateColors[static_cast<int>(state)].multAlpha(0.75f).toImGuiColor());
                    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, kStateColors[static_cast<int>(state)].toImGuiColor());
                  }
                  if (ImGui::BeginCombo("##translation_status_combo", preview.c_str(), ImGuiComboFlags_NoArrowButton)) {
                    ImGui::PushStyleColor(ImGuiCol_Text, oldTextColor);
                    for (const auto& s : magic_enum::enum_values<TranslationDocument::State>()) {
                      if (ImGui::Selectable(DecodeEnumName(s).c_str(), s == state)) {
                        m_currentDocument->setState(m_currentTranslation, s);
                      }

                      if (s == state) {
                        ImGui::SetItemDefaultFocus();
                      }
                    }
                    ImGui::PopStyleColor();
                    ImGui::EndCombo();
                  }
                  if (m_currentDocument && m_currentTranslation != -1) {
                    ImGui::PopStyleColor(4);
                  }
                  ImGui::SameLine();
                  ImGui::BeginDisabled(!m_currentDocument || !m_currentDocument->stateModified(m_currentTranslation));
                  if (ImGui::Button(trNOOP("Revert"))) {
                    m_currentDocument->revertState(m_currentTranslation);
                  }
                  ImGui::EndDisabled();
                }
                statusGroup.end();
                GroupBox keyGroup(trNOOP("Key"), "##translator_translation_key_group", {std::max(ImGui::GetContentRegionAvail().x, ImGui::GetDPIScaledValue(100.f) + buttonWidth), 0});
                if (keyGroup.begin()) {
                  ImGui::SetNextItemWidth(std::max(ImGui::GetContentRegionAvail().x - buttonWidth, ImGui::GetDPIScaledValue(100) + buttonWidth));
                  std::string tmp = m_currentDocument && m_currentTranslation != -1 ? m_currentDocument->key(m_currentTranslation) : "";
                  ImGui::InputTextMultiline("##translation_key", &tmp);
                  if (m_currentDocument && m_currentTranslation != -1 && ImGui::IsItemDeactivatedAfterEdit()) {
                    m_currentDocument->setKey(m_currentTranslation, tmp);
                  }
                  ImGui::SameLine();
                  ImGui::AlignTextToFramePadding();
                  ImGui::BeginDisabled(!m_currentDocument || !m_currentDocument->keyModified(m_currentTranslation));
                  {
                    if (ImGui::Button(revertStr, {-1, 0})) {
                      m_currentDocument->revertKey(m_currentTranslation);
                    }
                  }
                  ImGui::EndDisabled();
                }
                keyGroup.end();
                GroupBox valueGroup(trNOOP("Value"), "##translator_translation_value_group", {std::max(ImGui::GetContentRegionAvail().x, ImGui::GetDPIScaledValue(100.f) + buttonWidth), 0});
                if (valueGroup.begin()) {
                  ImGui::SetNextItemWidth(std::max(ImGui::GetContentRegionAvail().x - buttonWidth, ImGui::GetDPIScaledValue(100) + buttonWidth));
                  std::string tmp = m_currentDocument && m_currentTranslation != -1 ? m_currentDocument->value(m_currentTranslation) : "";
                  ImGui::InputTextMultiline("##translation_value", &tmp);
                  if (m_currentDocument && m_currentTranslation != -1 && ImGui::IsItemDeactivatedAfterEdit()) {
                    m_currentDocument->setValue(m_currentTranslation, tmp);
                  }
                  ImGui::SameLine();
                  ImGui::AlignTextToFramePadding();
                  ImGui::BeginDisabled(!m_currentDocument || !m_currentDocument->valueModified(m_currentTranslation));
                  {
                    if (ImGui::Button(revertStr, {-1, 0})) {
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
            (void)m_currentDocument->save();
          }
          m_open = false;
          m_confirmed = true;
        } else if (ret == 1) { // Apply
          if (m_currentDocument) {
            (void)m_currentDocument->save();
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

  // this check is disabled since it's incorrect, the inspector claims that the result will always be false.
  // ReSharper disable once CppDFAConstantConditions
  return {!m_open, m_confirmed};
}
