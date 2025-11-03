#include "Editor/MapTools/PenMapTool.hpp"

#include "Database/Database.hpp"
#include "Editor/ResourceManager.hpp"

std::vector<int> PenMapTool::execute(int layer, const std::vector<std::array<int, 4>>& penData) const {
  if (m_palettePenSize.isNull() || !m_palettePenSize.isValid()) {
    return {};
  }

  if (layer == -1) {
    layer = 0;
  }
  if (layer > 4) {
    layer = 3;
  }

  std::vector result(width() * height() * 6, -1);

  for (int y = 0; y < height(); y++) {
    for (int x = 0; x < width(); x++) {
      for (int l = layer; l < 4; l++) {
        result[(l * height() + y) * width() + x] = penData[(y * width() + x)][l];
      }
    }
  }

  return result;
}
