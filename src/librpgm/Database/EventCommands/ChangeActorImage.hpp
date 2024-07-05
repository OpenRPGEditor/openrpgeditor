#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeActorImageCommand final : IEventCommand {
  ~ChangeActorImageCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Actor_Images; }

  int actor;
  std::string charPicture;
  int charIndex;
  std::string facePicture;
  int faceIndex;
  std::string battlerPicture;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
