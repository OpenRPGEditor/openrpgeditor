#include "Database/EventCommands/ChangeParallax.hpp"

#include "Database/Database.hpp"

std::string ChangeParallaxCommand::stringRep(const Database& db) const {
  auto rep = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Parallax" +
             colon.data() + " " + db.imageText(image) + ColorFormatter::popColor();
  if (loopHorizontally || loopVertically) {
    rep += ColorFormatter::getColor(Color::Gray);

    if (loopHorizontally) {
      rep += "Loop Horizontally";
    }
    if (loopVertically) {
      if (loopHorizontally) {
        rep += ", ";
      }
      rep += "Loop Vertically";
    }
    rep = db.parentheses(rep) + ColorFormatter::popColor();
  }

  return rep;
}
