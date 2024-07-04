#pragma once
#include "Database/Database.hpp"
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementChangeImageCommand : IMovementRouteStep {
  ~MovementChangeImageCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Image; }

  std::string image;
  int character;

  void serializeParameters(nlohmann::json& out) override {
    out.push_back(image);
    out.push_back(character);
  }

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
