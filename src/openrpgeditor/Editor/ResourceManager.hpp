#pragma once

#include "SFML/Audio/Sound.hpp"

#include <map>
#include <string>

#include "Editor/Graphics/Texture.hpp"

#include "Database/Map.hpp"

class ResourceManager {
public:
  ResourceManager(ResourceManager&) = delete;
  ResourceManager(ResourceManager&&) = delete;
  ResourceManager& operator=(ResourceManager&) = delete;
  ResourceManager& operator=(ResourceManager&&) = delete;
  ~ResourceManager();

  void setBasepath(const std::string_view basepath);
  sf::SoundBuffer loadBGM(std::string_view path) const;
  sf::SoundBuffer loadBGS(std::string_view path) const;
  sf::SoundBuffer loadSE(std::string_view path) const;
  sf::SoundBuffer loadME(std::string_view path) const;
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

  static ResourceManager* instance() {
    static ResourceManager instance;
    return &instance;
  }

  [[nodiscard]] std::vector<std::string> getDirectoryContents(const std::string& directoryPath, std::string_view filter = {}) const;
  std::vector<std::string> getDirectories(const std::string& directoryPath) const;

private:
  ResourceManager() = default;
  static sf::SoundBuffer loadSound(std::filesystem::path path);
  Texture loadTexture(std::filesystem::path path);
  std::map<std::string, Texture> m_loadedTextures{}; // Path -> texture
  std::map<std::string, Texture> m_editorTextures{}; // Path -> texture
  std::filesystem::path m_basePath{};
  std::filesystem::path m_dataPath{};
  std::filesystem::path m_audioPath{};
  std::filesystem::path m_bgmPath{};
  std::filesystem::path m_bgsPath{};
  std::filesystem::path m_sePath{};
  std::filesystem::path m_mePath{};
  std::filesystem::path m_imgPath{};
  std::filesystem::path m_animationsPath{};
  std::filesystem::path m_battlebacks1Path{};
  std::filesystem::path m_battlebacks2Path{};
  std::filesystem::path m_charactersPath{};
  std::filesystem::path m_enemiesPath{};
  std::filesystem::path m_facesPath{};
  std::filesystem::path m_parallaxesPath{};
  std::filesystem::path m_picturesPath{};
  std::filesystem::path m_sv_actorsPath{};
  std::filesystem::path m_sv_enemiesPath{};
  std::filesystem::path m_systemPath{};
  std::filesystem::path m_tilesetsPath{};
  std::filesystem::path m_titles1Path{};
  std::filesystem::path m_titles2Path{};
};