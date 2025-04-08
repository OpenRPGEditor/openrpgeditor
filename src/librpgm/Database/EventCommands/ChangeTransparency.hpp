#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeTransparencyCommand final : IEventCommand {
  ChangeTransparencyCommand() = default;
  explicit ChangeTransparencyCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeTransparencyCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Transparency; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeTransparencyCommand>(*this); }

  ValueControl transparency;
};
