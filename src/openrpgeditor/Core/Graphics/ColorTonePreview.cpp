#include "Core/Graphics/ColorTonePreview.hpp"
#include "Core/Application.hpp"
#include "OREMath/Color.hpp"
#include "OREMath/Matrix4x4.hpp"
#include "OREMath/Vector3D.hpp"
#include "SDL3/SDL_render.h"

double fract(double x) { return x - floor(x); }

double hueToIntensity(double v1, double v2, double h) {
  h = fract(h);
  if (h < 1.0 / 6.0)
    return v1 + (v2 - v1) * 6.0 * h;
  if (h < 1.0 / 2.0)
    return v2;
  if (h < 2.0 / 3.0)
    return v1 + (v2 - v1) * 6.0 * (2.0 / 3.0 - h);
  return v1;
}

Vector3D HSLtoRGB(const Vector3D& color) {
  double h = color.x();
  double l = color.z();
  double s = color.y();
  if (s < 1.0 / 256.0)
    return {static_cast<float>(l), static_cast<float>(l), static_cast<float>(l)};
  double v1;
  double v2;
  if (l < 0.5)
    v2 = l * (1.0 + s);
  else
    v2 = (l + s) - (s * l);
  v1 = 2.0 * l - v2;
  double d = 1.0 / 3.0;
  double r = hueToIntensity(v1, v2, h + d);
  double g = hueToIntensity(v1, v2, h);
  double b = hueToIntensity(v1, v2, h - d);
  return Vector3D{static_cast<float>(r), static_cast<float>(g), static_cast<float>(b)};
}

ColorTonePreview::ColorTonePreview(const int width, const int height) : m_textureWidth(width), m_textureHeight(height) {}

ColorTonePreview::~ColorTonePreview() {
  SDL_DestroyTexture(static_cast<SDL_Texture*>(m_texture));
  m_texture = nullptr;
}

void ColorTonePreview::update(float r, float g, float b, float gray) {
  const float s = 1 - gray;
  const float a00 = 0.213 + 0.787 * s;
  const float a01 = 0.715 - 0.715 * s;
  const float a02 = 0.072 - 0.072 * s;
  const float a10 = 0.213 - 0.213 * s;
  const float a11 = 0.715 + 0.285 * s;
  const float a12 = 0.072 - 0.072 * s;
  const float a20 = 0.213 - 0.213 * s;
  const float a21 = 0.715 - 0.715 * s;
  const float a22 = 0.072 + 0.928 * s;
  Matrix4x4 matrix(a00, a10, a20, 0, a01, a11, a21, 0, a02, a12, a22, 0, r, g, b, 1);
  if (!m_texture) {
    m_texture = SDL_CreateTexture(App::APP->getWindow()->getNativeRenderer(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, m_textureWidth, m_textureHeight);
  }

  uint32_t* pixels = nullptr;
  int pitch = 0;
  if (SDL_LockTexture(static_cast<SDL_Texture*>(m_texture), nullptr, reinterpret_cast<void**>(&pixels), &pitch) == 0) {
    for (int y = 0; y < m_textureHeight; y++) {
      for (int x = 0; x < m_textureWidth; x++) {
        const float s = static_cast<float>(x) / static_cast<float>(m_textureWidth);
        const float t = static_cast<float>(y) / static_cast<float>(m_textureHeight);
        auto color = HSLtoRGB({t, 1.f, s}) * matrix;
        pixels[(y * m_textureWidth) + x] =
            Color::rgb(oRound(std::clamp(color.x(), 0.f, 1.f) * 255.f), oRound(std::clamp(color.y(), 0.f, 1.f) * 255.f), oRound(std::clamp(color.z(), 0.f, 1.f) * 255.f));
      }
    }
    SDL_UnlockTexture(static_cast<SDL_Texture*>(m_texture));
  }
}

void ColorTonePreview::setSize(const int width, const int height) {
  if (width != m_textureWidth || height != m_textureHeight) {
    SDL_DestroyTexture(static_cast<SDL_Texture*>(m_texture));
    m_texture = nullptr;
    m_textureWidth = width;
    m_textureHeight = height;
  }
}
