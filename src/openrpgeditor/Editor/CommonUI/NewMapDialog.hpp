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
 
  void accept() override {
    ITypedDialogController::accept();
    // Reset to initial state();
    m_newMapId = -1;
    m_mapName.clear();
    m_displayName.clear();
    m_tileset = 1;
    m_width = 17;
    m_height = 13;
    m_scrollType = ScrollType::No_Loop;
    m_encounterSteps = 30;
    m_autoplayBgm = false;
    m_bgm = Audio();
    m_autoplayBgs = false;
    m_bgs = Audio();
    m_autoplayBgs = false;
    m_specifyBattleback = false;
    m_battleBack1.clear();
    m_battleBack2.clear();
    m_disableDashing = false;
    m_parallax.clear();
    m_loopHorizontally = false;
    m_scrollX = 0;
    m_loopVertically = false;
    m_scrollY = 0;
    m_showParallaxInEditor = true;
  }
  
  int newMapId() const { return m_newMapId; }  
  void setNewMapId(const int id) {
    m_newMapId = id;
    if (m_newMapId > 0) {
      m_mapName = std::format("MAP{:04}", m_newMapId);
      m_displayName.clear();
    }
  }
  
  const std::string& mapName() const { return m_mapName; }
  const std::string& displayName() const { return m_displayName; }
  int tileset() const { return m_tileset; }
  int width() const { return m_width; }
  int height() const { return m_height; }
  ScrollType scrollType() const { return m_scrollType; }
  int encounterSteps() const { return m_encounterSteps; }
  bool autoplayBgm() const { return m_autoplayBgm; }
  const Audio& bgm() const { return m_bgm; }
  bool autoplayBgs() const { return m_autoplayBgs; }
  const Audio& bgs() const { return m_bgs; }
  bool specifyBattleback() const { return m_specifyBattleback; }
  const std::string& battleback1() const { return m_battleBack1; }
  const std::string& battleback2() const { return m_battleBack2; }
  bool disableDashing() const { return m_disableDashing; }
  const std::string& parallax() const { return m_parallax; }
  bool loopHorizontally() const { return m_loopHorizontally; }
  int scrollX() const { return m_scrollX; }
  bool loopVertically() const { return m_loopVertically; }
  int scrollY() const { return m_scrollY; }
  bool showParallaxInEditor() const { return m_showParallaxInEditor; }

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
  bool m_specifyBattleback = false;
  std::string m_battleBack1;
  std::string m_battleBack2;
  bool m_disableDashing = false;
  std::string m_parallax;
  bool m_loopHorizontally = false;
  int m_scrollX = 0;
  bool m_loopVertically = false;
  int m_scrollY = 0;
  bool m_showParallaxInEditor = true;


  std::optional<TilesetPicker> m_tilesetPicker;
  std::optional<ImagePicker> m_imagePicker;
  std::optional<AudioEditor> m_soundPicker;
};
