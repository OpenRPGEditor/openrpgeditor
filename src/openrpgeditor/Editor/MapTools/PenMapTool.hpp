#pragma once

#include "Editor/MapTools/IMapTool.hpp"
#include "IconsFontAwesome6.h"

class PenMapTool final : public ITypedMapTool<PenMapTool> {
public:
  [[nodiscard]] std::vector<int> execute(int layer, const std::vector<std::array<int, 4>>& penData) const override;
  [[nodiscard]] int width() const override { return m_palettePenSize.width(); }
  [[nodiscard]] int height() const override { return m_palettePenSize.height(); }
  [[nodiscard]] const Size& size() const override { return m_palettePenSize; }
  void setSize(const Size& size) override {}

  [[nodiscard]] std::string description() const override { return trNOOP("Modifies map tiles on a tile by tile basis, AutoTiles will automatically update their neighboring tiles."); }
  [[nodiscard]] std::string name() const override { return trNOOP("Pen Tool"); }
  [[nodiscard]] std::string fontIcon() const override { return ICON_FA_PENCIL; }

  void resetDefaults() override {}
};
