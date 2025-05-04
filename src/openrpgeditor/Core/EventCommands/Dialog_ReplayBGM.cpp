#include "Core/EventCommands/Dialog_ReplayBGM.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_ReplayBGM::draw() {
  if (m_open) {
    m_open = false;
    m_confirmed = true;
  }
  return std::make_tuple(!m_open, m_confirmed);
}
