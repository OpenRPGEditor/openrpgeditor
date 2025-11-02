#pragma once
#include "Editor/Graphics/GameWindowBackground.hpp"

/**
 * @name GameWindowColorPicker
 * Provides a common UI tool to manipulate game window color values
 */
class GameWindowColorPicker {
public:
  /**
   * Sets the shift color channel values affecting the window color
   * @param r Red channel shift value, [-255,255] range
   * @param g Green channel shift value, [-255,255] range
   * @param b Blue channel shift value, [-255,255] range
   */
  void setValues(const int r, const int g, const int b) {
    m_r = r;
    m_g = g;
    m_b = b;
    m_modified = false;
    m_backgroundInvalid = true;
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

  /**
   * Draws the GameWindowColorPicker elements.
   * @return True if any of the values were modified, false otherwise
   */
  void draw(ImVec2 size);

private:
  GameWindowBackground m_background;
  int m_r{0};
  int m_g{0};
  int m_b{0};
  int m_modified{false};
  int m_backgroundInvalid{true};
};
