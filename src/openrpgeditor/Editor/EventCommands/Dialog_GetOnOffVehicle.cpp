#include "Editor/EventCommands/Dialog_GetOnOffVehicle.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_GetOnOffVehicle::draw() {
  if (isOpen()) {
    setOpen(false);
    m_confirmed = true;
  }
  return std::make_tuple(!m_open, m_confirmed);
}
