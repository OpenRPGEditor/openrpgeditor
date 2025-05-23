#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct InputNumberCommand final : IEventCommand {
  InputNumberCommand() = default;
  explicit InputNumberCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~InputNumberCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Input_Number; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<InputNumberCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Variable) {
      return variable == targetId;
    }
    return false;
  };
  bool setReference(int targetId, int newId, SearchType type) override {
    if (hasReference(targetId, type)) {
      variable = newId;
      return true;
    }
    return false;
  }
  int variable = 1;
  int digits = 1;
};
