#include "Core/Graphics/Image.hpp"

#include "Core/ResourceManager.hpp"

Image::Image(const std::string_view imageName, const int pickType, const bool secondImage = false) {
  if (imageName.empty()) {
    return;
  }
  if (pickType == 0) {
    if (secondImage) {
      m_imageTexture = ResourceManager::instance()->loadBattlebacks2Image(imageName);
    } else {
      m_imageTexture = ResourceManager::instance()->loadBattlebacks1Image(imageName);
    }

  } else if (pickType == 1) {
    m_imageTexture = ResourceManager::instance()->loadParallaxImage(imageName);
  } else if (pickType == 2) {
    m_imageTexture = ResourceManager::instance()->loadPictureImage(imageName);
  } else if (pickType == 3) {
    if (secondImage) {
      m_imageTexture = ResourceManager::instance()->loadTitle2Image(imageName);
    } else {
      m_imageTexture = ResourceManager::instance()->loadTitle1Image(imageName);
    }
  } else if (pickType == 4) {
    m_imageTexture = ResourceManager::instance()->loadTilesetImage(imageName);
  }
}

int Image::imageWidth() const { return m_imageTexture.width(); }
int Image::imageHeight() const { return m_imageTexture.height(); }
