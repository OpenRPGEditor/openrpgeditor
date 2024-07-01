#include "Dialog_OpenMenuScreen.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_OpenMenuScreen::draw() {
  return std::make_tuple(!m_open, m_confirmed);
}
