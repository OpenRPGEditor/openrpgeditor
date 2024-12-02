#pragma once
#include "CheckerboardTexture.hpp"
#include "Core/Graphics/Texture.hpp"
#include "OREMath/Point.hpp"

#include <vector>

struct CompositeTexture {
  Texture m_texture;
  Point originalSize;
  Point offset;
  bool scaleToRect{true};
};

class CheckeredCompositeTexture {
public:
  CheckeredCompositeTexture();

  void setSize(int finalWidth, int finalHeight);
  void setTexturesToComposite(const std::vector<CompositeTexture>& texturesToComposite);
  bool hasCompositeTextures() const { return !m_texturesToComposite.empty(); }
  ImTextureID get();

  int width() const { return m_finalSize.x(); }
  int height() const { return m_finalSize.y(); }

  Point size() const { return m_finalSize; }

  void clear() {
    m_texturesToComposite.clear();
    m_isDirty = true;
  }

private:
  void update();
  Point m_finalSize;
  void* m_compositedTexture = nullptr;
  CheckerboardTexture m_checker;
  std::vector<CompositeTexture> m_texturesToComposite;
  bool m_isDirty{true};
};
