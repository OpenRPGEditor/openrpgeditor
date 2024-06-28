#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShopProcessingGoodCommand : IEventCommand {
  ~ShopProcessingGoodCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Shop_Processing_Good; }

  ShopType type;
  int id;
  PriceType priceType;
  int price;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + " {}" + ColorFormatter::popColor();
  }
};

struct ShopProcessingCommand : IEventCommand {
  ~ShopProcessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Shop_Processing; }

  ShopType type;
  int id;
  PriceType priceType;
  int price;
  bool purchaseOnly;
  std::vector<std::shared_ptr<ShopProcessingGoodCommand>> goods;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Shop Processing : {}" + ColorFormatter::popColor();
  }
};
