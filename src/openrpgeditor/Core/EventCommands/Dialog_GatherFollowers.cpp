#include "Core/EventCommands/Dialog_GatherFollowers.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_GatherFollowers::draw() {
  if (isOpen()) {
    setOpen(false);
    m_confirmed = true;
  }
  return std::make_tuple(!m_open, m_confirmed);
}
