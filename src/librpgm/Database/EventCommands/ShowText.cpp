#include "ShowText.hpp"

#include "Database/Database.hpp"

std::string ShowTextCommand::stringRep(const Database& db) const {
  const auto name = UndectorateEnumName(code());
  std::string ret = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + name + colon.data() +
                    ColorFormatter::popColor() + ColorFormatter::getColor(Color::Gray) +
                    db.imageText(faceImage, faceIndex) + ", " + DecodeEnumName(background) + ", " +
                    DecodeEnumName(position) + ColorFormatter::popColor();

  for (const auto& t : text) {
    if (!ret.empty()) {
      ret += "\n";
    }
    ret += indentText(indent) + symbol(t->code()) + ColorFormatter::getColorCode(t->code()) +
           std::string(name.length(), ' ') + colon.data() + t->text + ColorFormatter::popColor();
  }
  return ret;
}