#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShopProcessingGoodCommand final : IEventCommand {
  ShopProcessingGoodCommand() = default;
  explicit ShopProcessingGoodCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);

  ~ShopProcessingGoodCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Shop_Processing_Good; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ShopProcessingGoodCommand>(*this); }
  bool hasReference(int targetId, SearchType searchType) override {
    if (searchType == SearchType::Items && type == ShopType::Item) {
      return id == targetId;
    }
    if (searchType == SearchType::Armors && type == ShopType::Armor) {
      return id == targetId;
    }
    if (searchType == SearchType::Weapons && type == ShopType::Weapon) {
      return id == targetId;
    }
    return false;
  };
  bool setReference(int targetId, int newId, SearchType type) override {
    if (hasReference(targetId, type)) {
      id = newId;
      return true;
    }
    return false;
  }
  ShopType type = ShopType::Item;
  int id{1};
  PriceType priceType = PriceType::Standard;
  int price{0};
};

struct ShopProcessingCommand final : IEventCommand {
  ShopProcessingCommand() = default;
  explicit ShopProcessingCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);

  ~ShopProcessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Shop_Processing; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ShopProcessingCommand>(*this); }

  void addGood(ShopProcessingGoodCommand* good) { goods.emplace_back(good); }

  ShopType type{};
  int id{0};
  PriceType priceType{};
  int price{};
  bool purchaseOnly{false};
  std::vector<std::shared_ptr<ShopProcessingGoodCommand>> goods;
};