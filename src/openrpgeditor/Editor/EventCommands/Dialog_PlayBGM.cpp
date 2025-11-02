#include "Editor/EventCommands/Dialog_PlayBGM.hpp"

std::tuple<bool, bool> Dialog_PlayBGM::draw() {
  if (const auto [closed, confirmed] = m_audioRenderer.value().draw(m_name, "PlayBGM"); closed) {
    m_confirmed = confirmed;
    setOpen(false);
  }
  return std::make_tuple(!m_open, m_confirmed);
}
