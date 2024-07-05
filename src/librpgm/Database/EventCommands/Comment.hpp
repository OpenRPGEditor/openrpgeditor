#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct NextCommentCommand final : IEventCommand {
  NextCommentCommand() = default;
  explicit NextCommentCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(text);
  }
  ~NextCommentCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Comment; }

  std::string text;
};
struct CommentCommand final : IEventCommand {
  CommentCommand() = default;
  explicit CommentCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(text);
  }
  ~CommentCommand() override = default;

  [[nodiscard]] EventCode code() const override { return EventCode::Comment; }
  void addText(NextCommentCommand* n) { nextComments.emplace_back(n); }

  std::string text = "";
  std::vector<std::shared_ptr<NextCommentCommand>> nextComments;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};