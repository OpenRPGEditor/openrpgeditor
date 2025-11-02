#pragma once
#include "imgui.h"

class ColorTonePreview {
public:
  ColorTonePreview() = default;
  ColorTonePreview(int width, int height);
  ~ColorTonePreview();
  void update(float r, float g, float b, float gray);

  operator ImTextureRef() { return ImTextureRef(m_texture); }

  int width() const { return m_textureWidth; }
  int height() const { return m_textureHeight; }

  void setSize(int width, int height);

private:
  int m_textureWidth{60};
  int m_textureHeight{100};
  void* m_texture{nullptr};
};
