#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ErasePictureCommand final : IEventCommand {
  ErasePictureCommand() = default;
  explicit ErasePictureCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ErasePictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Erase_Picture; }
  int picture;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
