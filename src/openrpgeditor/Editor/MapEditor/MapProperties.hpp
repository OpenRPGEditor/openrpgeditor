#pragma once
#include "Database/Map.hpp"
#include "Editor/CommonUI/AudioEditor.hpp"
#include "Editor/CommonUI/IDialogController.hpp"
#include "Editor/CommonUI/ImagePicker.hpp"
#include "Editor/CommonUI/ObjectPicker.hpp"

class Map;
class MapInfo;
class MapProperties final : public IDialogController {
public:
  MapProperties()
  : IDialogController("Map Properties") {}

  void setMapInfo(MapInfo* mapInfo);
  std::tuple<bool, bool> draw() override;

  [[nodiscard]] Map* map() const;

private:
  void drawPickers();
  void applyChanges();
  MapInfo* m_mapInfo{nullptr};
  std::string m_mapName;
  std::string m_displayName;
  std::string m_note;
  int m_tileset = 1;
  int m_width = 17;
  int m_height = 13;
  ScrollType m_scrollType = ScrollType::No_Loop;
  int m_encounterSteps = 30;
  bool m_autoplayBgm = false;
  Audio m_bgm;
  bool m_autoplayBgs = false;
  Audio m_bgs;
  bool m_specifyBattleback = false;
  std::string m_battleBack1;
  std::string m_battleBack2;
  bool m_disableDashing = false;
  std::string m_parallax;
  bool m_parallaxLoopX = false;
  int m_parallaxSx = 0;
  bool m_parallaxLoopY = false;
  int m_parallaxSy = 0;
  bool m_parallaxShow = true;
  std::vector<Map::Encounter> m_encounters;

  std::optional<TilesetPicker> m_tilesetPicker;
  std::optional<ImagePicker> m_imagePicker;
  std::optional<AudioEditor> m_soundPicker;
};
