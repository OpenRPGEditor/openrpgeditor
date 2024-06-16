#pragma once

#include <vector>
#include <string>
#include <array>

#include <nlohmann/json.hpp>

class Tileset {
public:
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Tileset, id, flags, mode, name, note, tilesetNames);

  enum class Mode {
    World,
    Area,
  };

  enum Flags {
    None = 0,
    Unk1 = 1 << 0,
    Unk2 = 1 << 2,
    Unk3 = 1 << 3,
    HigherTile = 1 << 4,
    Unk5 = 1 << 5,
    Unk6 = 1 << 6,
    IsA2Tile = 1 << 7,
    Unk8 = 1 << 8,
    Unk9 = 1 << 9,
    Unk10 = 1 << 10,
  };

  Tileset();

  int id;
  std::vector<int> flags;
  Mode mode;
  std::string name;
  std::string note;
  std::array<std::string, 9> tilesetNames;

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

class Tilesets {
public:
  static Tilesets load(std::string_view filepath);
  bool serialize(std::string_view filepath);

  [[nodiscard]] Tileset* tileset(int id) {
    for (auto& item : m_tilesets) {
      if (item.id == id && item.m_isValid) {
        return &item;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Tileset* tileset(int id) const {
    for (const auto& set : m_tilesets) {
      if (set.id == id && item.m_isValid) {
        return &set;
      }
    }

    return nullptr;
  }

private:
  std::vector<Tileset> m_tilesets;
};
