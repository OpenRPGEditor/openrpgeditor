#pragma once

#include "Editor/Graphics/Texture.hpp"
#include "OREMath/Point.hpp"

struct Face {
  PointF min;
  PointF max;
};

struct FaceSheet {
  explicit FaceSheet(const std::string_view sheetName);

  Face getFaceRect(int index) const;
  [[nodiscard]] const Texture& texture() const { return m_texture; }

  static constexpr int faceWidth() { return 144; }
  static constexpr int faceHeight() { return 144; }

private:
  Texture m_texture;
};
