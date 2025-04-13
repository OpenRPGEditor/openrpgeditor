#include "Core/ResourceManager.hpp"

#include "Core/Log.hpp"
#include "Core/Resources.hpp"
#include "Core/Utils.hpp"
#include "Resources.hpp"
#include "SDL3/SDL.h"
#include "SFML/Audio/SoundBuffer.hpp"
#include <exception>
#include <iostream>
#include <SDL3/SDL_hints.h>
namespace fs = std::filesystem;

void ResourceManager::setBasepath(const std::string_view basepath) {
  m_basePath = basepath;
  m_dataPath = m_basePath / "data/";
  m_audioPath = m_basePath / "audio/";
  m_bgmPath = m_basePath / "audio/bgm/";
  m_bgsPath = m_basePath / "audio/bgs/";
  m_sePath = m_basePath / "audio/se/";
  m_mePath = m_basePath / "audio/me/";
  m_imgPath = m_basePath / "img/";
  m_animationsPath = m_basePath / "img/animations/";
  m_battlebacks1Path = m_basePath / "img/battlebacks1/";
  m_battlebacks2Path = m_basePath / "img/battlebacks2/";
  m_charactersPath = m_basePath / "img/characters/";
  m_enemiesPath = m_basePath / "img/enemies/";
  m_facesPath = m_basePath / "img/faces/";
  m_parallaxesPath = m_basePath / "img/parallaxes/";
  m_picturesPath = m_basePath / "img/pictures/";
  m_sv_actorsPath = m_basePath / "img/sv_actors/";
  m_sv_enemiesPath = m_basePath / "img/sv_enemies/";
  m_systemPath = m_basePath / "img/system/";
  m_tilesetsPath = m_basePath / "img/tilesets/";
  m_titles1Path = m_basePath / "img/titles1/";
  m_titles2Path = m_basePath / "img/titles2/";

  for (const auto& texture : m_loadedTextures | std::views::values) {
    SDL_DestroyTexture(static_cast<SDL_Texture*>(texture.m_texture));
  }
  m_loadedTextures.clear();
}

ResourceManager::~ResourceManager() {
  for (const auto& texture : m_loadedTextures | std::views::values) {
    SDL_DestroyTexture(static_cast<SDL_Texture*>(texture.m_texture));
  }
  m_loadedTextures.clear();

  for (const auto& texture : m_editorTextures | std::views::values) {
    SDL_DestroyTexture(static_cast<SDL_Texture*>(texture.m_texture));
  }
  m_editorTextures.clear();
}

sf::SoundBuffer ResourceManager::loadSound(std::filesystem::path path) {
#ifndef _WIN32
  auto pathStr = path.generic_string();
  auto pos = pathStr.find('\\');
  while (pos != pathStr.npos) {
    pathStr[pos] = std::filesystem::path::preferred_separator;
    pos = pathStr.find('\\', pos);
  }
  path = pathStr;
#else
  path.make_preferred();
#endif
  if (!fs::is_regular_file(path)) {
    return {};
  }

  sf::SoundBuffer buffer;
  buffer.loadFromFile(path.generic_string());
  return buffer;
}
sf::SoundBuffer ResourceManager::loadBGM(const std::string_view path) const { return loadSound((m_bgmPath / path.data()).replace_extension(".ogg")); }

sf::SoundBuffer ResourceManager::loadBGS(const std::string_view path) const { return loadSound((m_bgsPath / path.data()).replace_extension(".ogg")); }

sf::SoundBuffer ResourceManager::loadSE(const std::string_view path) const { return loadSound((m_sePath / path.data()).replace_extension(".ogg")); }

sf::SoundBuffer ResourceManager::loadME(const std::string_view path) const { return loadSound((m_mePath / path.data()).replace_extension(".ogg")); }

Texture ResourceManager::loadTexture(std::filesystem::path path) {
#ifndef _WIN32
  auto pathStr = path.generic_string();
  auto pos = pathStr.find('\\');
  while (pos != pathStr.npos) {
    pathStr[pos] = std::filesystem::path::preferred_separator;
    pos = pathStr.find('\\', pos);
  }
  path = pathStr;
#else
  path.make_preferred();
#endif
  if (!fs::is_regular_file(path)) {
    return {};
  }

  if (m_loadedTextures.contains(path.generic_string())) {
    return m_loadedTextures[path.generic_string()];
  }

  const Texture ret(path.generic_string());
  m_loadedTextures[path.generic_string()] = ret;
  return ret;
}

Texture ResourceManager::loadImage(const std::string_view path) { return loadTexture((m_imgPath / path).replace_extension(".png")); }

Texture ResourceManager::loadAnimationImage(const std::string_view path) { return loadTexture((m_animationsPath / path).replace_extension(".png")); }

Texture ResourceManager::loadBattlebacks1Image(const std::string_view path) { return loadTexture((m_battlebacks1Path / path).replace_extension(".png")); }

Texture ResourceManager::loadBattlebacks2Image(const std::string_view path) { return loadTexture((m_battlebacks2Path / path).replace_extension(".png")); }

Texture ResourceManager::loadCharacterImage(const std::string_view path) { return loadTexture((m_charactersPath / path).replace_extension(".png")); }

Texture ResourceManager::loadEnemyImage(std::string_view path) { return loadTexture((m_enemiesPath / path).replace_extension(".png")); }

Texture ResourceManager::loadFaceImage(std::string_view path) { return loadTexture((m_facesPath / path).replace_extension(".png")); }

Texture ResourceManager::loadParallaxImage(std::string_view path) { return loadTexture((m_parallaxesPath / path).replace_extension(".png")); }

Texture ResourceManager::loadPictureImage(std::string_view path) { return loadTexture((m_picturesPath / path).replace_extension(".png")); }

Texture ResourceManager::loadSVActorImage(std::string_view path) { return loadTexture((m_sv_actorsPath / path).replace_extension(".png")); }
Texture ResourceManager::loadSVEnemyImage(std::string_view path) { return loadTexture((m_sv_enemiesPath / path).replace_extension(".png")); }
Texture ResourceManager::loadSystemImage(std::string_view path) { return loadTexture((m_systemPath / path).replace_extension(".png")); }
Texture ResourceManager::loadTilesetImage(std::string_view path) { return loadTexture((m_tilesetsPath / path).replace_extension(".png")); }

Texture ResourceManager::loadTitle1Image(std::string_view path) { return loadTexture((m_titles1Path / path).replace_extension(".png")); }

Texture ResourceManager::loadTitle2Image(std::string_view path) { return loadTexture((m_titles2Path / path).replace_extension(".png")); }

Texture ResourceManager::loadTileMarkers(int width, int height) {
  fs::path relativePath = std::format("tilemarkers_{}x{}.png", width, height);
  return loadEditorTexture(relativePath.generic_string());
}
Texture ResourceManager::loadEditorTexture(std::string_view path) {
  auto imagePath = App::Resources::image_path(path).generic_string();
  if (!fs::is_regular_file(imagePath)) {
    return {};
  }

  if (m_editorTextures.contains(imagePath)) {
    return m_editorTextures[imagePath];
  }

  Texture ret(imagePath);
  m_editorTextures[imagePath] = ret;
  ret.setFilter(Texture::Linear);
  return ret;
}

std::vector<std::string> ResourceManager::getDirectoryContents(const std::string& directoryPath, const std::string_view filter) const {
  std::vector<std::string> fileNames;
  for (const auto& entry : fs::directory_iterator(m_basePath / directoryPath)) {
    if (!filter.empty() && entry.path().extension().compare(filter) != 0) {
      continue;
    }

    auto filename = entry.path().filename();
    filename = filename.replace_extension();
    const auto path = filename.generic_string();
    fileNames.emplace_back(path);
  }
  return fileNames;
}
std::vector<std::string> ResourceManager::getDirectories(const std::string& directoryPath) const {
  std::vector<std::string> fileNames;
  for (const auto& entry : fs::directory_iterator(m_basePath / directoryPath)) {
    if (!entry.is_directory()) {
      continue;
    }
    const auto path = entry.path().filename().generic_string();
    fileNames.emplace_back(path);
  }
  return fileNames;
}
