#include "Core/Graphics/CheckerboardTexture.hpp"
#include "Core/Application.hpp"

CheckerboardTexture::CheckerboardTexture(const int width, const int height, const CellSizes cellSize, const uint8_t evenColor, const uint8_t oddColor)
: m_width(width), m_height(height), m_cellSize(cellSize), m_evenColor(evenColor), m_oddColor(oddColor) {}

CheckerboardTexture::CheckerboardTexture(const CheckerboardTexture& other) {
  m_width = other.m_width;
  m_height = other.m_height;
  m_cellSize = other.m_cellSize;
  m_evenColor = other.m_evenColor;
  m_oddColor = other.m_oddColor;
  m_texture = other.m_texture;
  other.m_texture = nullptr;
}

CheckerboardTexture& CheckerboardTexture::operator=(const CheckerboardTexture& other) {
  m_width = other.m_width;
  m_height = other.m_height;
  m_texture = other.m_texture;
  m_cellSize = other.m_cellSize;
  m_evenColor = other.m_evenColor;
  m_oddColor = other.m_oddColor;
  m_texture = other.m_texture;
  other.m_texture = nullptr;
  return *this;
}

void CheckerboardTexture::updateTexture() const {
  if (m_texture != nullptr) {
    return;
  }

  const uint8_t colors[2]{m_oddColor, m_evenColor};
  auto* data = static_cast<uint8_t*>(malloc(m_width * m_height * 4));
  for (int y = 0; y < m_height * 4; y += 4) {
    for (int x = 0; x < m_width * 4; x += 4) {
      const uint8_t c = colors[y / static_cast<int>(m_cellSize) % 2 ^ x / static_cast<int>(m_cellSize) % 2];
      *(data + (y * m_width + x) + 0) = c;
      *(data + (y * m_width + x) + 1) = c;
      *(data + (y * m_width + x) + 2) = c;
      *(data + (y * m_width + x) + 3) = static_cast<uint8_t>(255);
    }
  }
  SDL_Renderer* renderer = App::APP->getWindow()->getNativeRenderer();

  SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(data, m_width, m_height, 32, 4 * m_width, SDL_PIXELFORMAT_ARGB8888);

  m_texture = SDL_CreateTextureFromSurface(renderer, surface);

  free(data);
}
CheckerboardTexture::~CheckerboardTexture() {
  SDL_DestroyTexture(static_cast<SDL_Texture*>(m_texture));
  m_texture = nullptr;
}