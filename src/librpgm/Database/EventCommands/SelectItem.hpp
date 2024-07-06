#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct SelectItemCommand final : IEventCommand {
  SelectItemCommand() = default;
  explicit SelectItemCommand(const std::optional<int>& _indent, const nlohmann::json& parameters);
  ~SelectItemCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Select_Item; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int item = 1;
  ItemType type = ItemType::Key_Item;
};