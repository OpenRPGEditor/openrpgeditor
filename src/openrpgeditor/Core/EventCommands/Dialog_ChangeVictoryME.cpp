#include "Core/EventCommands/Dialog_ChangeVictoryME.hpp"

std::tuple<bool, bool> Dialog_ChangeVictoryME::draw() {
  if (const auto [closed, confirmed] = m_audioRenderer.value().draw(m_name, "ChangeVictoryME"); closed) {
    m_confirmed = confirmed;
    setOpen(false);
  }

  return std::make_tuple(!m_open, m_confirmed);
}
