#include "Core/Graphics/FaceSheet.hpp"
#include "Core/ResourceManager.hpp"

FaceSheet::FaceSheet(const std::string_view sheetName)
: m_texture(ResourceManager::instance()->loadFaceImage(sheetName)) {}

Face FaceSheet::getFaceRect(const int index) const {
  const int fx = index % 4;
  const int fy = index / 4;

  Face face;
  face.u0 = fx * faceWidth();
  face.v0 = fy * faceHeight();
  face.u1 = (fx * faceWidth()) + faceWidth();
  face.v1 = (fy * faceHeight()) + faceHeight();
  face.u0 /= m_texture.width();
  face.v0 /= m_texture.height();
  face.u1 /= m_texture.width();
  face.v1 /= m_texture.height();

  return face;
}
