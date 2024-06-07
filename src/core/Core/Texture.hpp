#pragma once

#include <string_view>

class Texture {
  friend class ResourceManager;

public:
  Texture() = default;
  Texture(const Texture& other) {
    m_texture = other.m_texture;
    m_width = other.m_width;
    m_height = other.m_height;
  }

  Texture& operator=(const Texture& other) {
    m_texture = other.m_texture;
    m_width = other.m_width;
    m_height = other.m_height;
    return *this;
  }

  [[nodiscard]] void* get() const { return m_texture; }
  [[nodiscard]] int width() const { return m_width; }
  [[nodiscard]] int height() const { return m_height; }

  explicit operator bool() const { return m_texture != nullptr; }

private:
  explicit Texture(std::string_view filename);
  void* m_texture{};
  int m_width{};
  int m_height{};
};