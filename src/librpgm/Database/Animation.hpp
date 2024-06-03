#pragma once

#include "Database/RPGEnums.hpp"

#include <string>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>

class Animation {
public:
  friend void to_json(nlohmann::json& json, const Animation& animation);
  friend void from_json(const nlohmann::json& json, Animation& animation);

  struct Color {
    friend void to_json(nlohmann::json& json, const Color& color);
    friend void from_json(const nlohmann::json& json, Color& color);
    int r;
    int g;
    int b;
    int intensity;
  };

  struct FramePart {
    friend void to_json(nlohmann::json& json, const FramePart& frame);
    friend void from_json(const nlohmann::json& json, FramePart& frame);
    int pattern;
    int x;
    int y;
    int scale;
    int rotation;
    Mirror mirror;
    int opacity;
    Blend blend;
  };

  struct Frame {
    friend void to_json(nlohmann::json& json, const Frame& frame);
    friend void from_json(const nlohmann::json& json, Frame& frame);

    std::vector<FramePart> frameParts;
  };

  struct SoundEffect {
    friend void to_json(nlohmann::json& json, const SoundEffect& frame);
    friend void from_json(const nlohmann::json& json, SoundEffect& frame);
    SoundEffect() : name(""), pan(0), pitch(0), volume(0) {}
    std::string name; // base name of the file (without extension) in audio/se
    int pan;
    int pitch;
    int volume;
  };

  struct Timing {
    friend void to_json(nlohmann::json& json, const Timing& timing);
    friend void from_json(const nlohmann::json& json, Timing& timing);
    Color flashColor;
    int flashDuration;
    FlashScope flashScope;
    int frame;
    std::optional<SoundEffect> se;
  };

  int id = 0;
  int animation1Hue = 0;
  std::string animation1Name;
  int animation2Hue = 0;
  std::string animation2Name;
  std::vector<Frame> frames;
  std::string name;
  Position position = Position::Head;
  std::vector<Timing> timings;
};
