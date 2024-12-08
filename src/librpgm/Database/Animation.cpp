#include "Animation.hpp"
#include <iostream>

void to_json(nlohmann::ordered_json& to, const Animation::Timing& timing) {
  to = {
      {"flashColor", timing.flashColor}, {"flashDuration", timing.flashDuration}, {"flashScope", timing.flashScope}, {"frame", timing.frame}, {"se", timing.se},
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
      {"id", animation.m_id},
      {"animation1Hue", animation.m_animation1Hue},
      {"animation1Name", animation.m_animation1Name},
      {"animation2Hue", animation.m_animation2Hue},
      {"animation2Name", animation.m_animation2Name},
      {"frames", animation.m_frames},
      {"name", animation.m_name},
      {"position", animation.m_position},
      {"timings", animation.m_timings},
  };
}
void from_json(const nlohmann::ordered_json& from, Animation& animation) {
  animation.m_id = from.value("id", animation.m_id);
  animation.m_animation1Hue = from.value("animation1Hue", animation.m_animation1Hue);
  animation.m_animation1Name = from.value("animation1Name", animation.m_animation1Name);
  animation.m_animation2Hue = from.value("animation2Hue", animation.m_animation2Hue);
  animation.m_animation2Name = from.value("animation2Name", animation.m_animation2Name);
  animation.m_frames = from.value("frames", animation.m_frames);
  animation.m_name = from.value("name", animation.m_name);
  animation.m_position = from.value("position", animation.m_position);
  animation.m_timings = from.value("timings", animation.m_timings);
}