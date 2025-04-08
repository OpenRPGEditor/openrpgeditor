#pragma once
#include "Database/Database.hpp"
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementSwitchONCommand final : IMovementRouteStep {
  MovementSwitchONCommand() = default;
  explicit MovementSwitchONCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~MovementSwitchONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Switch_ON; }
  int id{1};

  void serializeParameters(nlohmann::ordered_json& out) const override;

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
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementSwitchONCommand>(*this); }
};
