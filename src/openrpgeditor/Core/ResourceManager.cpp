#include "Core/ResourceManager.hpp"

#include "Core/Log.hpp"
#include "Core/Utils.hpp"
#include "SDL2/SDL.h"
#include "SFML/Audio/SoundBuffer.hpp"
#include <exception>
namespace fs = std::filesystem;

ResourceManager* ResourceManager::m_instance = nullptr;
ResourceManager::ResourceManager(std::string_view basePath)
: m_basePath(basePath)
, m_dataPath(std::string(basePath.data()) + "data/")
, m_audioPath(std::string(basePath.data()) + "audio/")
, m_bgmPath(std::string(basePath.data()) + "audio/bgm/")
, m_bgsPath(std::string(basePath.data()) + "audio/bgs/")
, m_sePath(std::string(basePath.data()) + "audio/se/")
, m_imgPath(std::string(basePath.data()) + "img/")
, m_animationsPath(std::string(basePath.data()) + "img/animations/")
, m_battlebacks1Path(std::string(basePath.data()) + "img/battlebacks1/")
, m_battlebacks2Path(std::string(basePath.data()) + "img/battlebacks2/")
, m_charactersPath(std::string(basePath.data()) + "img/characters/")
, m_enemiesPath(std::string(basePath.data()) + "img/enemies/")
, m_facesPath(std::string(basePath.data()) + "img/faces/")
, m_parallaxesPath(std::string(basePath.data()) + "img/parallaxes/")
, m_picturesPath(std::string(basePath.data()) + "img/pictures/")
, m_sv_actorsPath(std::string(basePath.data()) + "img/sv_actors/")
, m_sv_enemiesPath(std::string(basePath.data()) + "img/sv_enemies/")
, m_systemPath(std::string(basePath.data()) + "img/system/")
, m_tilesetsPath(std::string(basePath.data()) + "img/tilesets/")
, m_titles1Path(std::string(basePath.data()) + "img/titles1/")
, m_titles2Path(std::string(basePath.data()) + "img/titles2/") {
  m_instance = this;
}

ResourceManager::~ResourceManager() {
  if (m_instance == this) {
    m_instance = nullptr;
  }
  for (auto& [_, texture] : m_loadedTextures) {
    SDL_DestroyTexture(static_cast<SDL_Texture*>(texture.m_texture));
  }
  m_loadedTextures.clear();
}

sf::SoundBuffer ResourceManager::loadSound(const std::string_view path) {
  if (!fs::is_regular_file(path)) {
    return {};
  }

  sf::SoundBuffer buffer;
  buffer.loadFromFile(path.data());
  return buffer;
}
sf::SoundBuffer ResourceManager::loadBGM(const std::string_view path) const {
  std::string fullpath = m_bgmPath + path.data() + ".ogg";
  return loadSound(fullpath);
}

sf::SoundBuffer ResourceManager::loadBGS(const std::string_view path) const {
  std::string fullpath = m_bgsPath + path.data() + ".ogg";
  return loadSound(fullpath);
}

sf::SoundBuffer ResourceManager::loadSE(const std::string_view path) const {
  std::string fullpath = m_sePath + path.data() + ".ogg";
  return loadSound(fullpath);
}

Texture ResourceManager::loadTexture(const std::string_view path) {
  if (!fs::is_regular_file(path)) {
    return {};
  }

  if (m_loadedTextures.contains(path.data())) {
    return m_loadedTextures[path.data()];
  }

  const Texture ret(path);
  m_loadedTextures[path.data()] = ret;
  return ret;
}

Texture ResourceManager::loadImage(const std::string_view path) {
  std::string fullpath = m_imgPath + path.data() + ".png";
  return loadTexture(fullpath);
}

Texture ResourceManager::loadAnimationImage(const std::string_view path) {
  std::string fullpath = m_animationsPath + path.data() + ".png";
  return loadTexture(fullpath);
}

Texture ResourceManager::loadBattlebacks1Image(const std::string_view path) {
  std::string fullpath = m_battlebacks1Path + path.data() + ".png";
  return loadTexture(fullpath);
}

Texture ResourceManager::loadBattlebacks2Image(const std::string_view path) {
  std::string fullpath = m_battlebacks2Path + path.data() + ".png";
  return loadTexture(fullpath);
}

Texture ResourceManager::loadCharacterImage(const std::string_view path) {
  std::string fullpath = m_charactersPath + path.data() + ".png";
  return loadTexture(fullpath);
}

Texture ResourceManager::loadEnemyImage(std::string_view path) {
  std::string fullpath = m_enemiesPath + path.data() + ".png";
  return loadTexture(fullpath);
}

Texture ResourceManager::loadFaceImage(std::string_view path) {
  std::string fullpath = m_facesPath + path.data() + ".png";
  return loadTexture(fullpath);
}

Texture ResourceManager::loadParallaxImage(std::string_view path) {
  std::string fullpath = m_parallaxesPath + path.data() + ".png";
  return loadTexture(fullpath);
}

Texture ResourceManager::loadPictureImage(std::string_view path) {
  std::string fullpath = m_picturesPath + path.data() + ".png";
  return loadTexture(fullpath);
}

Texture ResourceManager::loadSVActorImage(std::string_view path) {
  std::string fullpath = m_sv_actorsPath + path.data() + ".png";
  return loadTexture(fullpath);
}
Texture ResourceManager::loadSVEnemyImage(std::string_view path) {
  std::string fullpath = m_sv_enemiesPath + path.data() + ".png";
  return loadTexture(fullpath);
}
Texture ResourceManager::loadSystemImage(std::string_view path) {
  std::string fullpath = m_systemPath + path.data() + ".png";
  return loadTexture(fullpath);
}
Texture ResourceManager::loadTilesetImage(std::string_view path) {
  std::string fullpath = m_tilesetsPath + path.data() + ".png";
  return loadTexture(fullpath);
}

Texture ResourceManager::loadTitle1Image(std::string_view path) {
  std::string fullpath = m_titles1Path + path.data() + ".png";
  return loadTexture(fullpath);
}

Texture ResourceManager::loadTitle2Image(std::string_view path) {
  std::string fullpath = m_titles2Path + path.data() + ".png";
  return loadTexture(fullpath);
}

std::vector<std::string> ResourceManager::getDirectoryContents(const std::string& directoryPath, const std::string_view filter) const {
  std::vector<std::string> fileNames;
  for (const auto& entry : fs::directory_iterator(m_basePath + directoryPath)) {
    if (!filter.empty() && entry.path().extension().compare(filter) != 0) {
      continue;
    }

    auto filename = entry.path().filename();
    filename = filename.replace_extension();
    fileNames.emplace_back(filename.generic_string());
  }
  return fileNames;
}
