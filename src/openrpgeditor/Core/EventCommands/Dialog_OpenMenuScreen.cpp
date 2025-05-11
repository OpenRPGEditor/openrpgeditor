#include "Core/EventCommands/Dialog_OpenMenuScreen.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_OpenMenuScreen::draw() {
  if (isOpen()) {
    setOpen(false);
    m_confirmed = true;
  }
  return std::make_tuple(!m_open, m_confirmed);
}
