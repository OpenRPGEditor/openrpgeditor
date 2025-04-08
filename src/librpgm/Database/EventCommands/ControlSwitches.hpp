#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ControlSwitches : IEventCommand {
  ControlSwitches() = default;
  explicit ControlSwitches(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ControlSwitches() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Switches; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ControlSwitches>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Switch) {
      return start == targetId && end == targetId;
    }
    if (type == SearchType::Range) {
      if (start != end) {
        return true; // within range
      }
    }
    return false;
  };

  bool setReference(int targetId, int newId, SearchType type) override {
    if (hasReference(targetId, type)) {
      if (start == targetId && end == targetId) {
        start = newId;
        end = newId;
      }
      return true;
    }
    return false;
  }

  int start{1};
  int end{1};
  ValueControl turnOff = ValueControl::ON;
};
