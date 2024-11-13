#include "Core/EventCommands/Dialog_FadeoutScreen.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_FadeoutScreen::draw() {
  return std::make_tuple(!m_open, m_confirmed);
}
