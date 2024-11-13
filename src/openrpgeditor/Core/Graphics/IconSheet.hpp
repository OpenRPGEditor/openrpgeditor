#pragma once
#include <string_view>

#include "Core/Graphics/SimpleRect.hpp"
#include "Core/Graphics/Texture.hpp"

class IconSheet {
public:
  IconSheet() = delete;
  explicit IconSheet(std::string_view gridAtlasPath);

  [[nodiscard]] static constexpr int iconWidth() { return 32; }
  [[nodiscard]] static constexpr int iconHeight() { return 32; }
  [[nodiscard]] static constexpr int columnCount() { return 16; }

  [[nodiscard]] SimpleRect rectForId(int id) const;

  const Texture& texture() const { return m_texture; }

private:
  Texture m_texture;
};
