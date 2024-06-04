#pragma once

#include <string>
#include <array>
#include <vector>
#include <string_view>

#include <nlohmann/json.hpp>

struct Audio {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Audio, name, pan, pitch, volume);
  std::string name; // base name of the file (without extension) in audio/se
  int pan = 0;
  int pitch = 100;
  int volume = 90;
};