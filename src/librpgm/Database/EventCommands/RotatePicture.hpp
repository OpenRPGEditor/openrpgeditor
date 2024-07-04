#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct RotatePictureCommand : IEventCommand {
  ~RotatePictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Rotate_Picture; }
  int picture;
  int rotation;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
