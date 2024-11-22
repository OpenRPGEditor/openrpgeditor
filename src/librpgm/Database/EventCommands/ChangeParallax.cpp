#include "Database/EventCommands/ChangeParallax.hpp"

#include "Database/Database.hpp"

ChangeParallaxCommand::ChangeParallaxCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(image);
  parameters.at(1).get_to(loopHorizontally);
  parameters.at(2).get_to(loopVertically);
  parameters.at(3).get_to(scrollX);
  parameters.at(4).get_to(scrollY);
}

void ChangeParallaxCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(image);
  out.push_back(loopHorizontally);
  out.push_back(loopVertically);
  out.push_back(scrollX);
  out.push_back(scrollY);
}

std::string ChangeParallaxCommand::stringRep(const Database& db) const {
  auto rep = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Parallax" +
             colon.data() + db.imageText(image) + ColorFormatter::popColor();
  if (loopHorizontally || loopVertically) {
    rep += ColorFormatter::getColor(FormatColor::Gray) + " (";

    if (loopHorizontally) {
      rep += "Loop Horizontally";
    }
    if (loopVertically) {
      if (loopHorizontally) {
        rep += ", ";
      }
      rep += "Loop Vertically";
    }
    rep = rep + ")" + ColorFormatter::popColor();
  }

  return rep;
}
