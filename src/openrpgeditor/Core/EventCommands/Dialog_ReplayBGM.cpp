#include "Core/EventCommands/Dialog_ReplayBGM.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_ReplayBGM::draw() {
  return std::make_tuple(!m_open, m_confirmed);
}
