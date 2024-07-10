#pragma once

#include "Core/Texture.hpp"
struct Face {
  float u0;
  float v0;
  float u1;
  float v1;
};
struct FaceSheet {
  explicit FaceSheet(const std::string_view sheetName);

  Face getFaceRect(int index);
  [[nodiscard]] const Texture& texture() const { return m_texture; }

  static constexpr int faceWidth() { return 144; }
  static constexpr int faceHeight() { return 144; }

private:
  Texture m_texture;
};
