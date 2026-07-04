#include "Editor/Translator/TranslatorDialog.hpp"

#include "Database/Database.hpp"
#include "Editor/CommonUI/GroupBox.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "orei18n.hpp"
#include "Translator.hpp"

#include <imgui.h>
#include <imgui_internal.h>

bool TranslatorDialog::closeAllDocuments(const bool saveDocuments) {
  bool ret = true;
  if (saveDocuments) {
    for (const auto& document : Translator::instance().documents()) {
      if (!document->save()) {
        ret = false;
      }
    }
  }
  
  m_currentDocument = nullptr;
  
  return ret;
}

std::tuple<bool, bool> TranslatorDialog::draw() {
  if (!m_open) {
    return {true, m_confirmed};
  }

  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, {0.5f, 0.5f});
  ImGui::SetNextWindowSizeConstraints(ImGui::GetDPIScaledSize(512, 512), {FLT_MAX, FLT_MAX});

  if (ImGui::Begin(std::format("{}###{}", trNOOP("Translator"), m_dialogId).c_str(), &m_open)) {
    const bool isDocked = ImGui::IsWindowDocked();
    ImGui::BeginVertical("##translator_main_layout", ImGui::GetContentRegionAvail(), 0.f);
    {
      GroupBox documentsComboGroup(trNOOP("Documents"), "##translator_documents_group", {-1, 0});

      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##translator_button_layout", {-1, 0}, 0.f);
      {
        ImGui::Spring();

        if (const auto ret = ImGui::ButtonGroup("##translator_dialog_button_layout", {trNOOP("OK"), trNOOP("Apply"), !isDocked ? trNOOP("Cancel") : trNOOP("Revert")}, {}, {},
                                                {false, m_currentDocument && m_currentDocument->isModified(), isDocked ? m_currentDocument && m_currentDocument->isModified() : false}, {!isDocked});
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
            m_currentDocument->restore();
          }
          
          if (isDocked) {
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
