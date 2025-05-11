#include "Core/EventCommands/Dialog_AbortBattle.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_AbortBattle::draw() {
  if (isOpen()) {
    setOpen(false);
    m_confirmed = true;
  }
  return std::make_tuple(!m_open, m_confirmed);
}
