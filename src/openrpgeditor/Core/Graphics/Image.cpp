#include "Core/Graphics/Image.hpp"

#include "Core/ResourceManager.hpp"

Image::Image(std::string_view imageName, int pickType, bool battleBack2 = false) {
  if (imageName.empty()) {
    return;
  }
  if (pickType == 0) {
    if (battleBack2) {
      m_imageTexture = ResourceManager::instance()->loadBattlebacks2Image(imageName);
    } else {
      m_imageTexture = ResourceManager::instance()->loadBattlebacks1Image(imageName);
    }

  } else if (pickType == 1) {
    m_imageTexture = ResourceManager::instance()->loadParallaxImage(imageName);
  } else {
    m_imageTexture = ResourceManager::instance()->loadPictureImage(imageName);
  }
}

int Image::imageWidth() const { return m_imageTexture.width(); }
int Image::imageHeight() const { return m_imageTexture.height(); }
