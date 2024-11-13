#include "Core/EventCommands/Dialog_AbortBattle.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_AbortBattle::draw() {
  return std::make_tuple(!m_open, m_confirmed);
}
