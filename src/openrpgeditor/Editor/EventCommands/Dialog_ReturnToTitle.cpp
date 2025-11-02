#include "Editor/EventCommands/Dialog_ReturnToTitle.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_ReturnToTitle::draw() {
  if (isOpen()) {
    setOpen(false);
    m_confirmed = true;
  }
  return std::make_tuple(!m_open, m_confirmed);
}
