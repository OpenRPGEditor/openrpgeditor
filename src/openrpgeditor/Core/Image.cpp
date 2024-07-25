#include "Core/Image.hpp"

#include "Core/ResourceManager.hpp"
#include "Log.hpp"

Image::Image(std::string_view imageName, std::string_view imageName2, int pickType) {
  if (imageName.empty()) {
    return;
  }
  if (pickType == 0) {
    m_imageTexture = ResourceManager::instance()->loadBattlebacks1Image(imageName);
    m_imageTexture2 = ResourceManager::instance()->loadBattlebacks2Image(imageName2);

  } else if (pickType == 1) {
    m_imageTexture = ResourceManager::instance()->loadParallaxImage(imageName);
  }
  else {
    m_imageTexture = ResourceManager::instance()->loadPictureImage(imageName);
  }
}

int Image::imageWidth() const { return m_imageTexture.width(); }
int Image::imageHeight() const { return m_imageTexture.height(); }

