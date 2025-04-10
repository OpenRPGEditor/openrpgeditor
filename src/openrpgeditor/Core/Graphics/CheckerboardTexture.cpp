#include "Core/Graphics/CheckerboardTexture.hpp"
#include "Core/Application.hpp"

CheckerboardTexture::CheckerboardTexture(const int width, const int height, const CellSizes cellSize, const uint8_t evenColor, const uint8_t oddColor)
: m_width(width)
, m_height(height)
, m_pendingWidth(width)
, m_pendingHeight(height)
, m_cellSize(cellSize)
, m_evenColor(evenColor)
, m_oddColor(oddColor) {}

CheckerboardTexture::CheckerboardTexture(const CheckerboardTexture& other) {
  m_width = other.m_width;
  m_height = other.m_height;
  m_pendingWidth = other.m_pendingWidth;
  m_pendingHeight = other.m_pendingHeight;
  m_cellSize = other.m_cellSize;
  m_evenColor = other.m_evenColor;
  m_oddColor = other.m_oddColor;
  m_texture = other.m_texture;
  other.m_texture = nullptr;
}

CheckerboardTexture& CheckerboardTexture::operator=(const CheckerboardTexture& other) {
  m_width = other.m_width;
  m_height = other.m_height;
  m_pendingWidth = other.m_pendingWidth;
  m_pendingHeight = other.m_pendingHeight;
  m_texture = other.m_texture;
  m_cellSize = other.m_cellSize;
  m_evenColor = other.m_evenColor;
  m_oddColor = other.m_oddColor;
  m_texture = other.m_texture;
  other.m_texture = nullptr;
  return *this;
}

void CheckerboardTexture::setWidth(const int width) {
  if (width == m_width) {
    return;
  }
  m_pendingWidth = width;
  m_workDone = false;
}

void CheckerboardTexture::setHeight(const int height) {
  if (height == m_height) {
    return;
  }
  m_pendingHeight = height;
  m_workDone = false;
}

void CheckerboardTexture::setSize(const int width, const int height) {
  if (width == m_width && m_height == height) {
    return;
  }
  m_pendingWidth = width;
  m_pendingHeight = height;
  m_workDone = false;
}

void CheckerboardTexture::update() const {
  if (m_workDone) {
    if (!m_workTexture) {
      return;
    }
    if (m_frameDelay > 0) {
      m_frameDelay--;
    }
    if (m_texture != m_workTexture && m_frameDelay <= 0) {
      if (m_texture != nullptr) {
        SDL_DestroyTexture(static_cast<SDL_Texture*>(m_texture));
      }
      m_texture = m_workTexture;
      m_workTexture = nullptr;
      m_width = m_pendingWidth;
      m_height = m_pendingHeight;
      m_frameDelay = 0;
    }
    return;
  }
  SDL_Renderer* renderer = App::APP->getWindow()->getNativeRenderer();

  if (!m_checkerTexture) {
    m_checkerSize = static_cast<int>(m_cellSize) * 2;
    const uint8_t colors[2]{m_oddColor, m_evenColor};
    auto* data = static_cast<uint8_t*>(malloc(m_checkerSize * m_checkerSize * 4));
    for (int y = 0; y < m_checkerSize * 4; y += 4) {
      for (int x = 0; x < m_checkerSize * 4; x += 4) {
        const uint8_t c = colors[y / static_cast<int>(m_cellSize) % 2 ^ x / static_cast<int>(m_cellSize) % 2];
        *(data + (y * m_checkerSize + x) + 0) = c;
        *(data + (y * m_checkerSize + x) + 1) = c;
        *(data + (y * m_checkerSize + x) + 2) = c;
        *(data + (y * m_checkerSize + x) + 3) = static_cast<uint8_t>(255);
      }
    }

    SDL_Surface* surface = SDL_CreateSurfaceFrom(m_checkerSize, m_checkerSize, SDL_PIXELFORMAT_ARGB8888, data, 4 * m_checkerSize);

    m_checkerTexture = SDL_CreateTextureFromSurface(renderer, surface);
    free(data);
  }

  m_workTexture = SDL_CreateTexture(App::APP->getWindow()->getNativeRenderer(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, m_pendingWidth, m_pendingHeight);
  SDL_SetTextureBlendMode(static_cast<SDL_Texture*>(m_workTexture), SDL_BLENDMODE_BLEND);
  auto* oldTarget = SDL_GetRenderTarget(renderer);
  SDL_SetRenderTarget(renderer, static_cast<SDL_Texture*>(m_workTexture));
  SDL_RenderTextureTiled(renderer, static_cast<SDL_Texture*>(m_checkerTexture), nullptr, 1.f, nullptr);

  SDL_SetRenderTarget(renderer, oldTarget);
  m_workDone = true;
  if (m_texture == nullptr) {
    m_texture = m_workTexture;
    m_workTexture = nullptr;
    m_width = m_pendingWidth;
    m_height = m_pendingHeight;
  } else {
    m_frameDelay = 1;
  }
}

CheckerboardTexture::~CheckerboardTexture() {
  SDL_DestroyTexture(static_cast<SDL_Texture*>(m_texture));
  m_texture = nullptr;
}