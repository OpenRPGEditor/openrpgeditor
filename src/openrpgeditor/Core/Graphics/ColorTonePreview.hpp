#pragma once
#include "imgui.h"

class ColorTonePreview {
public:
  ColorTonePreview(int width, int height);
  ~ColorTonePreview();
  void update(float r, float g, float b, float gray);

  operator ImTextureID() { return reinterpret_cast<ImTextureID>(m_texture); }

  int width() const { return m_textureWidth; }
  int height() const { return m_textureHeight; }

  void setSize(int width, int height);

  void draw();

  void setValues(int* r, int* g, int* b, int* gray) {
    m_r = r;
    m_g = g;
    m_b = b;
    m_gray = gray;
    if (r) {
      m_oldR = *r;
    }

    if (g) {
      m_oldG = *g;
    }
    if (b) {
      m_oldB = *b;
    }

    if (gray) {
      m_oldGray = *gray;
    }
  }

  [[nodiscard]] bool isAnyFieldChanged() const {
    return (m_r && *m_r != m_oldR) || (m_g && *m_g != m_oldG) || (m_b && *m_b != m_oldB) ||
           (m_gray && *m_gray != m_oldGray);
  }

private:
  int m_textureWidth;
  int m_textureHeight;
  void* m_texture = nullptr;

  int* m_r = nullptr;
  int* m_g = nullptr;
  int* m_b = nullptr;
  int* m_gray = nullptr;
  int m_oldR;
  int m_oldG;
  int m_oldB;
  int m_oldGray;
  bool m_invalid = true;
};
