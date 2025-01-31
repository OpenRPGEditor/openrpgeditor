#include "Core/DatabaseEditor/DBDocTab.hpp"
#include "Core/DatabaseEditor.hpp"
#include "Core/ImGuiExt/ImGuiNotify.hpp"
#include "Core/MainWindow.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cstring>
DBDocTab::DBDocTab(Docs& docs, DatabaseEditor* parent) : IDBEditorTab(parent), m_docs(&docs) {

  if (m_docs->docs.size() == 0) {
    Doc doc;

    doc.setNames({"Example Name 1", "Example Name 2", "Example Name 3"});
    doc.setTexts({" Example Text 1", "Example Text 2", "Example Test 3"});

    m_docs->docs.emplace_back(doc);

    Save();
  }
  std::vector<std::string> texts = splitString(m_docs->docs.at(m_selectedCategory).texts().at(m_selectedCategory), '\n');

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
    }
    if (isHeaderText) {
      if (!text.contains("##")) {
        headerTexts.at(headerIndex).push_back(text);
      }
    }
  }
}

void DBDocTab::draw() {
  const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
  ImGui::BeginChild("##orpg_docs_navigation", ImVec2{calc.x + (ImGui::GetStyle().ItemSpacing.x * 2), 0}, 0, ImGuiWindowFlags_NoScrollbar);
  {
    ImGui::BeginGroup();
    {
      ImGui::BeginChild("##orpg_docs_navigation_headers");
      {
        ImGui::BeginGroup();
        {
          ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x);
          if (ImGui::BeginCombo("##orpg_navigation_combo", m_docs->docs.at(m_selectedCategory).names().at(m_selectedCategory).c_str())) {
            for (int i = 0; i < m_docs->docs.at(m_selectedCategory).names().size(); i++) {
              if (ImGui::Selectable(m_docs->docs.at(m_selectedCategory).names().at(i).c_str(), i == m_selectedCategory)) {
                m_selectedCategory = i;
                // SetScroll
              }
            }
            ImGui::EndCombo();
          }
          ImGui::SeparatorText("Headers");
          if (headerTexts.size() == 0) {
            ImGui::Text("No headers available");
          } else {
            int index{0};
            for (auto& header : headerTexts) {
              char name[4096];
              snprintf(name, 4096, "%s", header.front().c_str());
              if (ImGui::Selectable(name, index == m_selectedHeader) || (ImGui::IsItemFocused() && m_selectedHeader != index)) {
                m_selectedHeader = index;
                // SetScroll
              }
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
    if (m_isEdittingText) {
      // Text Editor Mode
      char text[4096];
      strcpy(text, m_docs->docs.at(m_selectedCategory).texts().at(m_selectedCategory).c_str()); // Copy the string to the array
      ImGui::Text(trNOOP("Editing documentation... Press enter to confirm."));
      if (ImGui::InputTextMultiline("##orpg_docs_text", text, IM_ARRAYSIZE(text), ImVec2{ImGui::GetContentRegionMax().x, ImGui::GetContentRegionMax().y - 30})) {
        // m_docs->setString(text);
        if (ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)) {
          Save();
          m_isEdittingText = false;
        }
      }
    } else {
      // Text Viewing Mode
      if (m_docs->docs.at(m_selectedCategory).texts().at(m_selectedCategory).empty()) {
        ImGui::Text(trNOOP("\tNo documentation available.\n\tDouble-click to begin editing..."));
      } else {
        // Documentation available

        int headerIndex{0};
        float xPos = ImGui::GetCursorPosX();

        for (auto& text : splitString(m_docs->docs.at(m_selectedCategory).texts().at(m_selectedCategory), '\n')) {
          if (text.contains("##") && inHeader == false) {
            inHeader = true;
          } else if (text.contains("##") && inHeader) {
            inHeader = false;

            if (ImGui::CollapsingHeader(headerTexts.at(headerIndex).front().c_str())) {
              for (auto it = headerTexts.at(headerIndex).begin(); it != headerTexts.at(headerIndex).end(); ++it) {
                ImGui::SetCursorPosX(xPos + 20);
                if (isBulletFormatting(it->c_str())) {
                  ImGui::BulletText(it->substr(1, it->size() - 1).data());
                } else {
                  ImGui::Text(it->c_str());
                }
              }
            }
            headerIndex++;
          } else {
            if (!inHeader) {
              if (isBulletFormatting(text.c_str())) {
                ImGui::BulletText(text.substr(1, text.size() - 1).data());
              } else {
                ImGui::Text(text.c_str());
              }
            }
          }
        }
      }
      if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
        // Enter Edit Mode
        m_isEdittingText = true;
      }
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
