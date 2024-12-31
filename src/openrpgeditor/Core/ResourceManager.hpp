#pragma once

#include "SFML/Audio/Sound.hpp"

#include <map>
#include <string>

#include "Core/Graphics/Texture.hpp"

#include "Database/Map.hpp"

class ResourceManager {
public:
  ResourceManager() = delete;
  ResourceManager(ResourceManager&) = delete;
  ResourceManager(ResourceManager&&) = delete;
  ResourceManager& operator=(ResourceManager&) = delete;
  ResourceManager& operator=(ResourceManager&&) = delete;
  explicit ResourceManager(std::string_view basePath);
  ~ResourceManager();

  sf::SoundBuffer loadBGM(std::string_view path) const;
  sf::SoundBuffer loadBGS(std::string_view path) const;
  sf::SoundBuffer loadSE(std::string_view path) const;
  Texture loadImage(std::string_view path);
  Texture loadAnimationImage(std::string_view path);
  Texture loadBattlebacks1Image(std::string_view path);
  Texture loadBattlebacks2Image(std::string_view path);
  Texture loadCharacterImage(std::string_view path);
  Texture loadEnemyImage(std::string_view path);
  Texture loadFaceImage(std::string_view path);
  Texture loadParallaxImage(std::string_view path);
  Texture loadPictureImage(std::string_view path);
  Texture loadSVActorImage(std::string_view path);
  Texture loadSVEnemyImage(std::string_view path);
  Texture loadSystemImage(std::string_view path);
  Texture loadTilesetImage(std::string_view path);
  Texture loadTitle1Image(std::string_view path);
  Texture loadTitle2Image(std::string_view path);
  Texture loadEditorTexture(std::string_view path);
  Texture loadTileMarkers(int width, int height);

  static ResourceManager* instance() { return m_instance; }

  [[nodiscard]] std::vector<std::string> getDirectoryContents(const std::string& directoryPath, std::string_view filter = {}) const;

private:
  static sf::SoundBuffer loadSound(std::string_view path);
  Texture loadTexture(std::string_view path);
  std::map<std::string, Texture> m_loadedTextures; // Path -> texture
  std::filesystem::path m_basePath;
  std::filesystem::path m_dataPath;
  std::filesystem::path m_audioPath;
  std::filesystem::path m_bgmPath;
  std::filesystem::path m_bgsPath;
  std::filesystem::path m_sePath;
  std::filesystem::path m_imgPath;
  std::filesystem::path m_animationsPath;
  std::filesystem::path m_battlebacks1Path;
  std::filesystem::path m_battlebacks2Path;
  std::filesystem::path m_charactersPath;
  std::filesystem::path m_enemiesPath;
  std::filesystem::path m_facesPath;
  std::filesystem::path m_parallaxesPath;
  std::filesystem::path m_picturesPath;
  std::filesystem::path m_sv_actorsPath;
  std::filesystem::path m_sv_enemiesPath;
  std::filesystem::path m_systemPath;
  std::filesystem::path m_tilesetsPath;
  std::filesystem::path m_titles1Path;
  std::filesystem::path m_titles2Path;

  static ResourceManager* m_instance;
};