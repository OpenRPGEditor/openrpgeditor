#pragma once
#include "Database/MapInfo.hpp"
#include "Editor/CommonUI/AudioEditor.hpp"
#include "Editor/CommonUI/IDialogController.hpp"
#include "Editor/CommonUI/ImagePicker.hpp"
#include "Editor/CommonUI/ObjectPicker.hpp"

#include <format>
#include <string>

class NewMapDialog final : public ITypedDialogController<NewMapDialog> {
public:
  void drawPickers();
  std::tuple<bool, bool> draw() override;

  void setNewMapId(int id) {
    m_newMapId = id;
    if (m_newMapId > 0) {
      m_mapName = std::format("MAP{:04}", m_newMapId);
      m_displayName.clear();
    }
  }

private:
  int m_newMapId = -1;
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
  bool m_specifyBattleBack = false;
  std::string m_battleBack1;
  std::string m_battleBack2;
  bool m_disableDashing = false;
  std::string m_parallax;
  bool m_loopHorizontally = false;
  int m_scrollX = 0;
  bool m_loopVertically = false;
  int m_scrollY = 0;
  bool m_showParallaxInEditor = true;
  std::vector<Map::Encounter> m_encounters;


  std::optional<TilesetPicker> m_tilesetPicker;
  std::optional<ImagePicker> m_imagePicker;
  std::optional<AudioEditor> m_soundPicker;
};
