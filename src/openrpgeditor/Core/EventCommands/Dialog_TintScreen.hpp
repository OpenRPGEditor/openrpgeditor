#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/TintScreen.hpp"
#include <algorithm>

struct Dialog_TintScreen : IEventDialogController {
  Dialog_TintScreen() = delete;
  explicit Dialog_TintScreen(const std::string& name,
                             const std::shared_ptr<TintScreenCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new TintScreenCommand());
    }
    r = command->color.r;
    g = command->color.g;
    b = command->color.b;
    gray = command->color.gray;
    m_duration = command->duration;
    m_waitForCompletion = command->waitForCompletion;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int r;
  int g;
  int b;
  int gray;
  int m_duration;
  bool m_waitForCompletion;

  bool m_confirmed{false};
  std::shared_ptr<TintScreenCommand> command;
  std::tuple<bool, bool> result;

  std::array<float, 3> rgbToHsv(int r, int g, int b) {
    // Convert RGB values from 0-255 to 0-1 range

    float rf = r / 255.0f;
    float gf = g / 255.0f;
    float bf = b / 255.0f;
    //float cmax = std::max(rf, std::max(gf, bf));
    float cmax = 0.f;
    float cmin = 0.f;
    //float cmin = std::min(rf, std::min(gf, bf));
    float delta = cmax - cmin;

    float h = 0.0f;
    float s = 0.0f;
    float v = cmax;

    // Calculate hue
    if (delta != 0.0f) {
      if (cmax == rf)
        h = 60.0f * fmod(((gf - bf) / delta), 6.0f);
      else if (cmax == gf)
        h = 60.0f * (((bf - rf) / delta) + 2.0f);
      else // cmax == bf
        h = 60.0f * (((rf - gf) / delta) + 4.0f);
    }

    // Calculate saturation
    if (cmax != 0.0f)
      s = delta / cmax;

    // Create and return the HSV array
    std::array<float, 3> hsv = {h, s, v};
    return hsv;
  }
  std::array<int, 3> hsvToRgb(float h, float s, float v) {
    // Initialize output array for RGB values
    std::array<int, 3> rgb;

    if (s == 0.0f) {
      // If saturation is 0, the color is grayscale
      rgb[0] = rgb[1] = rgb[2] = static_cast<int>(v * 255);
    } else {
      h /= 60.0f;
      int i = static_cast<int>(std::floor(h));
      float f = h - i;
      float p = v * (1.0f - s);
      float q = v * (1.0f - s * f);
      float t = v * (1.0f - s * (1.0f - f));

      switch (i) {
      case 0:
        rgb[0] = static_cast<int>(v * 255);
        rgb[1] = static_cast<int>(t * 255);
        rgb[2] = static_cast<int>(p * 255);
        break;
      case 1:
        rgb[0] = static_cast<int>(q * 255);
        rgb[1] = static_cast<int>(v * 255);
        rgb[2] = static_cast<int>(p * 255);
        break;
      case 2:
        rgb[0] = static_cast<int>(p * 255);
        rgb[1] = static_cast<int>(v * 255);
        rgb[2] = static_cast<int>(t * 255);
        break;
      case 3:
        rgb[0] = static_cast<int>(p * 255);
        rgb[1] = static_cast<int>(q * 255);
        rgb[2] = static_cast<int>(v * 255);
        break;
      case 4:
        rgb[0] = static_cast<int>(t * 255);
        rgb[1] = static_cast<int>(p * 255);
        rgb[2] = static_cast<int>(v * 255);
        break;
      default: // case 5:
        rgb[0] = static_cast<int>(v * 255);
        rgb[1] = static_cast<int>(p * 255);
        rgb[2] = static_cast<int>(q * 255);
        break;
      }
    }

    return rgb;
  }
};