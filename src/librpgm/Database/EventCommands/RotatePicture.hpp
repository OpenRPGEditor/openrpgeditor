#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct RotatePictureCommand final : IEventCommand {
  RotatePictureCommand() = default;
  explicit RotatePictureCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~RotatePictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Rotate_Picture; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<RotatePictureCommand>(*this); }

  int picture;
  int rotation;
};
