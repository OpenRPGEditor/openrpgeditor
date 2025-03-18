#include "Core/Graphics/Image.hpp"

#include "Core/ResourceManager.hpp"

Image::Image(const std::string_view imageName, const Mode pickType, const bool secondImage = false) {
  if (imageName.empty()) {
    return;
  }
  switch (pickType) {
  case Mode::Battleback:
    if (secondImage) {
      m_imageTexture = ResourceManager::instance()->loadBattlebacks2Image(imageName);
    } else {
      m_imageTexture = ResourceManager::instance()->loadBattlebacks1Image(imageName);
    }
    break;
  case Mode::Parallax:
    m_imageTexture = ResourceManager::instance()->loadParallaxImage(imageName);
    break;
  case Mode::Picture:
    m_imageTexture = ResourceManager::instance()->loadPictureImage(imageName);
    break;
  case Mode::Title:
    if (secondImage) {
      m_imageTexture = ResourceManager::instance()->loadTitle2Image(imageName);
    } else {
      m_imageTexture = ResourceManager::instance()->loadTitle1Image(imageName);
    }
    break;
  case Mode::Tileset:
    m_imageTexture = ResourceManager::instance()->loadTilesetImage(imageName);
    break;
  case Mode::Animation:
    m_imageTexture = ResourceManager::instance()->loadAnimationImage(imageName);
    break;
  case Mode::SVBattler:
    m_imageTexture = ResourceManager::instance()->loadSVActorImage(imageName);
    break;
  case Mode::Window:
    m_imageTexture = ResourceManager::instance()->loadSystemImage("Window.png");
    break;
  }
  m_imageName = imageName;
}

int Image::imageWidth() const { return m_imageTexture.width(); }
int Image::imageHeight() const { return m_imageTexture.height(); }
std::string Image::imageName() const { return m_imageName; }
