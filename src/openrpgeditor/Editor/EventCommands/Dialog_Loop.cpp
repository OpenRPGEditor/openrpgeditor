#include "Editor/EventCommands/Dialog_Loop.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_Loop::draw() {
  if (isOpen()) {
    setOpen(false);
    m_confirmed = true;
  }
  return std::make_tuple(!m_open, m_confirmed);
}
