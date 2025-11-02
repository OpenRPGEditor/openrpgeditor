#include "Editor/Graphics/FaceSheet.hpp"
#include "Editor/ResourceManager.hpp"

FaceSheet::FaceSheet(const std::string_view sheetName)
: m_texture(ResourceManager::instance()->loadFaceImage(sheetName)) {}

Face FaceSheet::getFaceRect(const int index) const {
  const int fx = index % 4;
  const int fy = index / 4;

  Face face;
  face.min.xr() = fx * faceWidth();
  face.min.yr() = fy * faceHeight();
  face.max.xr() = (fx * faceWidth()) + faceWidth();
  face.max.yr() = (fy * faceHeight()) + faceHeight();
  face.min.xr() /= m_texture.width();
  face.min.yr() /= m_texture.height();
  face.max.xr() /= m_texture.width();
  face.max.yr() /= m_texture.height();

  return face;
}
