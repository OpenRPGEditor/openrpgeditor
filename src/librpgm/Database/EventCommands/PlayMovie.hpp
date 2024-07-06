#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct PlayMovieCommand final : IEventCommand {
  PlayMovieCommand() = default;
  explicit PlayMovieCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~PlayMovieCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_Movie; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::string name ;
};
