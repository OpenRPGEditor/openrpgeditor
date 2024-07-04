#pragma once
#include "IEventCommand.hpp"
#include <format>

struct SaveBGMCommand : IEventCommand {
  ~SaveBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Save_BGM; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
