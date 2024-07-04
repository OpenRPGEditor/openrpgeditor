#include "Database/EventCommands/ShopProcessing.hpp"

#include "Database/Database.hpp"

std::string ShopProcessingGoodCommand::stringRep(const Database& db) const {
  std::string good;
  switch (type) {
  case ShopType::Item:
    good = db.items.item(id)->name;
    break;
  case ShopType::Armor:
    good = db.armors.armor(id)->name;
    break;
  case ShopType::Weapon:
    good = db.weapons.weapon(id)->name;
    break;
  }
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "               " + colon.data() +
         good + ColorFormatter::popColor();
}

std::string ShopProcessingCommand::stringRep(const Database& db) const {
  std::string good;
  switch (type) {
  case ShopType::Item:
    good = db.items.item(id)->name;
    break;
  case ShopType::Armor:
    good = db.armors.armor(id)->name;
    break;
  case ShopType::Weapon:
    good = db.weapons.weapon(id)->name;
    break;
  }
  std::string ret = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Shop Processing" +
                    colon.data() + good + ColorFormatter::popColor();

  for (const auto& g : goods) {
    ret += "\n" + g->stringRep(db);
  }
  return ret;
}
