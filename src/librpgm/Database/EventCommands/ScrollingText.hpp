#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct NextScrollingTextCommand final : IEventCommand {
  NextScrollingTextCommand() = default;
  explicit NextScrollingTextCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~NextScrollingTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Scrolling_Text; }

  std::string text;
};

struct ShowScrollTextCommand final : IEventCommand {
  ShowScrollTextCommand() = default;
  explicit ShowScrollTextCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ShowScrollTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Scrolling_Text; }

  void addText(NextScrollingTextCommand* n) { text.emplace_back(n); }

  int speed;
  bool noFast;
  std::vector<std::shared_ptr<NextScrollingTextCommand>> text;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};