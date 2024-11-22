#pragma once

#include "Core/Graphics/ColorTonePreview.hpp"

class ColorFlashPicker {
public:
  ColorFlashPicker() = default;

  void setValues(const int r, const int g, const int b, const int intensity) {
    m_r = r;
    m_g = g;
    m_b = b;
    m_intensity = intensity;
    m_modified = false;
  }

  /**!
   * Sets the base size of the preview image before,
   * the values are scaled based on UI scale and applied automatically
   * @param width Unscaled width of the preview image
   * @param height Unscaled height of the preview image
   */
  void setPreviewSize(int width, int height);

  [[nodiscard]] int r() const { return m_r; }
  [[nodiscard]] int g() const { return m_g; }
  [[nodiscard]] int b() const { return m_b; }
  [[nodiscard]] int intensity() const { return m_intensity; }

  bool draw();

private:
  ColorTonePreview m_tonePreview;
  int m_r = 255;
  int m_g = 255;
  int m_b = 255;
  int m_intensity = 170;
  bool m_toneInvalid{true};
  bool m_modified{false};
};
