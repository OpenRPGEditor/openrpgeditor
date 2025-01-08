#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct GatherFollowersCommand final : IEventCommand {
  GatherFollowersCommand() = default;
  explicit GatherFollowersCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {}
  ~GatherFollowersCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Gather_Followers; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<GatherFollowersCommand>(*this); }
};
