#pragma once

#include "TileHelper.hpp"

#include <array>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

class Tileset {
public:
  friend class TilesetsSerializer;
  friend void to_json(nlohmann::ordered_json& j, const Tileset& tileset);
  friend void from_json(const nlohmann::ordered_json& j, Tileset& tileset);
  enum class Mode {
    World,
    Area,
  };

  [[nodiscard]] int id() const { return m_id; }
  void setId(const int id) { m_id = id; }

  [[nodiscard]] const std::vector<std::optional<int>>& flags() const { return m_flags; }
  int flag(const int id) {
    if (id > TileHelper::TILE_ID_MAX) {
      return 0;
    }
    if (id >= m_flags.size() && m_flags.size() <= TileHelper::TILE_ID_MAX) {
      int i = id - m_flags.size();
      while (i--) {
        m_flags.emplace_back();
      }
      m_flags.back() = 0;
    }

    if (id >= m_flags.size()) {
      return 0;
    }
    if (!m_flags[id]) {
      m_flags[id] = 0;
    }
    return *m_flags[id];
  }

  [[nodiscard]] int flag(const int idx) const {
    if (idx > TileHelper::TILE_ID_MAX) {
      return 0;
    }

    if (idx >= 0 && idx < m_flags.size()) {
      return m_flags[idx] ? *m_flags[idx] : 0;
    }
    return 0;
  }

  void setFlags(const std::vector<std::optional<int>>& flags) { m_flags = flags; }
  void setFlag(const int idx, const int flag, const bool enabled = true) {
    if (idx >= m_flags.size() && m_flags.size() <= TileHelper::TILE_ID_MAX) {
      int i = idx - m_flags.size();
      while (i--) {
        m_flags.emplace_back();
      }
    } else if (idx > TileHelper::TILE_ID_MAX) {
      return;
    }
    if (!m_flags[flag]) {
      m_flags[idx] = 0;
    }

    if (enabled) {
      *m_flags[idx] |= flag;
    } else {
      *m_flags[idx] &= ~flag;
    }
  }

  [[nodiscard]] Mode mode() const { return m_mode; }
  void setMode(const Mode mode) { m_mode = mode; }

  [[nodiscard]] const std::string& name() const { return m_name; }
  void setName(const std::string& name) { m_name = name; }

  [[nodiscard]] const std::string& note() const { return m_note; }
  void setNote(const std::string& note) { m_note = note; }

  [[nodiscard]] const std::array<std::string, 9>& tilesetNames() const { return m_tilesetNames; }
  void setTilesetNames(const std::array<std::string, 9>& tilesetNames) { m_tilesetNames = tilesetNames; }
  [[nodiscard]] const std::string& tilesetName(const int idx) const {
    assert(idx >= 0 && idx < m_tilesetNames.size());
    return m_tilesetNames[idx];
  }
  void setTilesetName(const int idx, const std::string& name) {
    assert(idx >= 0 && idx < m_tilesetNames.size());
    m_tilesetNames[idx] = name;
  }

  [[nodiscard]] bool isValid() const { return m_isValid; }
  void setValid(const bool isValid) { m_isValid = isValid; }

private:
  int m_id;
  std::vector<std::optional<int>> m_flags;
  Mode m_mode;
  std::string m_name;
  std::string m_note;
  std::array<std::string, 9> m_tilesetNames;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   *
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};
void to_json(nlohmann::json& j, const Tileset& tileset);
void from_json(const nlohmann::json& j, Tileset& tileset);