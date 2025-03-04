#include "Core/CommonUI/Preview.hpp"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <format>

#include <algorithm>

Preview::Preview(const std::string_view name, const std::vector<std::string>& values, const int initialSelection, int rangeStart) : IDialogController(name) {}

std::tuple<bool, bool> Preview::draw() {
  const std::string title = std::format("{} Preview##{}", m_name, reinterpret_cast<uintptr_t>(this));
  if (isOpen()) {
    ImGui::OpenPopup(title.c_str());
  }

  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size / 2, ImGuiCond_Once);
  if (ImGui::BeginPopupModal(title.c_str(), &m_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings)) {

    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        m_confirmed = true;
        m_open = false;
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        m_confirmed = false;
        m_open = false;
        ImGui::CloseCurrentPopup();
      }
    }
    ImGui::EndGroup();
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
