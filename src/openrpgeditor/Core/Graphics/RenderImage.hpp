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

  void* get() const { return m_renderImage; }
  explicit operator ImTextureID() const { return reinterpret_cast<ImTextureID>(m_renderImage); }

  bool bind();
  void unbind();
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
  void* m_renderImage = nullptr;
  CompositionMode m_compositionMode = CompositionMode::SourceOver;

  int m_width{0};
  int m_height{0};
  int m_oldBlend{0};
  void* m_oldTarget{nullptr};
  bool m_bound{false};
};