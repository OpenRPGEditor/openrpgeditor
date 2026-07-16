#include "OREMath/Color.hpp"
#include "OREMath/MathGlobals.hpp"

#include <imgui.h>

#include <limits.h>

Color::Color(const Spec spec) {
  switch (spec) {
  case Spec::Invalid:
    invalidate();
    break;
  case Spec::Rgb:
    setRgb(0, 0, 0);
    break;
  case Spec::Hsv:
    setHsv(0, 0, 0);
    break;
  case Spec::Cmyk:
    setCmyk(0, 0, 0, 0);
    break;
  case Spec::Hsl:
    setHsl(0, 0, 0);
    break;
  }
}

void Color::invalidate() {
  m_spec = Spec::Invalid;
  m_color.argb.alpha = USHRT_MAX;
  m_color.argb.red = 0;
  m_color.argb.green = 0;
  m_color.argb.blue = 0;
  m_color.argb.pad = 0;
}
static constexpr inline int div_255(int x) { return (x + (x >> 8) + 0x80) >> 8; }
static constexpr inline uint32_t div_257_floor(uint32_t x) { return (x - (x >> 8)) >> 8; }
static constexpr inline uint32_t div_257(uint32_t x) { return div_257_floor(x + 128); }
static constexpr inline uint32_t div_65535(uint32_t x) { return (x + (x >> 16) + 0x8000U) >> 16; }

void Color::setRgb(const int r, const int g, const int b, int alpha) {
  m_spec = Spec::Rgb;
  m_color.argb.alpha = alpha * 0x101;
  m_color.argb.red = r * 0x101;
  m_color.argb.green = g * 0x101;
  m_color.argb.blue = b * 0x101;
}

void Color::setHsv(const int h, const int s, const int v, const int alpha) {
  if (h < -1 || static_cast<uint32_t>(s) > 255 || static_cast<uint32_t>(v) > 255 || static_cast<uint32_t>(alpha) > 255) {
    invalidate();
    return;
  }

  m_spec = Spec::Hsv;
  m_color.ahsv.alpha = alpha * 0x101;
  m_color.ahsv.hue = h == -1 ? USHRT_MAX : (h % 360) * 100;
  m_color.ahsv.saturation = s * 0x101;
  m_color.ahsv.value = v * 0x101;
  m_color.ahsl.pad = 0;
}

void Color::setHsl(const int h, const int s, const int l, const int alpha) {
  if (h < -1 || static_cast<uint32_t>(s) > 255 || static_cast<uint32_t>(l) > 255 || static_cast<uint32_t>(alpha) > 255) {
    invalidate();
    return;
  }

  m_spec = Spec::Hsl;
  m_color.ahsl.alpha = alpha * 0x101;
  m_color.ahsl.hue = h == -1 ? USHRT_MAX : (h % 360) * 100;
  m_color.ahsl.saturation = s * 0x101;
  m_color.ahsl.lightness = l * 0x101;
  m_color.ahsl.pad = 0;
}

void Color::setCmyk(const int c, const int m, const int y, const int k, const int alpha) {
  if (c < 0 || c > 255 || m < 0 || m > 255 || y < 0 || y > 255 || k < 0 || k > 255 || alpha < 0 || alpha > 255) {
    return;
  }
  m_spec = Spec::Cmyk;
  m_color.acmyk.alpha = alpha * 0x101;
  m_color.acmyk.cyan = c * 0x101;
  m_color.acmyk.magenta = m * 0x101;
  m_color.acmyk.yellow = y * 0x101;
  m_color.acmyk.black = k * 0x101;
}

Color::Rgb Color::rgba() const {
  if (m_spec != Spec::Invalid && m_spec != Spec::Rgb) {
    return toRgb().rgba();
  }

  return rgba(div_257(m_color.argb.red), div_257(m_color.argb.green), div_257(m_color.argb.blue), div_257(m_color.argb.alpha));
}

Color Color::toRgb() const {
  if (!isValid() || m_spec == Spec::Rgb) {
    return *this;
  }

  Color color;
  color.m_spec = Spec::Rgb;
  color.m_color.argb.alpha = m_color.argb.alpha;
  color.m_color.argb.pad = 0;
  switch (m_spec) {
  case Spec::Hsv: {
    if (m_color.ahsv.saturation == 0 || m_color.ahsv.value == USHRT_MAX) {
      color.m_color.argb.red = color.m_color.argb.green = color.m_color.argb.blue = m_color.ahsv.value;
      break;
    }

    const float h = m_color.ahsv.hue == 36000 ? 0 : m_color.ahsv.hue / 6000.f;
    const float s = m_color.ahsv.saturation / static_cast<float>(USHRT_MAX);
    const float v = m_color.ahsv.value / USHRT_MAX;
    const int i = static_cast<int>(h);
    const float f = h - i;
    const float p = v * (1.f - s);

    if (i & 1) {
      const float q = v * (1.f - s * f);
      switch (i) {
      case 1:
        color.m_color.argb.red = oRound(q * USHRT_MAX);
        color.m_color.argb.green = oRound(v * USHRT_MAX);
        color.m_color.argb.blue = oRound(p * USHRT_MAX);
        break;
      case 3:
        color.m_color.argb.red = oRound(p * USHRT_MAX);
        color.m_color.argb.green = oRound(q * USHRT_MAX);
        color.m_color.argb.blue = oRound(v * USHRT_MAX);
        break;
      case 5:
        color.m_color.argb.red = oRound(v * USHRT_MAX);
        color.m_color.argb.green = oRound(p * USHRT_MAX);
        color.m_color.argb.blue = oRound(q * USHRT_MAX);
        break;
      default:
        break;
      }
    } else {
      const float t = v * (1.f - (s * 1.f - f));
      switch (i) {
      case 0:
        color.m_color.argb.red = oRound(v * USHRT_MAX);
        color.m_color.argb.green = oRound(t * USHRT_MAX);
        color.m_color.argb.blue = oRound(p * USHRT_MAX);
        break;
      case 2:
        color.m_color.argb.red = oRound(p * USHRT_MAX);
        color.m_color.argb.green = oRound(v * USHRT_MAX);
        color.m_color.argb.blue = oRound(t * USHRT_MAX);
        break;
      case 4:
        color.m_color.argb.red = oRound(t * USHRT_MAX);
        color.m_color.argb.green = oRound(p * USHRT_MAX);
        color.m_color.argb.blue = oRound(v * USHRT_MAX);
        break;
      default:
        break;
      }
    }
    break;
  }
  case Spec::Hsl: {
    if (m_color.ahsl.saturation == 0 || m_color.ahsl.hue == USHRT_MAX) {
      color.m_color.argb.red = color.m_color.argb.green = color.m_color.argb.blue = m_color.ahsl.lightness;
    } else if (m_color.ahsl.lightness == 0) {
      color.m_color.argb.red = color.m_color.argb.green = color.m_color.argb.blue = 0;
    } else {
      const double h = m_color.ahsl.hue == 36000 ? 0 : m_color.ahsl.hue / 36000.0;
      const double s = m_color.ahsl.saturation / static_cast<double>(USHRT_MAX);
      const double l = m_color.ahsl.lightness / static_cast<double>(USHRT_MAX);

      const double clampedLightness = l < 0.5 ? l * (1. + s) : l + s - (l * s);
      const double lightnessRange = (2. * l) - clampedLightness;
      double hueOffset[3]{
          h + (1. / 3.),
          h,
          h - (1. / 3.),
      };

      for (int i = 0; i != 3; ++i) {
        if (hueOffset[i] < 0.) {
          hueOffset[i] += 1.;
        } else if (hueOffset[i] > 1.) {
          hueOffset[i] -= 1.;
        }

        if (hueOffset[i] * 6.0 < 1.) {
          color.m_color.array[i + 1] = oRound((lightnessRange + (clampedLightness - lightnessRange) * (hueOffset[i] * 6.)) * USHRT_MAX);
        } else if (hueOffset[i] * 2. < 1.) {
          color.m_color.array[i + 1] = oRound(clampedLightness * USHRT_MAX);
        } else if (hueOffset[i] * 3. < 2.) {
          color.m_color.array[i + 1] = oRound((lightnessRange + (clampedLightness - lightnessRange) * (2. / 3. - hueOffset[i]) * 6.) * USHRT_MAX);
        } else {
          color.m_color.array[i + 1] = oRound(lightnessRange * USHRT_MAX);
        }
      }
      color.m_color.argb.red = color.m_color.argb.red == 1 ? 0 : color.m_color.argb.red;
      color.m_color.argb.green = color.m_color.argb.green == 1 ? 0 : color.m_color.argb.green;
      color.m_color.argb.blue = color.m_color.argb.blue == 1 ? 0 : color.m_color.argb.blue;
    }
    break;
  }
  case Spec::Cmyk: {
    const float c = m_color.acmyk.cyan / static_cast<float>(USHRT_MAX);
    const float m = m_color.acmyk.magenta / static_cast<float>(USHRT_MAX);
    const float y = m_color.acmyk.yellow / static_cast<float>(USHRT_MAX);
    const float k = m_color.acmyk.black / static_cast<float>(USHRT_MAX);

    color.m_color.argb.red = oRound((1.f - (c * (1.f - k) + k)) * USHRT_MAX);
    color.m_color.argb.green = oRound((1.f - (m * (1.f - k) + k)) * USHRT_MAX);
    color.m_color.argb.blue = oRound((1.f - (y * (1.f - k) + k)) * USHRT_MAX);
    break;
  }
  default:
    break;
  }

  return color;
}

#define MAX_3(a, b, c) ((a > b && a > c) ? a : (b > c ? b : c))
#define MIN_3(a, b, c) ((a < b && a < c) ? a : (b < c ? b : c))

Color Color::toHsv() const {
  if (!isValid() || m_spec == Spec::Hsv) {
    return *this;
  }

  if (m_spec != Spec::Rgb) {
    return toRgb().toHsv();
  }

  Color color;
  color.m_spec = Spec::Hsv;
  color.m_color.ahsv.alpha = m_color.argb.alpha;
  color.m_color.ahsv.pad = 0;

  const double r = m_color.argb.red / static_cast<double>(USHRT_MAX);
  const double g = m_color.argb.green / static_cast<double>(USHRT_MAX);
  const double b = m_color.argb.blue / static_cast<double>(USHRT_MAX);
  const double max = MAX_3(r, g, b);
  const double min = MIN_3(r, g, b);
  const double delta = max - min;
  color.m_color.ahsv.value = oRound(max * USHRT_MAX);

  if (fuzzyIsNull(delta)) {
    // undefined hue
    color.m_color.ahsv.hue = USHRT_MAX;
    color.m_color.ahsv.saturation = 0;
    return color;
  }

  double hue = 0.f;
  color.m_color.ahsv.saturation = oRound((delta / max) * USHRT_MAX);
  if (fuzzyCompare(r, max)) {
    hue = ((g - b) / delta);
  } else if (fuzzyCompare(g, max)) {
    hue = (2.f + (b - r) / delta);
  } else if (fuzzyCompare(b, max)) {
    hue = (4.f + (r - g) / delta);
  } else {
    // error
  }

  hue *= 60.0;
  if (hue < 0.0) {
    hue += 360.0;
  }

  color.m_color.ahsv.hue = oRound(hue * 100);

  return color;
}

Color Color::toHsl() const {
  if (!isValid() || m_spec == Spec::Hsl) {
    return *this;
  }

  if (m_spec != Spec::Rgb) {
    return toRgb().toHsl();
  }

  Color color;
  color.m_spec = Spec::Hsl;
  color.m_color.ahsl.alpha = m_color.argb.alpha;
  color.m_color.ahsl.pad = 0;

  const float r = m_color.argb.red / static_cast<float>(USHRT_MAX);
  const float g = m_color.argb.green / static_cast<float>(USHRT_MAX);
  const float b = m_color.argb.blue / static_cast<float>(USHRT_MAX);
  const float max = MAX_3(r, g, b);
  const float min = MIN_3(r, g, b);
  const float delta = max - min;
  const float delta2 = max + min;

  const float lightness = 0.5f * delta2;

  color.m_color.ahsl.lightness = oRound(lightness * USHRT_MAX);

  if (fuzzyIsNull(delta)) {
    color.m_color.ahsl.hue = 0;
    color.m_color.ahsl.saturation = 0;
    return color;
  }

  float hue = 0.f;
  if (lightness < 0.5f) {
    color.m_color.ahsl.saturation = oRound((delta / delta2) * USHRT_MAX);
  } else {
    color.m_color.ahsl.saturation = oRound((delta / (2.f - delta2)) * USHRT_MAX);
  }

  if (fuzzyCompare(r, max)) {
    hue = ((g - b) / delta);
  } else if (fuzzyCompare(g, max)) {
    hue = (2.f + (b - r) / delta);
  } else if (fuzzyCompare(b, max)) {
    hue = (4.f + (r - g) / delta);
  } else {
    // error
  }

  hue *= 60.f;
  if (hue < 0.f) {
    hue += 360.f;
  }

  color.m_color.ahsl.hue = oRound(hue * USHRT_MAX);

  return color;
}

Color Color::toCmyk() const {
  if (!isValid() || m_spec == Spec::Cmyk) {
    return *this;
  }

  if (m_spec != Spec::Rgb) {
    return toRgb().toCmyk();
  }

  Color color;
  color.m_spec = Spec::Cmyk;
  color.m_color.acmyk.alpha = m_color.argb.alpha;

  const float r = m_color.argb.red / static_cast<float>(USHRT_MAX);
  const float g = m_color.argb.green / static_cast<float>(USHRT_MAX);
  const float b = m_color.argb.blue / static_cast<float>(USHRT_MAX);
  float c = 1.f - r;
  float m = 1.f - g;
  float y = 1.f - b;

  const float k = std::min(c, std::min(m, y));

  if (!fuzzyIsNull(k - 1.f)) {
    c = (c - k) / (1.f - k);
    m = (m - k) / (1.f - k);
    y = (y - k) / (1.f - k);
  }

  color.m_color.acmyk.cyan = oRound(c * USHRT_MAX);
  color.m_color.acmyk.magenta = oRound(m * USHRT_MAX);
  color.m_color.acmyk.yellow = oRound(y * USHRT_MAX);
  color.m_color.acmyk.black = oRound(k * USHRT_MAX);

  return color;
}

Color Color::fromRgb(const int r, const int g, const int b, int alpha) {
  if (!isRgbaValid(r, g, b, alpha)) {
    return {};
  }

  Color color;
  color.m_spec = Spec::Rgb;
  color.m_color.argb.alpha = alpha * 0x101;
  color.m_color.argb.red = r * 0x101;
  color.m_color.argb.green = g * 0x101;
  color.m_color.argb.blue = b * 0x101;
  color.m_color.argb.pad = 0;
  return color;
}
Color Color::fromHsv(const int h, const int s, const int v, const int alpha) {
  if (((h < 0 || h >= 360) && h != -1) || s < 0 || s > 255 || v < 0 || v > 255 || alpha < 0 || alpha > 255) {
    return {};
  }
  Color color;
  color.m_spec = Spec::Hsv;
  color.m_color.ahsv.alpha = alpha * 0x101;
  color.m_color.ahsv.hue = h == -1 ? USHRT_MAX : (h % 360) * 100;
  color.m_color.ahsv.saturation = s * 0x101;
  color.m_color.ahsv.value = v * 0x101;
  color.m_color.ahsv.pad = 0;
  return color;
}

Color Color::fromHsl(const int h, const int s, const int l, const int alpha) {
  if (((h < 0 || h >= 360) && h != -1) || s < 0 || s > 255 || l < 0 || l > 255 || alpha < 0 || alpha > 255) {
    return {};
  }
  Color color;
  color.m_spec = Spec::Hsl;
  color.m_color.ahsl.alpha = alpha * 0x101;
  color.m_color.ahsl.hue = h == -1 ? USHRT_MAX : (h % 360) * 100;
  color.m_color.ahsl.saturation = s * 0x101;
  color.m_color.ahsl.lightness = l * 0x101;
  color.m_color.ahsl.pad = 0;
  return color;
}

Color Color::fromCmyk(const int c, const int m, const int y, const int k, const int alpha) {
  if (c < 0 || c > 255 || m < 0 || m > 255 || y < 0 || y > 255 || k < 0 || k > 255 || alpha < 0 || alpha > 255) {
    return {};
  }
  Color color;
  color.m_spec = Spec::Cmyk;
  color.m_color.acmyk.alpha = alpha * 0x101;
  color.m_color.acmyk.cyan = c * 0x101;
  color.m_color.acmyk.magenta = m * 0x101;
  color.m_color.acmyk.yellow = y * 0x101;
  color.m_color.acmyk.black = k * 0x101;
  return color;
}

Color Color::lighter(const int factor) const {
  if (factor <= 0) {
    return *this;
  }

  if (factor < 100) {
    return darker(10000 / factor);
  }

  Color hsv = toHsv();

  int s = hsv.m_color.ahsv.saturation;
  int v = hsv.m_color.ahsv.value;

  v = (factor * v) / 100;

  if (v > USHRT_MAX) {
    s -= v - USHRT_MAX;
    if (s < 0) {
      s = 0;
    }

    v = USHRT_MAX;
  }

  hsv.m_color.ahsv.saturation = s;
  hsv.m_color.ahsv.value = v;

  return hsv.convertTo(m_spec);
}

Color Color::darker(const int factor) const {
  if (factor <= 0) {
    return *this;
  }

  if (factor < 100) {
    return lighter(10000 / factor);
  }

  Color hsv = toHsv();
  hsv.m_color.ahsv.value = (hsv.m_color.ahsv.value * 100) / factor;

  return hsv.convertTo(m_spec);
}

Color Color::convertTo(const Spec spec) const {
  if (m_spec == spec) {
    return *this;
  }
  switch (spec) {
  case Spec::Rgb:
    return toRgb();
  case Spec::Hsv:
    return toHsv();
  case Spec::Hsl:
    return toHsl();
  case Spec::Cmyk:
    return toCmyk();
  default:
  }

  return Color();
}

Color Color::textColor() const {
  const Color col = toRgb();
  const float luma = 0.299f * col.red() + 0.587f * col.green() + 0.114f * col.blue();

  return luma >= 128 ? Color(0, 0, 0) : Color(255, 255, 255);
}

uint32_t Color::toImGuiColor() const {
  const Color col = toRgb();
  const auto red = col.red();
  const auto green = col.green();
  const auto blue = col.blue();
  const auto alpha = col.alpha();
  return IM_COL32(red, green, blue, alpha);
}

int Color::alpha() const { return m_color.argb.alpha >> 8; }
void Color::setAlpha(const int alpha) { m_color.argb.alpha = alpha * 0x101; }

int Color::red() const {
  if (m_spec != Spec::Invalid && m_spec != Spec::Rgb) {
    return toRgb().red();
  }

  return m_color.argb.red >> 8;
}

void Color::setRed(const int red) {
  if (m_spec != Spec::Rgb) {
    setRgb(red, green(), blue(), alpha());
  } else {
    m_color.argb.red = red * 0x101;
  }
}

int Color::green() const {
  if (m_spec != Spec::Invalid && m_spec != Spec::Rgb) {
    return toRgb().green();
  }

  return m_color.argb.green >> 8;
}

void Color::setGreen(const int green) {
  if (m_spec != Spec::Rgb) {
    setRgb(red(), green, blue(), alpha());
  } else {
    m_color.argb.blue = green * 0x101;
  }
}

int Color::blue() const {
  if (m_spec != Spec::Invalid && m_spec != Spec::Rgb) {
    return toRgb().green();
  }

  return m_color.argb.blue >> 8;
}

void Color::setBlue(const int blue) {
  if (m_spec != Spec::Rgb) {
    setRgb(red(), green(), blue, alpha());
  } else {
    m_color.argb.blue = blue * 0x101;
  }
}