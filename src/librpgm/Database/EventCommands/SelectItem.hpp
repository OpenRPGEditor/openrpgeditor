#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct SelectItemCommand final : IEventCommand {
  SelectItemCommand() = default;
  explicit SelectItemCommand(const std::optional<int>& _indent, nlohmann::json& parameters);
  ~SelectItemCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Select_Item; }
  int item{};
  ItemType type{};
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};