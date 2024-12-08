#pragma once

#include "Database/Tileset.hpp"

class Tilesets {
public:
  [[nodiscard]] Tileset* tileset(const int id) {
    for (auto& item : m_tilesets) {
      if (item.id() == id && item.isValid()) {
        return &item;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Tileset* tileset(const int id) const {
    for (const auto& item : m_tilesets) {
      if (item.id() == id && item.isValid()) {
        return &item;
      }
    }

    return nullptr;
  }

  [[nodiscard]] std::vector<Tileset>& tilesets() { return m_tilesets; }
  [[nodiscard]] const std::vector<Tileset>& tilesets() const { return m_tilesets; }
  [[nodiscard]] int count() const { return static_cast<int>(m_tilesets.size() - 1); }

  void resize(int newSize) {
    ++newSize;
    const int oldSize = static_cast<int>(m_tilesets.size());
    m_tilesets.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_tilesets.size(); ++i) {
        m_tilesets[i].setId(i);
      }
    }
  }

private:
  std::vector<Tileset> m_tilesets;
};
