#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct PlayMovieCommand final : IEventCommand {
  ~PlayMovieCommand() override = default;
  std::string name;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_Movie; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
