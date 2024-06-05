#pragma once

#include <string_view>

class Texture {
  friend class ResourceManager;
public:
  Texture()=default;


  [[nodiscard]] void* get() const { return m_texture; }
  [[nodiscard]] int width() const { return m_width; }
  [[nodiscard]] int height() const { return m_height; }
private:
  explicit Texture(std::string_view filename);
  void* m_texture{};
  int m_width{};
  int m_height{};
};