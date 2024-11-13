#include "Core/EventCommands/Dialog_GetOnOffVehicle.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_GetOnOffVehicle::draw() {
  return std::make_tuple(!m_open, m_confirmed);
}
