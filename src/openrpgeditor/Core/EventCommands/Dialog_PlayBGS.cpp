#include "Core/EventCommands/Dialog_PlayBGS.hpp"

std::tuple<bool, bool> Dialog_PlayBGS::draw() {
  if (const auto [closed, confirmed] = m_audioRenderer.value().draw(m_name, "PlayBGS"); closed) {
    m_confirmed = confirmed;
    setOpen(false);
  }

  return std::make_tuple(!m_open, m_confirmed);
}
