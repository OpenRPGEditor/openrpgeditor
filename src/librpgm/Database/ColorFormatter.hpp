#ifndef COLOR_FORMATTER_H
#define COLOR_FORMATTER_H

#include "Globals.hpp"

#include <string>

struct ColorFormatter {
  static std::string getColorCode(std::string command);
  static std::string popColor();
  static std::string getColor(Color preset);
};

#endif // COLOR_FORMATTER_H