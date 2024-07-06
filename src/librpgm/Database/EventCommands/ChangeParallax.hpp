#pragma once

#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeParallaxCommand final : IEventCommand {
  ChangeParallaxCommand() = default;
  explicit ChangeParallaxCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeParallaxCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Parallax; }
  void serializeParameters(nlohmann::json& out) const override;

  std::string image;
  bool loopHorizontally;
  bool loopVertically;
  int scrollX;
  int scrollY;

  std::string stringRep(const Database& db) const override;
};
