#ifndef COLOR_FORMATTER_H
#define COLOR_FORMATTER_H

#include "Globals.hpp"

#include <string>

struct ColorFormatter {
  static std::string getColorCode(EventCode command);
  static std::string popColor();
  static std::string getColor(FormatColor preset);
};

#endif // COLOR_FORMATTER_H