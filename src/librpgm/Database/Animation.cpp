#include "Animation.hpp"

#include <iostream>

using json = nlohmann::json;

template <typename T>
void to_json(nlohmann::json& j, const std::optional<T>& opt) {
  if (opt) {
    j = *opt;
  } else {
    j = nullptr;
  }
}

template <typename T>
void from_json(const nlohmann::json& j, std::optional<T>& opt) {
  if (j.is_null()) {
    opt = std::nullopt;
  } else {
    opt = j.get<T>();
  }
}

void to_json(nlohmann::json& j, const Animation::Color& color) {
  j = json{
      color.r,
      color.g,
      color.b,
      color.intensity,
  };
}
void from_json(const nlohmann::json& j, Animation::Color& color) {
  std::array<int, 4> values{0};
  size_t i = 0;
  for (const auto& v : j) {
    values[i] = v;
    ++i;
  }

  color.r = values[0];
  color.g = values[1];
  color.b = values[2];
  color.intensity = values[3];
}

void to_json(nlohmann::json& j, const Animation::FramePart& part) {
  j = json{
      part.pattern, part.x, part.y, part.scale, part.rotation, part.mirror, part.opacity, part.blend,
  };
}

void to_json(nlohmann::json& j, const Animation::SoundEffect& se) {
  std::string name;
  if (!se.name.empty() && se.name != "") {
    name = se.name;
  }

  j = json{
      {"name", name},
      {"pan", se.pan},
      {"pitch", se.pitch},
      {"volume", se.volume},
  };
}
void from_json(const nlohmann::json& j, Animation::SoundEffect& se) {
  j.at("name").get_to(se.name);
  j.at("pan").get_to(se.pitch);
  j.at("pitch").get_to(se.pitch);
  j.at("volume").get_to(se.volume);
}

void from_json(const nlohmann::json& j, Animation::FramePart& part) {
  auto parameters = j.get<std::array<int, 8>>();

  part.pattern = parameters[0];
  part.x = parameters[1];
  part.y = parameters[2];
  part.scale = parameters[3];
  part.rotation = parameters[4];
  part.mirror = static_cast<Mirror>(parameters[5]);
  part.opacity = parameters[6];
  part.blend = static_cast<Blend>(parameters[7]);
}

void to_json(nlohmann::json& j, const Animation::Frame& frame) {
  for (const auto& part : frame.frameParts) {
    j.push_back(part);
  }
}
void from_json(const nlohmann::json& j, Animation::Frame& frame) { j.get_to(frame.frameParts); }

void to_json(nlohmann::json& j, const Animation::Timing& timing) {
  j = json{
      {"flashColor", timing.flashColor},
      {"flashDuration", timing.flashDuration},
      {"flashScope", timing.flashScope},
      {"frame", timing.frame},
      {"se", timing.se},
  };
}
void from_json(const nlohmann::json& j, Animation::Timing& timing) {
  j.at("flashColor").get_to(timing.flashColor);
  j.at("flashDuration").get_to(timing.flashDuration);
  j.at("flashScope").get_to(timing.flashScope);
  j.at("se").get_to(timing.se);
}

void to_json(nlohmann::json& j, const Animation& animation) {
  j = json{
      {"animation1Hue", animation.animation1Hue},
      {"animation1Name", animation.animation1Name},
      {"animation2Hue", animation.animation2Hue},
      {"animation2Name", animation.animation2Name},
      {"frames", animation.frames},
      {"name", animation.name},
      {"position", animation.position},
      {"timings", animation.timings},
      {"id", animation.id},
  };
}

void from_json(const nlohmann::json& j, Animation& animation) {
  j.at("id").get_to(animation.id);
  j.at("animation1Hue").get_to(animation.animation1Hue);
  j.at("animation1Name").get_to(animation.animation1Name);
  j.at("animation2Hue").get_to(animation.animation2Hue);
  j.at("animation2Name").get_to(animation.animation2Name);
  j.at("frames").get_to(animation.frames);
  j.at("name").get_to(animation.name);
  j.at("position").get_to(animation.position);
  j.at("timings").get_to(animation.timings);
}