#pragma once

#include "Core/Graphics/ColorTonePreview.hpp"

/**
 * @name ColorTonePicker
 * Provides a common UI tool to manipulate color tone values
 */
class ColorTonePicker {
public:
  ColorTonePicker() = default;

  /**
   *
   * @param r Red channel shift value, [-255,255] range
   * @param g Green channel shift value, [-255,255] range
   * @param b Blue channel shift value, [-255,255] range
   * @param gray Grayscale intensity, [0,255] range
   */
  void setValues(const int r, const int g, const int b, const int gray) {
    m_r = r;
    m_g = g;
    m_b = b;
    m_gray = gray;
    m_modified = false;
    m_toneInvalid = true;
  }

  /**
   * Sets the base size of the preview image.
   * The values are scaled based on UI scale and applied automatically
   * @param width Unscaled width of the preview image
   * @param height Unscaled height of the preview image
   */
  void setPreviewSize(int width, int height);
  [[nodiscard]] int r() const { return m_r; }
  [[nodiscard]] int g() const { return m_g; }
  [[nodiscard]] int b() const { return m_b; }
  [[nodiscard]] int gray() const { return m_gray; }

  /**
   * Draws the ColorTonePicker elements.
   * @return True if any of the values were modified, false otherwise
   */
  bool draw();

private:
  ColorTonePreview m_tonePreview;
  int m_r = 0;
  int m_g = 0;
  int m_b = 0;
  int m_gray = 0;
  bool m_toneInvalid{true};
  bool m_modified{false};
};
