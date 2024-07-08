#pragma once

#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeParallaxCommand final : IEventCommand {
  ChangeParallaxCommand() = default;
  explicit ChangeParallaxCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeParallaxCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Parallax; }
  void serializeParameters(nlohmann::json& out) const override;

  std::string image;
  bool loopHorizontally{false};
  bool loopVertically{false};
  int scrollX{0};
  int scrollY{0};

  std::string stringRep(const Database& db) const override;
};
