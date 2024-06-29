
#include "Dialog_CommonEvent.hpp"
#include "imgui.h"
#include "Core/Log.hpp"
#include "Core/Project.hpp"

template<>
inline int ObjectPicker<std::optional<CommonEvent>>::getId(const std::optional<CommonEvent>& value) {
  return value->id;
}

template<>
inline const std::string& ObjectPicker<std::optional<CommonEvent>>::getName(const std::optional<CommonEvent>& value) {
  return value->name;
}

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
