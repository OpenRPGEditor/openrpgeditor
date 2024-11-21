#include "Animation.hpp"
#include <iostream>

void to_json(nlohmann::ordered_json& to, const Animation::Timing& timing) {
  to = {
      {"flashColor", timing.flashColor},
      {"flashDuration", timing.flashDuration},
      {"flashScope", timing.flashScope},
      {"frame", timing.frame},
      {"se", timing.se},
  };
}

void from_json(const nlohmann::ordered_json& from, Animation::Timing& timing) {
  from.at("flashColor").get_to(timing.flashColor);
  from.at("flashDuration").get_to(timing.flashDuration);
  from.at("flashScope").get_to(timing.flashScope);
  from.at("frame").get_to(timing.frame);
  from.at("se").get_to(timing.se);
}

void to_json(nlohmann::ordered_json& to, const Animation& animation) {
  to = {
      {"id", animation.id},
      {"animation1Hue", animation.animation1Hue},
      {"animation1Name", animation.animation1Name},
      {"animation2Hue", animation.animation2Hue},
      {"animation2Name", animation.animation2Name},
      {"frames", animation.frames},
      {"name", animation.name},
      {"position", animation.position},
      {"timings", animation.timings},
  };
}
void from_json(const nlohmann::ordered_json& from, Animation& animation) {
  animation.id = from.value("id", animation.id);
  animation.animation1Hue = from.value("animation1Hue", animation.animation1Hue);
  animation.animation1Name = from.value("animation1Name", animation.animation1Name);
  animation.animation2Hue = from.value("animation2Hue", animation.animation2Hue);
  animation.animation2Name = from.value("animation2Name", animation.animation2Name);
  animation.frames = from.value("frames", animation.frames);
  animation.name = from.value("name", animation.name);
  animation.position = from.value("position", animation.position);
  animation.timings = from.value("timings", animation.timings);
}