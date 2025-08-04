#include "Core/EventCommands/MovementRoute/Dialog_MovementSwitchOFF.hpp"

#include "Core/Log.hpp"
#include <imgui.h>

std::tuple<bool, bool> Dialog_MovementSwitchOFF::draw() {

  if (m_picker) {
    if (const auto [closed, confirmed] = m_picker->draw(); closed) {
      if (confirmed) {
        setOpen(false);
        ImGui::CloseCurrentPopup();
        m_confirmed = confirmed;
        m_command->id = m_picker->selection();
      }
      m_picker.reset();
    }
  }
  return std::make_tuple(!m_open, m_confirmed);
}
