#pragma once
#include "OREMath/Color.hpp"
#include "OREMath/Rect.hpp"

#ifndef ImTextureID
typedef unsigned long long ImTextureID;
#endif

class Texture;
class RenderImage {

public:
  enum class CompositionMode {
    SourceOver,
    DestinationOver,
    Clear,
    Source,
    Destination,
    SourceIn,
    DestinationIn,
    SourceOut,
    DestinationOut,
    SourceAtop,
    DestinationAtop,
    Xor,
    Plus,
  };
  RenderImage() = default;
  RenderImage(int width, int height);

  void* get() const { return m_renderImage[m_currentBuffer]; }
  explicit operator ImTextureID() const { return reinterpret_cast<ImTextureID>(get()); }

  bool lock();
  void unlock();
  void eraseRect(const RectF& rect) const;
  void fillRect(const RectF& rect, const Color& color) const;
  void drawImage(const RectF& target, const Texture& image, const Rect& src) const;
  void drawImageRaw(const RectF& target, void* image, const Rect& src) const;

  void setSize(int width, int height);
  CompositionMode compositionMode() const { return m_compositionMode; }
  void setCompositionMode(const CompositionMode mode) { m_compositionMode = mode; }

private:
  void createTexture();
  void setCurrentCompositionMode() const;
  void* m_renderImage[2]{nullptr, nullptr};
  int m_currentBuffer = 1;
  int m_workBuffer = 0;
  bool m_bufferInvalid[2] = {true, true};
  CompositionMode m_compositionMode = CompositionMode::SourceOver;
  int m_width[2] = {0, 0};
  int m_height[2] = {0, 0};
  int m_pendingWidth = 0;
  int m_pendingHeight = 0;
  int m_oldBlend = 0;
  void* m_oldTarget{nullptr};
  bool m_locked{false};
};