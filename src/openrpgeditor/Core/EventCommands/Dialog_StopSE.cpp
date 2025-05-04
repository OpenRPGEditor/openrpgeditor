#include "Core/EventCommands/Dialog_StopSE.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_StopSE::draw() {
  if (m_open) {
    m_open = false;
    m_confirmed = true;
  }
  return std::make_tuple(!m_open, m_confirmed);
}
