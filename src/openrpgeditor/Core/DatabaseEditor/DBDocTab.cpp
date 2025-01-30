#include "Core/DatabaseEditor/DBDocTab.hpp"
#include "Core/DatabaseEditor.hpp"
#include "Core/ImGuiExt/ImGuiNotify.hpp"
#include "Core/MainWindow.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cstring>

void DBDocTab::draw() {
  ImGui::BeginChild("##orpg_docs_editor");
  {
    if (m_isEdittingText) {
      // Text Editor Mode
      char text[4096];
      strcpy(text, m_docs->str().c_str()); // Copy the string to the array
      ImGui::Text(trNOOP("Editing documentation... Press enter to confirm."));
      if (ImGui::InputTextMultiline("##orpg_docs_text", text, IM_ARRAYSIZE(text), ImVec2{ImGui::GetContentRegionMax().x, ImGui::GetContentRegionMax().y - 30})) {
        m_docs->setString(text);
        if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)) {
          m_isEdittingText = false;
        }
      }
    } else {
      // Text Viewing Mode
      if (m_docs->str().empty()) {
        ImGui::Text(trNOOP("\tNo documentation available.\n\tDouble-click to begin editing..."));
      } else {
        // Documentation available
      }
      if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
        // Enter Edit Mode
        m_isEdittingText = true;
      }
    }
  }
  ImGui::EndChild();
}
