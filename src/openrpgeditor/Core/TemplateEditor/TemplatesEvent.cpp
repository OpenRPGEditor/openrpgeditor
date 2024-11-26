#include "Core/TemplateEditor/TemplatesEvent.hpp"
#include "Core/Application.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Templates.hpp"

#include "imgui.h"


void TemplatesEvent::draw() {
  ImGui::SetNextWindowSize(ImVec2{720, 640} * App::DPIHandler::get_ui_scale(), ImGuiCond_Once);
  if (ImGui::Begin("Template Properties", &m_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::Text("Name:");
    ImGui::InputText("##orpg_templates_event_properties_name", &m_eventName);ImGui::BeginChild("ArrowButtons", {70, 70});
    ImGui::Columns(3, nullptr, false);
    //ImGui::PushButtonRepeat(true);
    for (int i = 0; i < 9; i++)
    {
      switch (i)
      {
      case 0:
        //if (ImGui::ArrowButton("##UpLeft", ImGuiDir_UpLeft)){}
        break;
      case 1:
        if (ImGui::ArrowButton("##Up", ImGuiDir_Up)){}
        break;
      case 2:
        //if (ImGui::ArrowButton("##UpRight", ImGuiDir_UpRight)){}
        break;
      case 3:
        if (ImGui::ArrowButton("##Left", ImGuiDir_Left)){}
        break;
      case 4:
        if (ImGui::Button("C")){}
        break;
      case 5:
        if (ImGui::ArrowButton("##Right", ImGuiDir_Right)){}
        break;
      case 6:
        //if (ImGui::ArrowButton("##DownLeft", ImGuiDir_DownLeft)){}
        break;
      case 7:
        if (ImGui::ArrowButton("##Down", ImGuiDir_Down)){}
        break;
      case 8:
        //if (ImGui::ArrowButton("##DownRight", ImGuiDir_DownRight)){}
        break;
      }
      ImGui::NextColumn();
    }
    //ImGui::PopButtonRepeat();
    ImGui::EndChild();
  }
  ImGui::End();
};