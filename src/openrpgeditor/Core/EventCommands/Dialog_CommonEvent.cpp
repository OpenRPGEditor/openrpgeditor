
#include "Dialog_CommonEvent.hpp"
#include "imgui.h"
#include "Core/Log.hpp"
#include "Core/Project.hpp"

std::tuple<bool, bool> Dialog_CommonEvent::draw() {
  if (ce_picker) {
    auto [closed, confirmed] = ce_picker->draw();
    if (confirmed) {
      d_common_id = ce_picker->selection();
      ce_picker.reset();
    }
  }
  return std::make_tuple(!m_open, m_confirmed);
}
