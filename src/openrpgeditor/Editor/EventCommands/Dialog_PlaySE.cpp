#include "Editor/EventCommands/Dialog_PlaySE.hpp"

std::tuple<bool, bool> Dialog_PlaySE::draw() {
  if (const auto [closed, confirmed] = m_audioRenderer.value().draw(m_name, "PlaySE"); closed) {
    m_confirmed = confirmed;
    setOpen(false);
  }
  return std::make_tuple(!m_open, m_confirmed);
}
