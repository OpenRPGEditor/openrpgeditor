#pragma once
#include <cstdint>
#include <sys/types.h>

struct Color {
  using Rgb = uint32_t;
  enum class Spec {
    Invalid,
    Rgb,
    Hsv,
    Cmyk,
    Hsl,
  };

  Color() = default;
  explicit Color(Spec spec);
  Color(const int r, const int g, const int b, const int alpha = 255) { setRgb(r, g, b, alpha); }
  void setRgb(int r, int g, int b, int alpha = 255);
  [[nodiscard]] Rgb rgba() const;
  void setHsv(int h, int s, int v, int alpha = 255);
  void setHsl(int h, int s, int v, int alpha = 255);
  void setCmyk(int c, int m, int y, int k, int alpha = 255);
  void invalidate();

  [[nodiscard]] Color toRgb() const;

  static Color fromRgb(int r, int g, int b, int alpha = 255);
  static Color fromHsv(int h, int s, int v, int alpha = 255);
  static Color fromHsl(int h, int s, int l, int alpha = 255);
  static Color fromCmyk(int c, int m, int y, int k, int alpha = 255);

  static int alpha(const Rgb rgb) { return (rgb >> 24) & 0xff; }
  static int red(const Rgb rgb) { return (rgb >> 16) & 0xff; }
  static int green(const Rgb rgb) { return (rgb >> 8) & 0xff; }
  static int blue(const Rgb rgb) { return rgb & 0xff; }
  static Rgb rgb(const int r, const int g, const int b) { return (255 << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff); }
  static Rgb rgba(const int r, const int g, const int b, const int a) { return ((a & 0xFF) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff); }

  static int gray(const int r, const int g, const int b) { return (r * 11 + g * 16 + b * 5) / 32; }
  static int gray(const Rgb rgb) { return gray(red(rgb), green(rgb), blue(rgb)); }
  static bool isGray(const Rgb rgb) { return red(rgb) == green(rgb) && red(rgb) == blue(rgb); }

  bool isValid() const { return m_spec != Spec::Invalid; }
  static constexpr bool isRgbaValid(const int r, const int g, const int b, const int a = 255) {
    return static_cast<uint32_t>(r) <= 255 && static_cast<uint32_t>(g) <= 255 && static_cast<uint32_t>(b) <= 255 && static_cast<uint32_t>(a) <= 255;
  }

private:
  Spec m_spec{};
  union {
    struct {
      uint16_t alpha;
      uint16_t red;
      uint16_t green;
      uint16_t blue;
      uint16_t pad;
    } argb;
    struct {
      uint16_t alpha;
      uint16_t hue;
      uint16_t saturation;
      uint16_t value;
      uint16_t pad;
    } ahsv;
    struct {
      uint16_t alpha;
      uint16_t cyan;
      uint16_t magenta;
      uint16_t yellow;
      uint16_t black;
    } acmyk;
    struct {
      uint16_t alpha;
      uint16_t hue;
      uint16_t saturation;
      uint16_t lightness;
      uint16_t pad;
    } ahsl;
    uint16_t array[5];
  } m_color{};
};
