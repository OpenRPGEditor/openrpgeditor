#include "Editor/EventCommands/Dialog_ChangeDefeatME.hpp"

std::tuple<bool, bool> Dialog_ChangeDefeatME::draw() {
  if (const auto [closed, confirmed] = m_audioRenderer.value().draw(m_name, "ChangeDefeatME"); closed) {
    m_confirmed = confirmed;
    setOpen(false);
  }

  return std::make_tuple(!m_open, m_confirmed);
}
