#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct PlayMovieCommand final : IEventCommand {
  PlayMovieCommand() = default;
  explicit PlayMovieCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~PlayMovieCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_Movie; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<PlayMovieCommand>(*this); }
  std::string name;
};
