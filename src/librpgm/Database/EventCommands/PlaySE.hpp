#pragma once
#include "MovementRoute/IMovementRouteStep.hpp"
#include "Database/Audio.hpp"

#include <format>

struct PlaySECommand final : IMovementRouteStep {
  ~PlaySECommand() override = default;
  Audio audio;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_SE_del_Movement; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
