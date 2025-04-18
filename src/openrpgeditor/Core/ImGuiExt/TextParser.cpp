#include "Core/ImGuiExt/TextParser.hpp"

#include <iostream>
#include <regex>
#include <stack>
#include <string>

namespace ImGui {
struct Color {
  uint8_t r = 255;
  uint8_t g = 255;
  uint8_t b = 255;
  uint8_t a = 255;
};
std::stack<Color> colors;

bool BeginsWith(const char* str, const int len, const char* other) {
  for (int i = 0; i < len; ++i, ++str, ++other) {
    if (*str != *other) {
      return false;
    }
  }
  return true;
}

uint32_t FromHex(char ch) {
  if (ch >= '0' && ch <= '9')
    return ch - '0';

  if (ch >= 'A' && ch <= 'F')
    return ch - 'A' + 10;

  if (ch >= 'a' && ch <= 'f')
    return ch - 'a' + 10;

  return 0;
}

uint8_t GetColorValue(const char* str) { return (FromHex(str[0]) << 4) + FromHex(str[1]); }

Color _ParseColor(const char* s, const int len) {
  Color color;
  color.r = GetColorValue(s + 1);
  color.g = GetColorValue(s + 3);
  color.b = GetColorValue(s + 5);
  color.a = 0xff;
  if (len == 9) {
    color.a = GetColorValue(s + 5);
  }
  return color;
}

bool ParseColor(const char* s, ImU32* col, int* skipChars) {
  if (colors.empty()) {
    Color col;
    *((int*)(&col.r)) = GetColorU32(ImGuiCol_Text);
    colors.push(col);
  }

  std::string inputString{s};
  std::string token;
  std::string value;
  bool parsed = false;
  if (s[0] == '&') {
    int len = 1;
    while (s[len] != ';' && s[len] != '\0') {
      len++;
    }
    ++s;

    if (BeginsWith(s, 11, "push-color=")) {
      Color color = _ParseColor(s + 11, len - 11);
      *col = *reinterpret_cast<int*>(&color.r);
      colors.push(color);
      parsed = true;
    } else if (BeginsWith(s, 6, "color=")) {
      Color color = _ParseColor(s + 6, len - 6);
      *col = *reinterpret_cast<int*>(&color.r);
      parsed = true;
    } else if (BeginsWith(s, 9, "pop-color")) {
      if (colors.size() > 1) {
        colors.pop();
      }
      Color tmp = colors.top();
      *col = *reinterpret_cast<int*>(&tmp.r);
      parsed = true;
    }
    if (parsed) {
      *skipChars = len + 1;
      return true;
    }
  }
  return false;
}

ImU32 ParseGetCurrentColor() {
  if (colors.empty()) {
    Color col;
    *((int*)(&col.r)) = GetColorU32(ImGuiCol_Text);
    colors.push(col);
  }

  Color color = colors.top();
  return *(int*)(&color.r);
}

int ParseGetStackSize() { return colors.size(); }
} // namespace ImGui