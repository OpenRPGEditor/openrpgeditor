#include "Dialog_MovementSwitchON.hpp"
#include "Core/Log.hpp"

std::tuple<bool, bool> Dialog_MovementSwitchON::draw() {

  if (picker) {
    auto [closed, confirmed]  = picker->draw();
    if (confirmed) {
      m_open = closed;
      m_confirmed = confirmed;
      command->id = picker->selection();
      picker.reset();
    }
  }
  return std::make_tuple(!m_open, m_confirmed);
}
