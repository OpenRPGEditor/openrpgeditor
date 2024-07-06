#include "Audio.hpp"

#include <iostream>

void to_json(nlohmann::json& out, const Audio& audio) {
  out = {
      {"name", audio.name},
      {"volume", audio.volume},
      {"pitch", audio.pitch},
      {"pan", audio.pan},
  };
}

void from_json(const nlohmann::json& in, Audio& audio) {
  std::cout << in << std::endl;
  in.at("name").get_to(audio.name);
  in.at("volume").get_to(audio.volume);
  in.at("pitch").get_to(audio.pitch);
  in.at("pan").get_to(audio.pan);
}