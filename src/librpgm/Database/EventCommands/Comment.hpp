#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct NextCommentCommand : IEventCommand {
~NextCommentCommand() override = default;
[[nodiscard]] EventCode code() const override { return EventCode::Next_Comment; }

std::string text;
};
struct CommentCommand : IEventCommand {
  ~CommentCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Comment; }
  std::string text;
  std::vector<std::shared_ptr<NextCommentCommand>> nextComments;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};