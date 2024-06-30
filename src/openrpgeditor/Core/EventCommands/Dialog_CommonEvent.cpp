
#include "Dialog_CommonEvent.hpp"
#include "imgui.h"
#include "Core/Log.hpp"
#include "Core/Project.hpp"

template <>
inline int ObjectPicker<std::optional<CommonEvent>>::getId(const std::optional<CommonEvent>& value) {
  return value ? value->id : 0;
}

static const std::string InvalidCommonEvent = "Invalid Common Event";
template <>
inline const std::string& ObjectPicker<std::optional<CommonEvent>>::getName(const std::optional<CommonEvent>& value) {
  return value ? value->name : InvalidCommonEvent;
}

std::tuple<bool, bool> Dialog_CommonEvent::draw() {
  if (ce_picker) {
    auto [closed, confirmed] = ce_picker->draw();
    if (confirmed) {
      m_open = closed;
      m_confirmed = confirmed;
      command->event = ce_picker->selection();
      ce_picker.reset();
    }
  }
  return std::make_tuple(!m_open, m_confirmed);
}
