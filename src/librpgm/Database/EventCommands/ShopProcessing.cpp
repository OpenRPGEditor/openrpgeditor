#include "Database/EventCommands/ShopProcessing.hpp"
#include "Database/Database.hpp"

ShopProcessingGoodCommand::ShopProcessingGoodCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(type);
  parameters.at(1).get_to(id);
  parameters.at(2).get_to(priceType);
  parameters.at(3).get_to(price);
}

void ShopProcessingGoodCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(type);
  out.push_back(id);
  out.push_back(priceType);
  out.push_back(price);
}

std::string ShopProcessingGoodCommand::stringRep(const Database& db, const bool colored) const {
  std::string good;
  switch (type) {
  case ShopType::Item:
    good = db.itemName(id);
    break;
  case ShopType::Armor:
    good = db.armorNameOrId(id);
    break;
  case ShopType::Weapon:
    good = db.weaponNameOrId(id);
    break;
  }
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + std::string(tr("Shop Processing").length(), ' ') + colon.data() + good +
         ColorFormatter::popColor(colored);
}

ShopProcessingCommand::ShopProcessingCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(type);
  parameters.at(1).get_to(id);
  parameters.at(2).get_to(priceType);
  parameters.at(3).get_to(price);
  parameters.at(4).get_to(purchaseOnly);
}

void ShopProcessingCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(type);
  out.push_back(id);
  out.push_back(priceType);
  out.push_back(price);
  out.push_back(purchaseOnly);
}

std::string ShopProcessingCommand::stringRep(const Database& db, const bool colored) const {
  std::string good;
  switch (type) {
  case ShopType::Item:
    good = db.itemName(id);
    break;
  case ShopType::Armor:
    good = db.armorNameOrId(id);
    break;
  case ShopType::Weapon:
    good = db.weaponNameOrId(id);
    break;
  }
  std::string ret = indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Shop Processing") + colon.data() + good + ColorFormatter::popColor(colored);

  for (const auto& g : goods) {
    ret += "\n" + g->stringRep(db);
  }
  return ret;
}
