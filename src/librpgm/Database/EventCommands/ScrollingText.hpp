#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct NextScrollingTextCommand final : IEventCommand {
  NextScrollingTextCommand() = default;
  explicit NextScrollingTextCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~NextScrollingTextCommand() override = default;
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Scrolling_Text; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<NextScrollingTextCommand>(*this); }

  std::string text;
};

struct ShowScrollTextCommand final : IEventCommand {
  ShowScrollTextCommand() = default;
  explicit ShowScrollTextCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ShowScrollTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Scrolling_Text; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ShowScrollTextCommand>(*this); }

  void addText(NextScrollingTextCommand* n) { text.emplace_back(n); }

  int speed{2};
  bool noFast{false};
  std::vector<std::shared_ptr<NextScrollingTextCommand>> text;
};