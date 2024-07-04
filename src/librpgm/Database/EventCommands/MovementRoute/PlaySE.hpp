#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include "Database/Audio.hpp"
#include "Database/Database.hpp"

#include <format>

struct MovementPlaySECommand : IMovementRouteStep {
  ~MovementPlaySECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_SE_del_Movement; }
  Audio se;

  void serializeParameters(nlohmann::json& out) override { out.push_back(se); }

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
