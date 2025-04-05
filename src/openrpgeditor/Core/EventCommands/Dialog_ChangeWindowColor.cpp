#include "Core/EventCommands/Dialog_ChangeWindowColor.hpp"

#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ChangeWindowColor::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{402, 150}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::Dummy(ImVec2(0, 5));
    ImGui::BeginGroup();
    {
      ImGui::Text("Red:");
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 8.f);
      ImGui::Text("Green:");
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 8.f);
      ImGui::Text("Blue:");
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::SetNextItemWidth(150);
      ImGui::SliderInt("##windowcolor_red", &red, -255, 255, "", ImGuiSliderFlags_NoInput);
      ImGui::SameLine();
      ImGui::SetNextItemWidth(100);
      ImGui::InputInt("##windowcolor_int_red", &red, 1, 100);

      ImGui::SetNextItemWidth(150);
      ImGui::SliderInt("##windowcolor_green", &green, -255, 255, "", ImGuiSliderFlags_NoInput);
      ImGui::SameLine();
      ImGui::SetNextItemWidth(100);
      ImGui::InputInt("##windowcolor_int_red", &green, 1, 100);

      ImGui::SetNextItemWidth(150);
      ImGui::SliderInt("##windowcolor_blue", &blue, -255, 255, "", ImGuiSliderFlags_NoInput);
      ImGui::SameLine();
      ImGui::SetNextItemWidth(100);
      ImGui::InputInt("##windowcolor_int_red", &blue, 1, 100);
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::ColorButton("##windowcolor_square", ImVec4{static_cast<float>(red * (1.0f / 255.0f)), static_cast<float>(green * (1.0f / 255.0f)), static_cast<float>(blue * (1.0f / 255.0f)), 1}, 0,
                       ImVec2{100, 100});

    ImGui::SetCursorPosY(ImGui::GetCursorPos().y - 10.f);
    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->r = red;
      command->g = green;
      command->b = blue;
      ImGui::CloseCurrentPopup();
      setOpen(false);
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
