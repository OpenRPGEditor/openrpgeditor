#include "Core/EventCommands/Dialog_PlayBGM.hpp"

#include "Core/ImGuiExt/imgui-knobs.h"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Log.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include <IconsFontAwesome6.h>
using namespace std::string_view_literals;
std::tuple<bool, bool> Dialog_PlayBGM::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize)) {
    if (m_audioRenderer.value().draw()) {
      m_confirmed = true;
      setOpen(false);
    }
    ImGui::EndPopup();
  }
  return std::make_tuple(!m_open, m_confirmed);
}
