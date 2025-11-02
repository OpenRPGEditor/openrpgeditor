#include "Editor/EventCommands/Dialog_ChangeBattleBGM.hpp"

std::tuple<bool, bool> Dialog_ChangeBattleBGM::draw() {
  if (const auto [closed, confirmed] = m_audioRenderer.value().draw(m_name, "ChangeBattleBGM"); closed) {
    m_confirmed = confirmed;
    setOpen(false);
  }

  return std::make_tuple(!m_open, m_confirmed);
}
