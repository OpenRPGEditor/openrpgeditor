#include "Core/Graphics/IconSheet.hpp"

#include "Core/ResourceManager.hpp"
#include "OREMath/Size.hpp"

IconSheet::IconSheet(const std::string_view sheetName)
: m_texture(ResourceManager::instance()->loadSystemImage(sheetName)) {}

SimpleRect IconSheet::rectForId(const int id) const {
  float u0 = static_cast<float>(id % columnCount() * iconWidth());
  float v0 = floorf(static_cast<float>(id / columnCount())) * iconHeight();
  float u1 = u0 + iconWidth();
  float v1 = v0 + iconHeight();
  u0 /= static_cast<float>(m_texture.width());
  v0 /= static_cast<float>(m_texture.height());
  u1 /= static_cast<float>(m_texture.width());
  v1 /= static_cast<float>(m_texture.height());
  return {PointF{u0, v0}, PointF{u1, v1}};
}
