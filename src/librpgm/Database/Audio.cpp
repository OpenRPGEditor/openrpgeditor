#include "Database/Audio.hpp"

void to_json(nlohmann::ordered_json& out, const Audio& audio) {
  out = {
      {"name", audio.name},
      {"pan", audio.pan},
      {"pitch", audio.pitch},
      {"volume", audio.volume},
  };
}

void from_json(const nlohmann::ordered_json& from, Audio& audio) {
  audio.name = from.value("name", audio.name);
  audio.pan = from.value("pan", audio.pan);
  audio.pitch = from.value("pitch", audio.pitch);
  audio.volume = from.value("volume", audio.volume);
}