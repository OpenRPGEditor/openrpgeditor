#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct NextCommentCommand final : IEventCommand {
  NextCommentCommand() = default;
  explicit NextCommentCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~NextCommentCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Comment; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<NextCommentCommand>(*this); }

  std::string text;
};

struct CommentCommand final : IEventCommand {
  CommentCommand() = default;
  explicit CommentCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~CommentCommand() override = default;
  [[nodiscard]] std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<CommentCommand>(*this); }

  [[nodiscard]] EventCode code() const override { return EventCode::Comment; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  void addText(NextCommentCommand* n) { nextComments.emplace_back(n); }

  std::string text;
  std::vector<std::shared_ptr<NextCommentCommand>> nextComments;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;

  [[nodiscard]] std::vector<std::string> stringValues() const override {
    std::string result = text;
    for (const auto& next : nextComments) {
      result += (!result.empty() ? "\n" : "") + next->text;
    }

    return {result};
  }

  [[nodiscard]] std::vector<std::string> stringValueNames() const override { return {"text"}; }
  [[nodiscard]] bool hasStringValues() const override { return true; }
};