#include "Editor/ResourceManager.hpp"

#include "Editor/Log.hpp"
#include "Editor/Resources.hpp"
#include "Editor/Utils.hpp"
#include "Resources.hpp"
#include "SDL3/SDL.h"
#include "SFML/Audio/SoundBuffer.hpp"
#include <exception>
#include <iostream>
#include <SDL3/SDL_hints.h>
namespace fs = std::filesystem;

void ResourceManager::setBasepath(const std::string_view basepath) {
  m_basePath = basepath;
  m_dataPath = m_basePath / "data";
  m_audioPath = m_basePath / "audio";
  m_bgmPath = m_audioPath / "bgm";
  m_bgsPath = m_audioPath / "bgs";
  m_sePath = m_audioPath / "se";
  m_mePath = m_audioPath / "me";
  m_imgPath = m_basePath / "img";
  m_animationsPath = m_imgPath / "animations";
  m_battlebacks1Path = m_imgPath / "battlebacks1";
  m_battlebacks2Path = m_imgPath / "battlebacks2";
  m_charactersPath = m_imgPath / "characters";
  m_enemiesPath = m_imgPath / "enemies";
  m_facesPath = m_imgPath / "faces";
  m_parallaxesPath = m_imgPath / "parallaxes";
  m_picturesPath = m_imgPath / "pictures";
  m_sv_actorsPath = m_imgPath / "sv_actors";
  m_sv_enemiesPath = m_imgPath / "sv_enemies";
  m_systemPath = m_imgPath / "system";
  m_tilesetsPath = m_imgPath / "tilesets";
  m_titles1Path = m_imgPath / "titles1";
  m_titles2Path = m_imgPath / "titles2";

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

Texture ResourceManager::loadEnemyImage(const std::string_view path) { return loadTexture((m_enemiesPath / path).replace_extension(".png")); }

Texture ResourceManager::loadFaceImage(const std::string_view path) { return loadTexture((m_facesPath / path).replace_extension(".png")); }

Texture ResourceManager::loadParallaxImage(const std::string_view path) { return loadTexture((m_parallaxesPath / path).replace_extension(".png")); }

Texture ResourceManager::loadPictureImage(const std::string_view path) { return loadTexture((m_picturesPath / path).replace_extension(".png")); }

Texture ResourceManager::loadSVActorImage(const std::string_view path) { return loadTexture((m_sv_actorsPath / path).replace_extension(".png")); }
Texture ResourceManager::loadSVEnemyImage(const std::string_view path) { return loadTexture((m_sv_enemiesPath / path).replace_extension(".png")); }
Texture ResourceManager::loadSystemImage(const std::string_view path) { return loadTexture((m_systemPath / path).replace_extension(".png")); }
Texture ResourceManager::loadTilesetImage(const std::string_view path) { return loadTexture((m_tilesetsPath / path).replace_extension(".png")); }

Texture ResourceManager::loadTitle1Image(const std::string_view path) { return loadTexture((m_titles1Path / path).replace_extension(".png")); }

Texture ResourceManager::loadTitle2Image(const std::string_view path) { return loadTexture((m_titles2Path / path).replace_extension(".png")); }

Texture ResourceManager::loadTileMarkers(const int width, const int height) {
  const fs::path relativePath = std::format("tilemarkers_{}x{}.png", width, height);
  return loadEditorTexture(relativePath.generic_string());
}
Texture ResourceManager::loadEditorTexture(const std::string_view path) {
  const auto imagePath = App::Resources::image_path(path).generic_string();
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
