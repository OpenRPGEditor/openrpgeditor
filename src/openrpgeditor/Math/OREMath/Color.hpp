#pragma once
#include <cstdint>

struct Color {
  using Rgb = uint32_t;
  using Rgb64 = uint64_t;
  enum class Spec {
    Invalid,
    Rgb,
    Hsv,
    Cmyk,
    Hsl,
  };

  Color() = default;
  Color(int r, int g, int b, int a = 255);

  static int alpha(const Rgb rgb) { return (rgb >> 24) & 0xff; }
  static int red(const Rgb rgb) { return (rgb >> 16) & 0xff; }
  static int green(const Rgb rgb) { return (rgb >> 8) & 0xff; }
  static int blue(const Rgb rgb) { return rgb & 0xff; }
  static Rgb rgb(const int r, const int g, const int b) {
    return (255 << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
  }
  static Rgb rgba(const int r, const int g, const int b, const int a) {
    return ((a & 0xFF) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
  }

  static int gray(const int r, const int g, const int b) { return (r * 11 + g * 16 + b * 5) / 32; }
  static int gray(const Rgb rgb) { return gray(red(rgb), green(rgb), blue(rgb)); }
  static bool isGray(const Rgb rgb) { return red(rgb) == green(rgb) && red(rgb) == blue(rgb); }
};
