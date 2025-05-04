#include "Core/EventCommands/Dialog_ChangeVehicleBGM.hpp"

std::tuple<bool, bool> Dialog_ChangeVehicleBGM::draw() {
  if (const auto [closed, confirmed] = m_audioRenderer.value().draw(m_name, "ChangeVehicleBGM"); closed) {
    m_confirmed = confirmed;
    setOpen(false);
  }

  return std::make_tuple(!m_open, m_confirmed);
}
