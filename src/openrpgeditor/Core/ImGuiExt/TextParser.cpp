#include "Core/ImGuiExt/TextParser.hpp"
#include <regex>
#include <stack>
#include <string>

namespace ImGui {
std::vector<std::string> split(const std::string& str, const std::string& delim = " ") {
  std::vector<std::string> res;
  std::size_t current, previous = 0;
  current = str.find(delim);
  while (current != std::string::npos) {
    res.push_back(str.substr(previous, current - previous));
    previous = current + delim.length();
    current = str.find(delim, previous);
  }
  res.push_back(str.substr(previous, current - previous));
  return res;
}

static const std::regex pattern("^&([^&=;]+)(?:=([^&;]+))?;");

struct Color {
  uint8_t r = 255;
  uint8_t g = 255;
  uint8_t b = 255;
  uint8_t a = 255;
};
std::stack<Color> colors;

bool ParseColor(const char* s, ImU32* col, int* skipChars) {
  if (colors.empty()) {
    Color col;
    *((int*)(&col.r)) = GetColorU32(ImGuiCol_Text);
    colors.push(col);
  }
  std::string input_string(s);
  std::sregex_iterator iter(input_string.begin(), input_string.end(), pattern);
  std::sregex_iterator end;
  std::string token;
  std::string value;
  bool parsed = false;
  if (iter != end) {
    std::smatch match = *iter;
    token = match[1];
    if (match[2].length() > 0) {
      value = match[2];
    }
    if (!token.compare("push-color") && !value.empty()) {
      std::vector<std::string> values = split(value, ",");
      Color color;
      if (values.size() >= 1) {
        color.r = std::stoi(values[0], nullptr, 0);
      }
      if (values.size() >= 2) {
        color.g = std::stoi(values[1], nullptr, 0);
      }
      if (values.size() >= 3) {
        color.b = std::stoi(values[2], nullptr, 0);
      }
      if (values.size() >= 4) {
        color.a = std::stoi(values[3], nullptr, 0);
      }

      *col = *(int*)(&color.r);
      colors.push(color);
      parsed = true;
    } else if (!token.compare("pop-color") && !colors.empty()) {
      if (colors.size() > 1) {
        colors.pop();
      }
      Color tmp = colors.top();
      *col = *(int*)(&tmp.r);
      parsed = true;
    } else if (!token.compare("color") && !value.empty()) {
      std::vector<std::string> values = split(value, ",");
      Color color;
      if (values.size() >= 0) {
        color.r = std::stoi(values[0], nullptr, 0);
      }
      if (values.size() >= 1) {
        color.g = std::stoi(values[1], nullptr, 0);
      }
      if (values.size() >= 2) {
        color.b = std::stoi(values[2], nullptr, 0);
      }
      if (values.size() >= 3) {
        color.a = std::stoi(values[3], nullptr, 0);
      }

      *col = *(int*)(&color.r);
      parsed = true;
    }
    if (parsed) {
      *skipChars = match.length();
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