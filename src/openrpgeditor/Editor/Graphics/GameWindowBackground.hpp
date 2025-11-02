#pragma once

#include "Editor/Graphics/Texture.hpp"

#include <imgui.h>

class GameWindowBackground {
public:
  GameWindowBackground() = default;
  GameWindowBackground(const int width, const int height)
  : m_width(width)
  , m_height(height) {}
  void update(int r, int g, int b);

  operator ImTextureRef() const { return ImTextureRef(m_resultTexture); }

  int width() const { return m_width; }
  int height() const { return m_height; }

  void setSize(int width, int height);

private:
  void* m_renderTexture = nullptr;
  void* m_resultTexture = nullptr;
  int m_width{96};
  int m_height{96};
  Texture m_windowTexture;
  bool m_sizeChanged = true;
  bool m_isInitialized = false;
};
