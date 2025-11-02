#include "Editor/EventCommands/Dialog_FadeoutBGS.hpp"

#include "Editor/CommonUI/GroupBox.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_FadeoutBGS::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###FadeoutBGS");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 5) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###FadeoutBGS", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginVertical("##fadeout_bgm_main_layout", ImGui::GetContentRegionAvail());
    {
      GroupBox durationGroup(trNOOP("Duration"), "##fadeout_bgm_duration_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (durationGroup.begin()) {
        const std::string durationText = trNOOP("seconds");
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - (ImGui::CalcItemSize(ImGui::CalcTextSize(durationText.c_str()), 0, 0).x + ImGui::GetStyle().FramePadding.x * 2));
        if (ImGui::SpinInt("##fadeout_bgm_input", &m_duration)) {
          m_duration = std::clamp(m_duration, 1, 999);
        }
        ImGui::SameLine();
        ImGui::TextUnformatted(durationText.c_str());
      }
      durationGroup.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##fadeout_bgm_button_group", {-1, -1});
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##fadeout_bgm_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->duration = m_duration;
          ImGui::CloseCurrentPopup();
          setOpen(false);
        } else if (ret == 1) {
          ImGui::CloseCurrentPopup();
          setOpen(false);
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
