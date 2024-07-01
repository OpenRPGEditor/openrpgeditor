#include "Dialog_ExitEventProcessing.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_ExitEventProcessing::draw() {
  return std::make_tuple(!m_open, m_confirmed);
}
