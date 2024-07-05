#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeActorImageCommand final : IEventCommand {
  ChangeActorImageCommand() = default;
  explicit ChangeActorImageCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ChangeActorImageCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Actor_Images; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int actor;
  std::string charPicture;
  int charIndex;
  std::string facePicture;
  int faceIndex;
  std::string battlerPicture;
};
