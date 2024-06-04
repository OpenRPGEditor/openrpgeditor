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
    HigherTile = 1 << 4,
    IsA2Tile = 1 << 7,
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
