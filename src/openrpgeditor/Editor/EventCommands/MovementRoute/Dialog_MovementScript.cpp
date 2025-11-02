#include "Editor/EventCommands/MovementRoute/Dialog_MovementScript.hpp"
#include "Editor/Application.hpp"

#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "Editor/Log.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <tuple>

std::tuple<bool, bool> Dialog_MovementScript::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
    // SetOpen(false);
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{ImGui::GetDPIScaledValue(1400), 600}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open)) {

    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
    ImGui::PushFont(App::APP->getMonoFont());
    m_textEditor.Render("##no_title", ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, ImGui::GetContentRegionAvail().y - 32 - ImGui::GetStyle().FramePadding.y});
    ImGui::PopFont();
    if (ImGui::Button("OK")) {
      auto script = m_textEditor.GetText();
      std::vector<std::string> scripts = splitString(script, '\n');

      command->script = scripts.front();
      ImGui::CloseCurrentPopup();
      setOpen(false);
      m_confirmed = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }

    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
