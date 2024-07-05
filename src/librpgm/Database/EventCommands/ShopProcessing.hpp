#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShopProcessingGoodCommand final : IEventCommand {
  ShopProcessingGoodCommand() = default;
  explicit ShopProcessingGoodCommand(const std::optional<int>& indent, nlohmann::json& parameters);

  ~ShopProcessingGoodCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Shop_Processing_Good; }

  ShopType type;
  int id;
  PriceType priceType;
  int price;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};

struct ShopProcessingCommand final : IEventCommand {
  ShopProcessingCommand() = default;
  explicit ShopProcessingCommand(const std::optional<int>& indent, nlohmann::json& parameters);

  ~ShopProcessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Shop_Processing; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  void addGood(ShopProcessingGoodCommand* good) {
    goods.emplace_back(good);
  }

  ShopType type{};
  int id{};
  PriceType priceType{};
  int price{};
  bool purchaseOnly{};
  std::vector<std::shared_ptr<ShopProcessingGoodCommand>> goods;
};
