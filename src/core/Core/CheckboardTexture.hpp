#pragma once

#include "SDL2/SDL.h"
enum class CellSizes : uint8_t {
  _8 = (1 << 3),
  _16 = (1 << 4),
  _32 = (1 << 5),
  _64 = (1 << 6),
};
struct CheckboardTexture {

  CheckboardTexture() = default;
  CheckboardTexture(int width, int height, CellSizes cellSize = CellSizes::_64, uint8_t evenColor = 255,
                    uint8_t oddColor = 127);

  void* get() const { return m_texture; }
  explicit operator bool() const { return m_texture != nullptr; }

private:
  void* m_texture = nullptr;
  int m_width;
  int m_height;
};
