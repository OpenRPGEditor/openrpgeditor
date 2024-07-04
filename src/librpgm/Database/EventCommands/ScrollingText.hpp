#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct NextScrollingTextCommand : IEventCommand {
  ~NextScrollingTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Scrolling_Text; }

  std::string text;
};

struct ShowScrollTextCommand : IEventCommand {
  ~ShowScrollTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Scrolling_Text; }

  int speed;
  bool noFast;
  std::vector<std::shared_ptr<NextScrollingTextCommand>> text;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};