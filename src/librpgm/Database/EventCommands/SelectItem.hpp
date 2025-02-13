#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct SelectItemCommand final : IEventCommand {
  SelectItemCommand() = default;
  explicit SelectItemCommand(const std::optional<int>& _indent, const nlohmann::ordered_json& parameters);
  ~SelectItemCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Select_Item; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<SelectItemCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Variable) {
      return item == targetId;
    }
    if (type == SearchType::Items) {
      return targetId == item;
    }
    return false;
  };
  int item = 1;
  ItemType type = ItemType::Key_Item;
};