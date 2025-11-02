#include "Editor/EventCommands/MovementRoute/Dialog_MovementSwitchON.hpp"

#include "Editor/Log.hpp"
#include <imgui.h>

std::tuple<bool, bool> Dialog_MovementSwitchON::draw() {

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
