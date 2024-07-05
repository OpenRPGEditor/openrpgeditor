#include "Database/EventCommands/ShopProcessing.hpp"
#include "Database/Database.hpp"

ShopProcessingGoodCommand::ShopProcessingGoodCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(type);
  parameters[1].get_to(id);
  parameters[2].get_to(priceType);
  parameters[3].get_to(price);
}

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

ShopProcessingCommand::ShopProcessingCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(type);
  parameters[1].get_to(id);
  parameters[2].get_to(priceType);
  parameters[3].get_to(price);
  parameters[4].get_to(purchaseOnly);
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
