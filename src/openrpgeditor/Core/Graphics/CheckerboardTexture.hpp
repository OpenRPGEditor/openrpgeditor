#pragma once

#include "SDL2/SDL.h"
#include "imgui.h"
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
  CheckerboardTexture(int width, int height, CellSizes cellSize = CellSizes::_64, uint8_t evenColor = 255, uint8_t oddColor = 220);
  CheckerboardTexture(const CheckerboardTexture& other);
  CheckerboardTexture& operator=(const CheckerboardTexture& other);
  ~CheckerboardTexture();

  [[nodiscard]] void* get() const {
    update();
    return m_texture;
  }
  explicit operator bool() const { return m_texture != nullptr; }

  explicit operator ImTextureID() const { return reinterpret_cast<ImTextureID>(get()); }

  int width() const { return m_width; }
  int height() const { return m_height; }
  void setWidth(int width);
  void setHeight(int height);

  void setSize(int width, int height);
  void update() const;

private:
  mutable void* m_checkerTexture = nullptr;
  mutable void* m_texture = nullptr;
  mutable void* m_workTexture = nullptr;
  mutable bool m_workDone{false};
  mutable int m_width{0};
  mutable int m_height{0};
  int m_pendingWidth{0};
  int m_pendingHeight{0};
  CellSizes m_cellSize{CellSizes::_64};
  mutable int m_checkerSize{0};
  uint8_t m_evenColor{255};
  uint8_t m_oddColor{220};
  mutable int m_workX{0};
  mutable int m_workY{0};
  mutable int m_frameDelay{0};
};
