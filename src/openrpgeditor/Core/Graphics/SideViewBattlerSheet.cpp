#include "Core/Graphics/SideViewBattlerSheet.hpp"
#include "Core/ResourceManager.hpp"

struct SVMotion {
  int index;
  bool loop;
};

SideViewBattlerSheet::SideViewBattlerSheet(const std::string_view sheetName) : m_texture(ResourceManager::instance()->loadSVActorImage(sheetName)) {}

SideViewAction SideViewBattlerSheet::getAction(const SideViewActionType actionType) const {
  if (!m_texture) {
    return {};
  }

  SideViewAction action;
  action.type = actionType;
  const int motionIndex = static_cast<int>(actionType);
  const float cw = m_texture.width() / 9;
  const float ch = m_texture.height() / 6;
  for (int pattern = 0; pattern < 3; ++pattern) {
    const float cx = std::floor(motionIndex % 3) + pattern;
    const float cy = std::floor(motionIndex / 3);
    action.frames[pattern].min.xr() = (cx * cw);
    action.frames[pattern].min.yr() = (cy * ch);
    action.frames[pattern].max.xr() = ((cx * cw) + cw);
    action.frames[pattern].max.yr() = ((cy * ch) + ch);
    action.frames[pattern].min.xr() /= m_texture.width();
    action.frames[pattern].min.yr() /= m_texture.height();
    action.frames[pattern].max.xr() /= m_texture.width();
    action.frames[pattern].max.yr() /= m_texture.height();
  }

  return action;
}
