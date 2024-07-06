#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ErasePictureCommand final : IEventCommand {
  ErasePictureCommand() = default;
  explicit ErasePictureCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ErasePictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Erase_Picture; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int picture;
};
