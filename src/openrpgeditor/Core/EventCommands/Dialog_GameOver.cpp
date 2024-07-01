#include "Dialog_GameOver.hpp"

#include "Core/Log.hpp"

#include <tuple>

std::tuple<bool, bool> Dialog_GameOver::draw() {
  return std::make_tuple(!m_open, m_confirmed);
}
