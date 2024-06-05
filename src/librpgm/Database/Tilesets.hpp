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
};

class Tilesets {
public:
  static Tilesets load(std::string_view filepath);
  bool serialize(std::string_view filepath);

  // private:
  std::vector<Tileset> m_tilesets;
};
