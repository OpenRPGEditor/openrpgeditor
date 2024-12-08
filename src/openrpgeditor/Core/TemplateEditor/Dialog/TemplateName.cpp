#include "TemplateName.hpp"
#include "Core/Application.hpp"
#include "Database/Templates.hpp"

#include "imgui.h"

void TemplateName::draw() {
  ImGui::SetNextWindowSize(ImVec2{500, 130} * App::DPIHandler::get_ui_scale(), ImGuiCond_Once);
  if (ImGui::Begin("Template Properties", &m_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize)) {
    ImGui::Text("Template Name:");
    ImGui::SetNextItemWidth(400 * App::DPIHandler::get_ui_scale());
    ImGui::InputText("##orpg_templates_rename", &m_templateName);

    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        m_template->setName(m_templateName);
        m_hasChanges = true;
        ImGui::CloseCurrentPopup();
      }
    }
    ImGui::EndGroup();
  }
  ImGui::End();
};