#pragma once

#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeParallaxCommand final : IEventCommand {
  ~ChangeParallaxCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Parallax; }

  void serializeParameters(nlohmann::json& out) override {
    out.push_back(image);
    out.push_back(loopHorizontally);
    out.push_back(loopVertically);
    out.push_back(scrollX);
    out.push_back(scrollY);
  }

  std::string image;
  bool loopHorizontally;
  bool loopVertically;
  int scrollX;
  int scrollY;

  std::string stringRep(const Database& db) const override;
};
