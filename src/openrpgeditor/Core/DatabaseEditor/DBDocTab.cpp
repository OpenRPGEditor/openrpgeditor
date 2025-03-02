#include "Core/DatabaseEditor/DBDocTab.hpp"
#include "Core/Application.hpp"
#include "Core/DatabaseEditor.hpp"
#include "Core/ImGuiExt/ImGuiNotify.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/MainWindow.hpp"
#include "imgui.h"
#include <cstring>
#include <misc/cpp/imgui_stdlib.h>
#include <numeric>
DBDocTab::DBDocTab(Docs& docs, DatabaseEditor* parent) : IDBEditorTab(parent), m_docs(&docs) {
  if (m_docs->docs.size() == 0) {
    Doc doc;

    doc.setNames({"Example Name 1", "Example Name 2", "Example Name 3"});
    doc.setTexts({" Example Text 1", "Example Text 2", "Example Test 3"});

    m_docs->docs.emplace_back(doc);

    Save();
  }
  m_names = m_docs->docs.front().names();
  m_texts = m_docs->docs.front().texts();

  createHeaders();
}

void DBDocTab::draw() {
  const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
  ImGui::BeginChild("##orpg_docs_navigation", ImVec2{calc.x + (ImGui::GetStyle().ItemSpacing.x * 2), 0}, 0, ImGuiWindowFlags_NoScrollbar);
  {
    ImGui::BeginGroup();
    {
      ImGui::BeginChild("##orpg_docs_navigation_headers", ImVec2{calc.x + (ImGui::GetStyle().ItemSpacing.x * 2), ImGui::GetContentRegionMax().y - 35});
      {
        ImGui::BeginGroup();
        {
          if (ImGui::Button("+##orpg_docs_add_category", ImVec2(40, 0))) {
            m_selectedCategory = m_names.size();
            m_names.emplace_back(trNOOP("New Category"));
            m_texts.emplace_back(trNOOP("Begin adding text!"));
            m_docs->docs.front().setNames(m_names);
            m_docs->docs.front().setTexts(m_texts);
            Save();
            setDoc(m_selectedCategory);
          }
          if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            {
              ImGui::TextUnformatted(trNOOP("Adds a new category."));
              ImGui::EndTooltip();
            }
          }
          ImGui::SameLine();
          ImGui::BeginDisabled(m_names.empty());
          if (ImGui::Button("-##orpg_docs_remove_category", ImVec2(40, 0))) {
            // int indexToRemove = m_selectedCategory;
            // if (m_selectedCategory > 0) {
            //   m_selectedCategory--;
            // }
            // if (indexToRemove >= 0 && indexToRemove < m_names.size()) {
            //   m_names.erase(m_names.begin() + indexToRemove);
            // }
            // if (indexToRemove >= 0 && indexToRemove < m_texts.size()) {
            //   m_texts.erase(m_texts.begin() + indexToRemove);
            // }
            // m_docs->docs.front().setNames(m_names);
            // m_docs->docs.front().setTexts(m_texts);
            // Save();
            // setDoc(m_selectedCategory);
          }
          if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            {
              ImGui::TextUnformatted(trNOOP("Removes a category."));
              ImGui::EndTooltip();
            }
          }
          ImGui::EndDisabled();
          ImGui::SameLine();
          ImGui::BeginDisabled(m_selectedCategory == 0);
          if (ImGui::Button("↑##orpg_docs_sort_up", ImVec2(40, 0))) {
            std::swap(m_names.at(m_selectedCategory), m_names.at(m_selectedCategory - 1));
            std::swap(m_texts.at(m_selectedCategory), m_texts.at(m_selectedCategory - 1));
            m_docs->docs.front().setNames(m_names);
            m_docs->docs.front().setTexts(m_texts);
            setDoc(m_selectedCategory - 1);
          }
          if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            {
              ImGui::TextUnformatted(trNOOP("Moves the selected category up by one on its index."));
              ImGui::EndTooltip();
            }
          }
          ImGui::EndDisabled();
          ImGui::SameLine();
          ImGui::BeginDisabled(m_selectedCategory == m_names.size() - 1);
          if (ImGui::Button("↓##orpg_docs_sort_down", ImVec2(40, 0))) {
            std::swap(m_names.at(m_selectedCategory), m_names.at(m_selectedCategory + 1));
            std::swap(m_texts.at(m_selectedCategory), m_texts.at(m_selectedCategory + 1));
            m_docs->docs.front().setNames(m_names);
            m_docs->docs.front().setTexts(m_texts);
            setDoc(m_selectedCategory + 1);
          }
          ImGui::EndDisabled();
          if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            {
              ImGui::TextUnformatted(trNOOP("Moves the selected category down by one on its index."));
              ImGui::EndTooltip();
            }
          }
          ImGui::EndGroup();
        }
        ImGui::BeginGroup();
        {
          ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x);
          if (ImGui::BeginCombo(std::format("##orpg_navigation_combo_{}", m_selectedCategory).c_str(),
                                m_names.empty() ? "Add a new category" : m_docs->docs.at(0).names().at(m_selectedCategory).c_str(), ImGuiComboFlags_NoArrowButton)) {
            for (int i = 0; i < m_docs->docs.at(0).names().size(); i++) {
              if (ImGui::Selectable(std::format("{}##{}", m_docs->docs.at(0).names().at(i), i).c_str(), i == m_selectedCategory)) {
                setDoc(i);
                // SetScroll
              }
            }
            ImGui::EndCombo();
          }
          ImGui::SeparatorText("Headers");
          if (headerTexts.size() == 0) {
            ImGui::TextUnformatted(trNOOP("No headers available"));
          } else {
            int index{0};
            for (auto& header : headerTexts) {
              char name[4096];
              snprintf(name, 4096, "%s", header.front().c_str());
              if (ImGui::Selectable(std::format("{}##headerText_{}", name, index).c_str(), index == m_selectedHeader) || (ImGui::IsItemFocused() && m_selectedHeader != index)) {

                m_selectedHeader = index;
              }
              if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                std::string text;
                for (auto iter = headerTexts.at(m_selectedHeader).begin() + 1; iter != headerTexts.at(m_selectedHeader).end(); ++iter) {
                  text += *iter + "\n";
                }
                headerText = text;
                referenceText = text;
                m_isEdittingHeader = true;
              }
              index++;
            }
          }
        }
        ImGui::EndGroup();
      }
      ImGui::EndChild();
    }
    ImGui::EndGroup();
  }
  ImGui::EndChild();
  ImGui::SameLine();
  ImGui::BeginChild("##orpg_docs_editor");
  {
    // Menu Bar above editor
    ImGui::BeginGroup();
    {
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.f);
      ImGui::TextUnformatted(trNOOP("Name:"));
      ImGui::SameLine();
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
      char name[4096];
      strncpy(name, m_names.empty() ? "" : m_names.at(m_selectedCategory).c_str(), 4096);
      ImGui::SetNextItemWidth(200);
      ImGui::BeginDisabled(!m_isEdittingText);
      if (ImGui::InputText(std::format("##orpg_doc_category_editor_name{}", m_selectedCategory).c_str(), name, 4096)) {
        m_names.at(m_selectedCategory) = name;
        m_isEdittingNames = true;
      }
      ImGui::EndDisabled();
      ImGui::SameLine();
      ImGui::BeginDisabled(m_isEdittingText);
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
      ImGui::BeginDisabled(m_texts.empty());
      if (ImGui::Button(trNOOP("Edit"), ImVec2(100, 30))) {
        m_isEdittingText = !m_isEdittingText;
      }
      ImGui::EndDisabled();
      ImGui::EndDisabled();
      ImGui::SameLine();
      ImGui::BeginDisabled(!(m_isEdittingText || m_isEdittingHeader));
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
      if (ImGui::Button(trNOOP("Apply"), ImVec2(100, 30))) {
        if (m_isEdittingHeader) {
          std::vector<std::string> text = m_docs->docs.front().texts();
          if (text.at(m_selectedCategory).contains(referenceText)) {
            size_t beginPos = text.at(m_selectedCategory).find(referenceText);
            size_t count = 1;
            size_t pos = 0;

            // Use find to locate each occurrence of referenceText
            if (text.at(m_selectedCategory).find(referenceText, beginPos + referenceText.length()) != std::string::npos) {
              count++;
            }
            if (count < 2) {
              // If found, replace the substring with "this is a test"
              if (headerText.back() != '\n') {
                headerText += '\n';
              }
              text.at(m_selectedCategory).replace(beginPos, referenceText.length(), "");
              text.at(m_selectedCategory).insert(beginPos, headerText);
              m_docs->docs.front().setTexts(text);
              Save();
              createHeaders();
            }
            m_isEdittingHeader = false;
          }
        } else {

          if (m_isEdittingText) {
            m_docs->docs.front().setTexts(m_texts);
            m_isEdittingText = false;
          }
          if (m_isEdittingNames) {
            m_docs->docs.front().setNames(m_names);
            m_isEdittingNames = false;
          }
          Save();
          createHeaders();
        }
      }
      ImGui::EndDisabled();
      ImGui::SameLine();
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
      ImGui::TextDisabled("(?)");
      if (ImGui::BeginItemTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(GetFormattingHelpText().c_str());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
      }
      ImGui::EndGroup();
    }
    // Editor
    ImGui::BeginChild("##orpg_docs_editor_text", ImVec2(0, 0), 0, ImGuiWindowFlags_NoScrollbar);
    {
      if (m_isEdittingHeader) {
        std::string text = headerText;
        // headerTexts.at(m_selectedHeader);
        if (ImGui::InputTextMultiline("##orpg_docs_text", &text, ImVec2{ImGui::GetContentRegionMax().x, ImGui::GetContentRegionMax().y - 30})) {
          headerText = text;
          //  m_isEdittingTexts = true;
        }
      } else if (m_isEdittingText) {
        // Text Editor Mode
        std::string text = m_texts.at(m_selectedCategory);
        if (ImGui::InputTextMultiline("##orpg_docs_text", &text, ImVec2{ImGui::GetContentRegionMax().x, ImGui::GetContentRegionMax().y - 30})) {
          m_texts.at(m_selectedCategory) = text;
          // m_isEdittingTexts = true;
        }
      } else {
        // Text Viewing Mode

        if (m_docs->docs.at(0).texts().empty()) {
          ImGui::TextUnformatted(trNOOP("\tNo documentation available."));
        } else {
          // Documentation available

          int headerIndex{0};
          float xPos = ImGui::GetCursorPosX();

          for (auto& text : splitString(m_docs->docs.at(0).texts().at(m_selectedCategory), '\n')) {
            if (text.contains("##") && inHeader == false) {
              inHeader = true;
            } else if (text.contains("##") && inHeader) {
              inHeader = false;
              // TODO: Header Status
              if (ImGui::CollapsingHeader(std::format("{}##header{}", headerTexts.at(headerIndex).front(), m_selectedCategory).c_str())) {
                for (auto it = headerTexts.at(headerIndex).begin() + (headerTexts.at(headerIndex).size() == 1 ? 0 : 1); it != headerTexts.at(headerIndex).end(); ++it) {
                  std::string itText = *it;
                  ImGui::SetCursorPosX(xPos + 20);
                  if (isSeperatorFormatting(itText)) {
                    // Seperators have no extra formatting
                    if (itText.size() > 1) {
                      // We use text
                      ImGui::Text("");
                      ImGui::SeparatorText(itText.substr(1, it->size() - 1).c_str());
                      ImGui::Text("");
                    } else {
                      // Just seperator
                      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 3);
                    }
                  } else {
                    // Process through formatting
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
                    while (ProcessFormatting(itText)) {}
                    if (isArrowBullet) {
                      itText = "→ " + itText;
                    }
                    if (isFontFormat) {
                      ImFont font = *App::APP->getMainFont();
                      font.Scale = font.Scale + m_formatScale;

                      ImGui::PushFont(&font);

                      if (isPictureFormat) {
                        // Show Picture
                        Texture texture = ResourceManager::instance()->loadEditorTexture(itText);
                        ImGui::Image(texture, ImVec2{static_cast<float>(texture.width()), static_cast<float>(texture.height())}, ImVec2{0, 0}, ImVec2{1, 1});
                      } else {
                        if (isBulletFormat) {
                          ImGui::BulletText("%s", itText.c_str());
                        } else {
                          ImGui::Text("%s", itText.c_str());
                        }
                      }
                      ImGui::PopFont();
                    } else {
                      if (isPictureFormat) {
                        // Show Picture
                        Texture texture = ResourceManager::instance()->loadEditorTexture(itText);
                        ImGui::Image(texture, ImVec2{static_cast<float>(texture.width()), static_cast<float>(texture.height())}, ImVec2{0, 0}, ImVec2{1, 1});

                      } else {
                        if (isBulletFormat) {
                          ImGui::BulletText("%s", itText.c_str());
                        } else {
                          ImGui::Text("%s", itText.c_str());
                        }
                      }
                    }

                    // Reset bools per line
                    isFontFormat = false;
                    isArrowBullet = false;
                    isIdenting = false;
                    isBulletFormat = false;
                    isPictureFormat = false;
                  }
                }
              }
              headerIndex++;
            } else {
              if (!inHeader) {
                if (isSeperatorFormatting(text)) {
                  // Seperators don't get any extra formatting
                  if (text.size() > 1) {
                    // We use text
                    ImGui::SeparatorText(text.substr(1, text.size() - 1).c_str());
                  } else {
                    // Just seperator
                    ImGui::Text("");
                    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 3);
                    ImGui::Text("");
                  }
                } else {
                  // Basic text formatting branching
                  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
                  while (ProcessFormatting(text)) {}
                  if (isArrowBullet) {
                    text = "→ " + text;
                  }
                  if (isFontFormat) {
                    ImFont font = *App::APP->getMainFont();
                    font.Scale = font.Scale + m_formatScale;

                    ImGui::PushFont(&font);

                    if (isPictureFormat) {
                      // Show Picture
                      Texture texture = ResourceManager::instance()->loadEditorTexture(text);
                      ImGui::Image(texture, ImVec2{static_cast<float>(texture.width()), static_cast<float>(texture.height())}, ImVec2{0, 0}, ImVec2{1, 1});
                    } else {
                      if (isBulletFormat) {
                        ImGui::BulletText("%s", text.c_str());
                      } else {
                        ImGui::Text("%s", text.c_str());
                      }
                    }
                    ImGui::PopFont();
                  } else {
                    if (isPictureFormat) {
                      // Show Picture
                      Texture texture = ResourceManager::instance()->loadEditorTexture(text);
                      ImGui::Image(texture, ImVec2{static_cast<float>(texture.width()), static_cast<float>(texture.height())}, ImVec2{0, 0}, ImVec2{1, 1});

                    } else {
                      if (isBulletFormat) {
                        ImGui::BulletText("%s", text.c_str());
                      } else {
                        ImGui::Text("%s", text.c_str());
                      }
                    }
                  }

                  // Reset bools per line
                  isFontFormat = false;
                  isArrowBullet = false;
                  isIdenting = false;
                  isBulletFormat = false;
                  isPictureFormat = false;
                }
              }
            }
          }
        }
      }
      ImGui::EndChild();
    }
  }
  ImGui::EndChild();
}
void DBDocTab::Save() const {

  fs::path editorPath = fs::path(Database::instance()->basePath) / "editor";

  if (!exists(editorPath)) {
    create_directories(editorPath);
  }

  if (m_docs->serialize(Database::instance()->basePath + "editor/Documentation.json")) {
    ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Success, "Serialized editor/Documentation.json successfully!"});
  } else {
    ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Error, "Failed to serialize editor/Documentation.json!"});
  }
}
// ##Example Text 1
// Example Text in Header 2231
/// ##
void DBDocTab::createHeaders() {
  headerTexts.clear();
  referenceHeaderTexts.clear();
  if (m_names.empty()) {
    return;
  }
  std::vector<std::string> texts = splitString(m_docs->docs.front().texts().at(m_selectedCategory), '\n');
  bool isHeaderText{false};
  int headerIndex{0};
  for (auto& text : texts) {
    if (text.contains("/##")) {
      isHeaderText = false;
      headerIndex++;
    } else if (text.contains("##")) {
      isHeaderText = true;
      headerTexts.push_back(std::vector<std::string>{});
      headerTexts.at(headerIndex).push_back(text.substr(2, text.size() - 1).data());

      referenceHeaderTexts.push_back(std::vector<std::string>{});
      referenceHeaderTexts.at(headerIndex).push_back(text.substr(2, text.size() - 1).data());
    }
    if (isHeaderText) {
      if (!text.contains("##")) {
        headerTexts.at(headerIndex).push_back(text);
        referenceHeaderTexts.at(headerIndex).push_back(text);
      }
    }
  }
}
void DBDocTab::setDoc(int index) {
  m_selectedCategory = index;
  createHeaders();
}
std::string DBDocTab::GetFormattingHelpText() {
  return trNOOP("Headers: \n##\n<text>\n/##\nBullets: -text\nSeperators: ~ or ~text\nArrow Bullets: >text\nIndent: \\t<text>\nPicture: \\p<name>\nFont Scale: \\f0.5<text>");
}

bool DBDocTab::ProcessFormatting(std::string& text) {
  bool containsFormatting{false};
  if (isArrowBulletFormatting(text)) {
    isArrowBullet = true;
    containsFormatting = true;
    text = text.substr(1, text.size() - 1);
  }
  if (isIndentFormatting(text)) {
    isIdenting = true;
    containsFormatting = true;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
    text = text.substr(2, text.size() - 1);
  }
  if (isFontFormatting(text)) {
    isFontFormat = true;
    containsFormatting = true;
    text = std::string(text.substr(2, text.size() - 1));

    std::string fontScale = text.substr(0, 3);

    m_formatScale = std::stof(fontScale);

    text = std::string(text.substr(3, text.size() - 1));
  }
  if (isBulletFormatting(text)) {
    isBulletFormat = true;
    containsFormatting = true;
    text = text.substr(1, text.size() - 1).data();
  }
  if (isPictureFormatting(text)) {
    isPictureFormat = true;
    containsFormatting = true;
    text = std::string(text.substr(2, text.size() - 1)) + ".png";
  }
  return containsFormatting;
}
bool DBDocTab::isNumericOnly(const std::string& str) {
  // Check if all characters in the string are alphabetic
  return std::all_of(str.begin(), str.end(), [](char c) {
    return std::isdigit(c); // Returns true if c is an alphabetic character
  });
}