#pragma once

#include "SDL2/SDL.h"
enum class CellSizes : uint8_t {
  _8 = 8,
  _16 = 16,
  _24 = 24,
  _32 = 32,
  _48 = 48,
  _64 = 64,
};
struct CheckerboardTexture {

  CheckerboardTexture() = default;
  CheckerboardTexture(int width, int height, CellSizes cellSize = CellSizes::_64, uint8_t evenColor = 255,
                    uint8_t oddColor = 127);

  void* get() const { return m_texture; }
  explicit operator bool() const { return m_texture != nullptr; }

private:
  void* m_texture = nullptr;
  int m_width;
  int m_height;
};
