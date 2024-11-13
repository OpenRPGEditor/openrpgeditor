#include "Core/EventCommands/Dialog_SaveBGM.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_SaveBGM::draw() {
  return std::make_tuple(!m_open, m_confirmed);
}
