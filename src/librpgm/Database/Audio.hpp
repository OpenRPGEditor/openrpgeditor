#pragma once

#include <string>
#include <array>
#include <vector>
#include <string_view>

#include "nlohmann/json.hpp"

struct Audio {
  friend void to_json(nlohmann::json& out, const Audio& audio);
  friend void from_json(const nlohmann::json& in, Audio& audio);
  // NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Audio, name, volume, pitch, pan);
  std::string name; // base name of the file (without extension) in audio/se
  int pan = 0;
  int pitch = 100;
  int volume = 100;
};