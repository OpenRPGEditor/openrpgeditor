#include "ColorFormatter.hpp"

#include "Globals.hpp"

  /**
   *
   * @param command
   * @return Returns an ImGui formatted string for the specific command name.
   */
  std::string ColorFormatter::getColorCode(std::string command) {
    return "&push-color=255,255,0;"; // Default to no color
  }
  /**
   *
   * @return Returns Pop() for ImGui color encoding. Used at the end of color encoding to indicate the end of it.
   */
  std::string ColorFormatter::popColor() {
    return "&pop-color;"; // Default to no color
  }
  /**
   *
   * @param preset
   * @return Input: Color enum, returns an ImGui formatted string based off the enum.
   */
  std::string ColorFormatter::getColor(Color preset) {
    if (preset == Gray)
      return "&push-color=65,68,69;";

    return "&push-color=255,255,0;";
  }