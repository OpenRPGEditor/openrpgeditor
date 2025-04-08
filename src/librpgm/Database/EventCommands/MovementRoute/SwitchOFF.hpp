#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementSwitchOFFCommand final : IMovementRouteStep {
  MovementSwitchOFFCommand() = default;
  explicit MovementSwitchOFFCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~MovementSwitchOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Switch_OFF; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementSwitchOFFCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Switch) {
      return targetId == id;
    }
    return false;
  };
  bool setReference(int targetId, int newId, SearchType type) override {
    if (hasReference(targetId, type)) {
      id = newId;
      return true;
    }
    return false;
  }
  int id{1};
};
