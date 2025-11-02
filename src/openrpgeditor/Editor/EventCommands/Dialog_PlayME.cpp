#include "Editor/EventCommands/Dialog_PlayME.hpp"

std::tuple<bool, bool> Dialog_PlayME::draw() {
  if (const auto [closed, confirmed] = m_audioRenderer.value().draw(m_name, "PlayME"); closed) {
    m_confirmed = confirmed;
    setOpen(false);
  }

  return std::make_tuple(!m_open, m_confirmed);
}
