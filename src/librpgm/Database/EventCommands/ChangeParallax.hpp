#pragma once

#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeParallaxCommand final : IEventCommand {
  ChangeParallaxCommand() = default;
  explicit ChangeParallaxCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeParallaxCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Parallax; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeParallaxCommand>(*this); }

  std::string image;
  bool loopHorizontally{false};
  bool loopVertically{false};
  int scrollX{0};
  int scrollY{0};

  std::string stringRep(const Database& db, bool colored = true) const override;
};
