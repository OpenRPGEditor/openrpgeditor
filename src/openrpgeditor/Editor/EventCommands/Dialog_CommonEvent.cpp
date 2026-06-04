
#include "Editor/EventCommands/Dialog_CommonEvent.hpp"
#include "Editor/Log.hpp"

std::tuple<bool, bool> Dialog_CommonEvent::draw() {
  if (m_picker) {
    auto [closed, confirmed] = m_picker->draw();
    if (closed) {
      if (confirmed) {
        command->event = m_picker->selection();
        m_confirmed = true;
        setOpen(false);
        ImGui::CloseCurrentPopup();
      }
      m_picker.reset();
    }
  }
  return std::make_tuple(!m_open, m_confirmed);
}
