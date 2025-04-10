#pragma once

#include "Core/Graphics/RenderImage.hpp"
#include "Core/Graphics/Texture.hpp"

#include "OREMath/Point.hpp"
#include "OREMath/Size.hpp"

/**
 * @class ParallaxTexture Core/Graphics/ParallaxTexture.hpp
 * @brief
 * <p>
 * ParallaxTexture describes a texture which can be rendered as with scroll and looping behavior to add more
 * visual flair to a scene.
 * </p>
 */
class ParallaxTexture {
public:
  /**
   * Constructs a ParallaxTexture with known size, scroll speed, and looping behavior
   * @param size The total size of the area to render the parallax map to
   * @param scroll The Speed at which the parallax moves, in tiles
   * @param loopX Whether to loop the texture along the X axis
   * @param loopY Whether to loop the texture along the Y axis
   */
  ParallaxTexture(const Size size, const Point& scroll, const bool loopX, const bool loopY)
  : m_size(size)
  , m_scroll(scroll)
  , m_loopX(loopX)
  , m_loopY(loopY) {}
  /**
   *
   * @param width Width of the area to render the parallax map to
   * @param height Height of the area to render the parallax map to
   * @param scrollX The Speed at which the parallax move along the X axis, in tiles
   * @param scrollY The Speed at which the parallax move along the Y axis, in tiles
   * @param loopX Whether to loop the texture along the X axis
   * @param loopY Whether to loop the texture along the Y axis
   */
  explicit ParallaxTexture(const int width = 0, const int height = 0, const int scrollX = 0, const int scrollY = 0, const bool loopX = false, const bool loopY = false)
  : ParallaxTexture(Size{width, height}, {scrollX, scrollY}, loopX, loopY) {}

  /**
   *
   * @param texture Texture object to render as a parallax
   * @param size The total size of the area to render the parallax map to
   * @param scroll The Speed at which the parallax moves, in tiles
   * @param loopX Whether to loop the texture along the X axis
   * @param loopY Whether to loop the texture along the Y axis
   */
  explicit ParallaxTexture(const Texture& texture, const Size& size = {}, const Point& scroll = {}, const bool loopX = false, const bool loopY = false)
  : ParallaxTexture(size, scroll, loopX, loopY) {
    m_texture = texture;
  }

  /**
   *
   * @param path Project relative path to the texture to render as a parallax
   * @param size The total size of the area to render the parallax map to, in pixels
   * @param scroll The Speed at which the parallax moves, in tiles
   * @param loopX Whether to loop the texture along the X axis
   * @param loopY Whether to loop the texture along the Y axis
   */
  explicit ParallaxTexture(const std::string_view path, const Size& size = {}, const Point& scroll = {}, const bool loopX = false, const bool loopY = false)
  : ParallaxTexture(size, scroll, loopX, loopY) {
    setTexture(path);
  }

  /**
   * The speed at which to scroll the texture
   * @return The current scroll speed
   */
  const Point& scroll() const { return m_scroll; }
  /**
   * Sets the scroll speed of the texture
   * @param scroll The speed at which to scroll the texture, as defined in tiles
   */
  void setScroll(const Point& scroll) {
    if (m_scroll == scroll) {
      return;
    }
    m_scroll = scroll;
    m_workDone = false;
  }
  /**
   * Sets the scroll speed of the texture
   * @param x The speed at which to scroll the texture along the X axis, as defined in tiles
   * @param y The speed at which to scroll the texture along the Y axis, as defined in tiles
   */
  void setScroll(const int x, const int y) { setScroll(Point{x, y}); }

  /**
   * The size of the rendered area, in pixels
   * @return The size of the rendered area, in pixels
   */
  const Size& size() const { return m_size; }

  int width() const { return m_size.width(); }
  int height() const { return m_size.height(); }

  int pixelWidth() const;
  int pixelHeight() const;

  /**
   * Sets the size of the area the texture is rendered to, in pixels
   * @param size The size
   */
  void setSize(const Size& size) {
    if (m_size == size) {
      return;
    }
    m_size = size;
    m_workDone = false;
  }
  void setSize(const int w, const int h) { setSize(Size{w, h}); }

  const Texture& texture() const { return m_texture; }
  void setTexture(const Texture& texture) { m_texture = texture; }
  void setTexture(const std::string_view path);

  bool loopX() const { return m_loopX; }
  void setLoopX(const bool loopX) {
    if (loopX) {
      return;
    }

    m_loopX = loopX;
    m_workDone = false;
  }

  bool loopY() const { return m_loopY; }
  void setLoopY(const bool loopY) {
    if (m_loopY == loopY) {
      return;
    }
    m_loopY = loopY;
    m_workDone = false;
  }

  void enableUpdates() {
    // We don't want to set the scroll offset here just in case the user set the setting two times in a row
    m_updateScrolling = true;
  }
  void disableUpdates() {
    m_updateScrolling = false;
    m_scrollPosition = {};
    m_workDone = false;
  }
  bool areUpdatesEnabled() const { return m_updateScrolling; }
  void setUpdatesEnabled(const bool updates) {
    // We don't simply set m_updateScrolling here and use a ternary to avoid setting m_scrollPosition unnecessarily
    // Also the fact that this is valid C++ is cursed as all hell lmao
    updates ? enableUpdates() : disableUpdates();
  }

  explicit operator ImTextureID() const {
    this->update();
    return reinterpret_cast<ImTextureID>(m_renderTexture);
  }
  explicit operator bool() const { return m_texture.operator bool(); }

private:
  void update() const;
  /* Project related information */
  Texture m_texture;
  Size m_size;
  Point m_scroll;
  bool m_loopX;
  bool m_loopY;

  /* Runtime status */
  bool m_updateScrolling{false};
  mutable void* m_renderTexture = nullptr;
  mutable void* m_workTexture = nullptr;
  mutable bool m_workDone{false};
  mutable PointF m_scrollPosition;
  mutable float m_frameTime{0.f};
};
