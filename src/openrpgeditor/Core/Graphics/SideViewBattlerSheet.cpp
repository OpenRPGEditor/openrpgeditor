#include "Core/Graphics/SideViewBattlerSheet.hpp"
#include "Core/ResourceManager.hpp"

struct SVMotion {
  int index;
  bool loop;
};

SideViewBattlerSheet::SideViewBattlerSheet(const std::string_view sheetName)
: m_texture(ResourceManager::instance()->loadSVActorImage(sheetName)) {}

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
    action.frames[pattern].u0 = (cx * cw);
    action.frames[pattern].v0 = (cy * ch);
    action.frames[pattern].u1 = ((cx * cw) + cw);
    action.frames[pattern].v1 = ((cy * ch) + ch);
    action.frames[pattern].u0 /= m_texture.width();
    action.frames[pattern].v0 /= m_texture.height();
    action.frames[pattern].u1 /= m_texture.width();
    action.frames[pattern].v1 /= m_texture.height();
  }

  return action;
}
