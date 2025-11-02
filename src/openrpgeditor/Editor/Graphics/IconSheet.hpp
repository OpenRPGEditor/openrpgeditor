#pragma once
#include <string_view>

#include "Editor/Graphics/SimpleRect.hpp"
#include "Editor/Graphics/Texture.hpp"

class IconSheet {
public:
  explicit IconSheet(std::string_view sheetName);

  [[nodiscard]] static constexpr int iconWidth() { return 32; }
  [[nodiscard]] static constexpr int iconHeight() { return 32; }
  [[nodiscard]] static constexpr int columnCount() { return 16; }

  [[nodiscard]] SimpleRect rectForId(int id) const;

  const Texture& texture() const { return m_texture; }

private:
  Texture m_texture;
};
