#pragma once

#include "Database/Globals.hpp"
#include "Database/Audio.hpp"

#include <string>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>

class Animation {
public:
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Animation, id, animation1Hue, animation1Name, animation2Hue, animation2Name, frames,
                                 name, position, timings);
  struct Color {
    int r;
    int g;
    int b;
    int intensity;
  };

  struct FramePart {
    int pattern;
    int x;
    int y;
    int scale;
    int rotation;
    Mirror mirror;
    int opacity;
    Blend blend;
  };

  struct Timing {
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Timing, flashColor, flashDuration, flashScope, frame, se);
    std::array<int, 4> flashColor{};
    int flashDuration{};
    FlashScope flashScope = FlashScope{};
    int frame{};
    std::optional<Audio> se{};
  };

  int id = 0;
  int animation1Hue = 0;
  std::string animation1Name;
  int animation2Hue = 0;
  std::string animation2Name;
  std::vector<std::vector<std::array<int, 8>>> frames;
  std::string name;
  Position position = Position::Head;
  std::vector<Timing> timings;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};
