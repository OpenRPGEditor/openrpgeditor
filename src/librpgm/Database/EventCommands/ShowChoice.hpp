#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ShowChoiceCommand : IEventCommand {
  ~ShowChoiceCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Choices; }
  TextBackground background;
  ChoiceWindowPosition positionType;
  std::vector<std::string> choices;
  int cancelType;  // < 0 == disallow/branch
  int defaultType; // -1 is none

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};

struct WhenSelectedCommand : IEventCommand {
  ~WhenSelectedCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Selected; }

  int param1;
  std::string choice;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};

struct WhenCancelCommand : IEventCommand {
  ~WhenCancelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Cancel; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};

struct ShowChoicesEndCommand : IEventCommand {
  ~ShowChoicesEndCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End_del_ShowChoices; }
};