#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ShowChoiceCommand : IEventCommand {
  ~ShowChoiceCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Choices; }
  TextBackground background;
  ChoiceWindowPosition positionType;
  std::vector<std::string> choices;
  int cancelType; // < 0 == disallow/branch
  int defaultType; // -1 is none

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    std::string suffix = ColorFormatter::getColor(Color::Gray) + " (" + DecodeEnumName(background) + ", " + DecodeEnumName(positionType);
    suffix += (defaultType < 0 ? ", -," : ", #") + std::to_string(defaultType) + ",";
    suffix += (cancelType < 0 ? ", -," : ", #") + std::to_string(defaultType) + ")" + ColorFormatter::popColor();

    std::string choiceList = std::accumulate(std::next(choices.begin()), choices.end(), *choices.begin(),
                         [](const std::string& a, const std::string& b){ return a + ", " + b; });

    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Show Choices" + colon.data() + choiceList + suffix;
  }
};

struct WhenSelectedCommand : IEventCommand {
  ~WhenSelectedCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Selected; }

  int param1;
  std::string choice;

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + " When " + choice + ColorFormatter::popColor();
  }
};

struct WhenCancelCommand : IEventCommand {
  ~WhenCancelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Cancel; }
  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + " When Cancel" + ColorFormatter::popColor();
  }
};

struct ShowChoicesEndCommand : IEventCommand {
  ~ShowChoicesEndCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End_del_ShowChoices; }
  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + " End" + ColorFormatter::popColor();
  }
};