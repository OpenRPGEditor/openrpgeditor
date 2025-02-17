
#include "Core/EventCommands/Dialog_CommonEvent.hpp"
#include "Core/Log.hpp"

template <>
inline int ObjectPicker<std::optional<CommonEvent>>::getId(const std::optional<CommonEvent>& value) {
  return value ? value->id() : 0;
}

static const std::string InvalidCommonEvent = "Invalid Common Event";
template <>
inline const std::string& ObjectPicker<std::optional<CommonEvent>>::getName(const std::optional<CommonEvent>& value) {
  return value ? value->name() : InvalidCommonEvent;
}

std::tuple<bool, bool> Dialog_CommonEvent::draw() {
  if (m_picker) {
    auto [closed, confirmed] = m_picker->draw();
    if (closed) {
      if (confirmed) {
        command->event = m_picker->selection();
      }
      m_picker.reset();
    }
  }
  return std::make_tuple(!m_open, m_confirmed);
}
