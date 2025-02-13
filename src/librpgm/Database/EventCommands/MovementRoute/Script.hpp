#pragma once
#include "Database/Database.hpp"
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementScriptCommand final : IMovementRouteStep {
  MovementScriptCommand() = default;
  explicit MovementScriptCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~MovementScriptCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Script_del_Movement; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementScriptCommand>(*this); }

  std::string script;
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Variable) {
      if (script.contains("gameVariables") && script.contains(Database::instance()->gameConstants.variables.at(targetId))) {
        return true;
      }
    }
    if (type == SearchType::Switch) {
      if (script.contains("gameSwitches") && script.contains(Database::instance()->gameConstants.variables.at(targetId))) {
        return true;
      }
    }
    return false;
  };
};
