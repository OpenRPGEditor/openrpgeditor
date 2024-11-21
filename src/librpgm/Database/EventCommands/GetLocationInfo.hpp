#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct GetLocationInfoCommand final : IEventCommand {
  GetLocationInfoCommand() = default;
  explicit GetLocationInfoCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~GetLocationInfoCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Get_Location_Info; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int variable;
  TileType type;
  LocationSource source;
  int x;
  int y;
};
