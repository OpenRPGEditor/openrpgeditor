#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct WaitCommand : IEventCommand {
  int duration;
  ~WaitCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Wait; }

  void serializeParameters(nlohmann::json& out) override {
    out.push_back(duration);
  }

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
