#include "Editor/EventCommands/Dialog_FadeinScreen.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_FadeinScreen::draw() {  if (isOpen()) {
  setOpen(false);
  m_confirmed = true;
} return std::make_tuple(!m_open, m_confirmed); }
