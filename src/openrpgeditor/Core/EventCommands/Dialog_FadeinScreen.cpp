#include "Core/EventCommands/Dialog_FadeinScreen.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_FadeinScreen::draw() {
  return std::make_tuple(!m_open, m_confirmed);
}
