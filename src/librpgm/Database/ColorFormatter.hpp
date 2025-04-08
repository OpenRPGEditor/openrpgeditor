#ifndef COLOR_FORMATTER_H
#define COLOR_FORMATTER_H

#include "Globals.hpp"

#include <string>

struct ColorFormatter {
  static std::string getColorCode(EventCode command, bool color = true);
  static std::string popColor(bool color = true);
  static std::string getColor(FormatColor preset, bool color = true);
};

#endif // COLOR_FORMATTER_H