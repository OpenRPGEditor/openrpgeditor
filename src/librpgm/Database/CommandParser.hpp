#pragma once
#include "ColorFormatter.hpp"
#include "Database/Globals.hpp"
#include "Database/Audio.hpp"
#include "Database/MovementRoute.hpp"
#include "nlohmann/json.hpp"

#include <format>
#include <memory>
struct NextScriptCommand;
struct IEventCommand {
  std::optional<int> indent{};
  const std::string diamond = "\u25c6";
  //const std::string colon = "\uff1a";
  const std::string colon = ":";
  virtual ~IEventCommand() = default;
  [[nodiscard]] virtual EventCode code() const = 0;
  [[nodiscard]] virtual std::string stringRep() const {
    return std::string(indent ? *indent * 4 : 0, ' ') + DecodeEnumName(code());
  }
  virtual std::string symbol(EventCode code) const {
    return static_cast<int>(code) < 400 ? diamond : colon;
  }
  virtual std::string indentText(std::optional<int> indent) const {
    std::string text = "";
    for(int i = 0; i < indent; ++i) {
      text += "  ";
    }
    return text;
  }
};

struct UnhandledEventCommand : IEventCommand {
  ~UnhandledEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return m_code; }

  EventCode m_code{};
  nlohmann::json data;
};

struct EventDummy : IEventCommand {
  ~EventDummy() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Event_Dummy; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + ColorFormatter::getColorCode(code()) + symbol(code()) + ColorFormatter::popColor();
  }
};

struct NextTextCommand : IEventCommand {
  ~NextTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Text; }

  std::string text;
};

struct ShowTextCommand : IEventCommand {
  ~ShowTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Text; }
  std::string faceImage;
  int faceIndex;
  TextBackground background;
  TextWindowPosition position;
  std::string textLine;
  std::vector<std::shared_ptr<NextTextCommand>> text;


  [[nodiscard]] std::string stringRep() const override {
    std::string ret = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Text : " + ColorFormatter::popColor() +
      (faceImage == "" ? "None, " : faceImage + std::format("({}), ", faceIndex)) + DecodeEnumName(background) + ", " + DecodeEnumName(position);

    for (const auto& t : text) {
      if (!ret.empty()) {
        ret += "\n";
      }
      ret += indentText(indent) + colon + indentText(indent) + colon + ColorFormatter::getColorCode(code()) + t->text;
    }
    return ret;
  }
};

struct ShowChoiceCommand : IEventCommand {
  ~ShowChoiceCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Text; }
  TextBackground background;
  ChoiceWindowPosition positionType;
  std::vector<std::string> choices;
  int cancelType; // < 0 == disallow/branch
  int defaultType; // -1 is none

  [[nodiscard]] std::string stringRep() const override {
    std::string suffix = ColorFormatter::getColor(Gray) + " (" + DecodeEnumName(background) + ", " + DecodeEnumName(positionType);
    suffix += defaultType < 0 ? "-," : "#" + std::to_string(defaultType) + ",";
    suffix += cancelType < 0 ? "-," : "#" + std::to_string(defaultType) + ")" + ColorFormatter::popColor();

    std::string choiceList = std::accumulate(std::next(choices.begin()), choices.end(), *choices.begin(),
                         [](const std::string& a, const std::string& b){ return a + ", " + b; });

    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + " " + ColorFormatter::popColor() + choiceList + suffix;
  }
};

struct WhenSelectedCommand : IEventCommand {
  ~WhenSelectedCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Selected; }

  int param1;
  std::string choice;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + " When " + choice + ColorFormatter::popColor();
  }
};

struct WhenCancelCommand : IEventCommand {
  ~WhenCancelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Cancel; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + " When Cancel" + ColorFormatter::popColor();
  }
};

struct ShowChoicesEndCommand : IEventCommand {
  ~ShowChoicesEndCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End_del_ShowChoices; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + " End" + ColorFormatter::popColor();
  }
};

struct InputNumberCommand : IEventCommand {
  ~InputNumberCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Input_Number; }
  int variable;
  int digits;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Select Item : {}, " + (digits > 1 ? " digits" : " digit") + ColorFormatter::popColor();
  }
};

struct SelectItemCommand : IEventCommand {
  ~SelectItemCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Select_Item; }
  int item;
  ItemType type;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Select Item : {}, " + DecodeEnumName(type) + ColorFormatter::popColor();
  }
};

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

  [[nodiscard]] std::string stringRep() const override {
    std::string ret = std::string(indent ? *indent : 0, '\t') + "◇Text(S) : Speed " + std::to_string(speed) + (noFast == true ? ", No Fast Forward" : "");

    for (const auto& t : text) {
      // TODO: How do we do this indent?
      if (!ret.empty()) {
        ret += "\n";
      }
      ret += std::string(indent ? *indent : 0, '\t') + " :" + std::string(((t->indent ? *t->indent : 0) + 1), '\t') +
             " : " + t->text;
    }
    return ret;
  }
};

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
  [[nodiscard]] std::string stringRep() const override {
    // TODO: How do we do this indent?
    std::string ret = std::string(indent ? *indent : 0, '\t') + "◇Comment: " + text;
    for (const auto& t : nextComments) {
      if (!ret.empty()) {
        ret += "\n";
      }
      ret += std::string(indent ? *indent : 0, '\t') + " :" + std::string(((t->indent ? *t->indent : 0) + 1), '\t') +
             " : " + t->text;
    }
    return ret;
  }
};

struct ConditionalBranchCommand : IEventCommand {
  ConditionalBranchCommand() {}
  ~ConditionalBranchCommand() override {}
  [[nodiscard]] EventCode code() const override { return EventCode::Conditional_Branch; }
  ConditionType type{};
  struct {
    int switchIdx;
    ValueControl checkIfOn;
  } globalSwitch;
  struct {
    int id;
    VariableComparisonSource source;
    VariableComparisonType comparison;
    union {
      int constant;
      int otherId;
    };
  } variable;
  struct {
    ValueControl checkIfOn;
  } selfSwitch;
  struct {
    TimerComparisonType comparison;
    int sec;
  } timer;
  struct {
    int id;
    ActorConditionType type;
    int checkId;
  } actor;
  struct {
    int id;
    EnemyConditionType type;
    int stateId; // For "State" condition
  } enemy;
  struct {
    int id;
    Direction facing;
  } character;
  struct {
    int id;
  } vehicle;
  struct {
    GoldComaprisonType type;
    int value;
  } gold;
  struct {
    int id;
  } item;
  /* For Weapon and Armor conditions */
  struct {
    int equipId;
    bool includeEquipment;
  } equip;
  std::string button;
  std::string selfSw; // A, B, C, D
  std::string name;
  std::string script;

  [[nodiscard]] std::string stringRep() const override {
    std::string strBuild;
    if (type == ConditionType::Variable) {
      std::string test = ColorFormatter::getColorCode(code());
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If {} " + DecodeEnumName(variable.comparison) + " " +
             (variable.source == VariableComparisonSource::Constant ? std::to_string(variable.constant) : "{}");
    }
    if (type == ConditionType::Switch) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If " + "{}" + " is " +
             (globalSwitch.checkIfOn == ValueControl::OFF ? "OFF" : "ON");
    }
    if (type == ConditionType::Self_Switch) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If Self Switch " + "{}" + " is " +
             (selfSwitch.checkIfOn == ValueControl::OFF ? "OFF" : "ON");
    }
    if (type == ConditionType::Timer) {
      std::string min;
      std::string sec;

      if (timer.sec > 59) {
        min = std::to_string(timer.sec / 60);
        sec = std::to_string(timer.sec % 60);
      } else {
        min = "0";
        sec = std::to_string(timer.sec);
      }
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If Timer" + ColorFormatter::popColor() + " " + DecodeEnumName(timer.comparison) + " " + min + " min " +
             sec + " sec";
    }
    if (type == ConditionType::Actor) {
      if (actor.type == ActorConditionType::Name) {
        return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If Name of {} is []" + ColorFormatter::popColor();
      } else if (actor.type == ActorConditionType::Class) {
        return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If Class of {} is []" + ColorFormatter::popColor();
      } else if (actor.type == ActorConditionType::Skill) {
        return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If {} has learned []" + ColorFormatter::popColor();
      } else if (actor.type == ActorConditionType::Weapon || actor.type == ActorConditionType::Armor) {
        return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If {} has equipped []" + ColorFormatter::popColor();
      } else if (actor.type == ActorConditionType::State) {
        return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If {} is affected by []" + ColorFormatter::popColor();
      } else {
        return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If {} is in party" + ColorFormatter::popColor();
      }
    }
    if (type == ConditionType::Enemy) {
      if (enemy.type == EnemyConditionType::State) {
        return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
     + "If {} is affected by []" + ColorFormatter::popColor();
      }
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If {} is appeared" + ColorFormatter::popColor();
    }
    if (type == ConditionType::Character) {

      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
             + "If {} is facing " + DecodeEnumName(character.facing) + ColorFormatter::popColor();
    }
    if (type == ConditionType::Vehicle) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
              + "If {} is driven" + ColorFormatter::popColor();
    }
    if (type == ConditionType::Gold) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
               + "If Gold" + " " + DecodeEnumName<GoldComaprisonType>(gold.type) + " {}";
    }
    if (type == ConditionType::Item) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
              + "If Party has {}" + ColorFormatter::popColor();
    }
    if (type == ConditionType::Weapon || type == ConditionType::Armor) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
              + "If Party has {}" + ColorFormatter::popColor() + ColorFormatter::getColor(Color::Gray) +
             " (Include Equipment)" + ColorFormatter::popColor();
    }
    if (type == ConditionType::Button) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
             + "If Button [" + "{}" + "] is pressed down" + ColorFormatter::popColor();
    }
    if (type == ConditionType::Script) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
             + "If Script : " + ColorFormatter::popColor();
    }
    return std::string(indent ? *indent * 4 : 0, ' ') +
           "◇ &push-color=255,0,255;Condition&pop-color; &push-color=0,255,0;TBD&pop-color;";
  }
};

struct ElseCommand : IEventCommand {
  ~ElseCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Else; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Else"; }
};

struct LoopCommand : IEventCommand {
  ~LoopCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Loop; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Loop"; }
};

struct RepeatAboveCommand : IEventCommand {
  ~RepeatAboveCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Repeat_Above; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Repeat Above";
  }
};

struct BreakLoopCommand : IEventCommand {
  ~BreakLoopCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Break_Loop; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Break Loop";
  }
};
struct ExitEventProecessingCommand : IEventCommand {
  ~ExitEventProecessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Exit_Event_Processing; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Exit Event Processing";
  }
};

struct CommonEventCommand : IEventCommand {
  ~CommonEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Common_Event; }
  int event;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Common Event: ";
  }
};

struct LabelCommand : IEventCommand {
  ~LabelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Label; }
  std::string label;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Label: " + label;
  }
};

struct JumpToLabelCommand : IEventCommand {
  ~JumpToLabelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Jump_To_Label; }
  std::string label;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Jump to Label: " + label;
  }
};

struct ControlSwitches : IEventCommand {
  ~ControlSwitches() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Switches; }
  int start;
  int end;
  ValueControl turnOff;

  [[nodiscard]] std::string stringRep() const override {
    if (start != end) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Control Switches : #" + std::format("{:04}", start) + ".." + std::format("{:04}", end) + " = " +
             (turnOff == ValueControl::ON ? "ON" : "OFF") + ColorFormatter::popColor();
    } else {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Control Switches : #" + std::format("{:04}", start) + " {}" + " = " +
             (turnOff == ValueControl::ON ? "ON" : "OFF") + ColorFormatter::popColor();
    }
  }
};

struct ControlVariables : IEventCommand {
  ControlVariables() {}
  ~ControlVariables() override{};
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Variables; }
  int start{};
  int end{};
  VariableControlOperation operation{};
  VariableControlOperand operand{};
  union {
    int constant;
    int variable;
    struct {
      int min;
      int max;
    } random;
    struct {
      GameDataSource source;
      union {
        ActorDataSource actorSource;
        EnemyDataSource enemySource;
        CharacterDataSource characterSource;
        OtherDataSource otherSource;
        int rawSource;
      };
      int value;
    } gameData;
    int raw{};
  };
  std::string script{};
  [[nodiscard]] std::string stringRep() const override {
    std::string strBuild;
    if (start != end) {
      strBuild = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Control Variables : #" + std::format("{:04}", start) + "..#" + std::format("{:04}", end) + " " +
                 DecodeEnumName(operation) + " ";
    } else {
      strBuild = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Control Variables : #" + std::format("{:04}", start) + " {} " + DecodeEnumName(operation) + " ";
    }

    if (operand == VariableControlOperand::Random) {
      strBuild += "Random " + std::to_string(random.min) + ".." + std::to_string(random.max);
    } else if (operand == VariableControlOperand::Variable) {
      strBuild += std::format("{:04}", variable) + " {}";
    } else if (operand == VariableControlOperand::Game_Data) {
      if (gameData.source == GameDataSource::Character) {
        strBuild += DecodeEnumName(static_cast<CharacterDataSource>(gameData.value)) + " of " +
                    (gameData.rawSource == -1  ? "Player"
                     : gameData.rawSource == 0 ? "This Event"
                                               : "{}");
      } else if (gameData.source == GameDataSource::Party) {
        strBuild += "Actor ID of the party member #" + std::to_string(gameData.rawSource);
      } else if (gameData.source == GameDataSource::Other) {
        strBuild += DecodeEnumName(static_cast<OtherDataSource>(gameData.value));
      } else {
        strBuild += "The number of {}";
      }
    } else {
      if (operand == VariableControlOperand::Script)
        strBuild += script;
      else
        strBuild += std::to_string(constant);
    }
    return strBuild + ColorFormatter::popColor();
  }
};

struct ControlSelfSwitch : IEventCommand {
  ~ControlSelfSwitch() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Self_Switch; }

  std::string selfSw; // A, B, C, D
  ValueControl turnOff;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Control Self Switch : " + selfSw + " is " + DecodeEnumName(turnOff) + ColorFormatter::popColor();
  }
};

struct ControlTimer : IEventCommand {
  ~ControlTimer() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Self_Switch; }
  TimerControl control;
  int seconds;
  [[nodiscard]] std::string stringRep() const override {
    std::string min;
    std::string sec;

    if (seconds > 59) {
      min = std::to_string(seconds / 60);
      sec = std::to_string(seconds % 60);
    } else {
      min = "0";
      sec = std::to_string(seconds);
    }
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Control Timer : " + DecodeEnumName(control) + ", " + min + " min " +
           sec + " sec" + ColorFormatter::popColor();
  }
};

struct ChangeGoldCommmand : IEventCommand {
  ~ChangeGoldCommmand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Gold; }
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
  [[nodiscard]] std::string stringRep() const override {
    if (operandSource == QuantityChangeSource::Constant)
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Gold: " + (operation == QuantityChangeOp::Increase ? "+ " : "- ") + std::to_string(operand);

    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Gold: " + (operation == QuantityChangeOp::Increase ? "+ " : "- "); // Add variable name at the end
  }
};

struct ChangeItemsCommmand : IEventCommand {
  ~ChangeItemsCommmand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Items; }
  int item;
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Items : {} " + DecodeEnumName(operation) + (operandSource == QuantityChangeSource::Constant ? std::to_string(operand) : " []") + ColorFormatter::popColor();
  }
};

struct ChangeWeaponsCommmand : IEventCommand {
  ~ChangeWeaponsCommmand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Weapons; }
  int item;
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
  bool includeEquipment;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Weapons : {} " + DecodeEnumName(operation) + (operandSource == QuantityChangeSource::Constant ? std::to_string(operand) : " [] ") + ColorFormatter::popColor() + (includeEquipment == true ? ColorFormatter::getColor(Color::Gray) + "(Include Equipment)" : "");
  }
};

struct ChangeArmorsCommmand : IEventCommand {
  ~ChangeArmorsCommmand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Armors; }
  int item;
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
  bool includeEquipment;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Armors : {} " + DecodeEnumName(operation) + (operandSource == QuantityChangeSource::Constant ? std::to_string(operand) : " [] ") + ColorFormatter::popColor() + (includeEquipment == true ? ColorFormatter::getColor(Color::Gray) + "(Include Equipment)" : "");
  }
};

struct ChangePartyMemberCommand : IEventCommand {
  ~ChangePartyMemberCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Party_Member; }
  int member;
  PartyMemberOperation operation;
  bool initialize;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Party Member : " + DecodeEnumName(operation) + " {} " + ColorFormatter::popColor() + (initialize == true ? ColorFormatter::getColor(Color::Gray) + "(Initialize)" : "");
  }
};
struct ChangeBattleBGMCommand : IEventCommand {
  ~ChangeBattleBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Battle_BGM; }
  Audio bgm;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Battle BGM : " + (bgm.name == "" ? "None" : bgm.name) +
           " " + std::format("({}, {}, {})", bgm.volume, bgm.pitch, bgm.pan) + ColorFormatter::popColor();
  }
};

struct ChangeVictoryMECommand : IEventCommand {
  ~ChangeVictoryMECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Victory_ME; }
  Audio me;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Victory ME : " + (me.name == "" ? "None" : me.name) +
           " " + std::format("({}, {}, {})", me.volume, me.pitch, me.pan) + ColorFormatter::popColor();
  }
};

struct ChangeSaveAccessCommand : IEventCommand {
  ~ChangeSaveAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Save_Access; }
  AccessMode access;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Save Access : " + DecodeEnumName(access) + ColorFormatter::popColor();
  }
};

struct ChangeMenuAccessCommand : IEventCommand {
  ~ChangeMenuAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Menu_Access; }
  AccessMode access;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Menu Access : " + DecodeEnumName(access) + ColorFormatter::popColor();
  }
};

struct ChangeEncounterDisableCommand : IEventCommand {
  ~ChangeEncounterDisableCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Encounter_Disable; }
  AccessMode access;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Encounter : " + DecodeEnumName(access) + ColorFormatter::popColor();
  }
};

struct ChangeFormationAccessCommand : IEventCommand {
  ~ChangeFormationAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Formation_Access; }
  AccessMode access;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Formation Access : " + DecodeEnumName(access) + ColorFormatter::popColor();
  }
};

struct ChangeWindowColorCommand : IEventCommand {
  ~ChangeWindowColorCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Window_Color; }
  int r;
  int g;
  int b;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Window Color : " + std::format("({}, {}, {})", r, g, b) + ColorFormatter::popColor();
  }
};

struct ChangeDefeatMECommand : IEventCommand {
  ~ChangeDefeatMECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Defeat_ME; }
  Audio me;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Defeat ME : " + (me.name == "" ? "None" : me.name) +
           " " + std::format("({}, {}, {})", me.volume, me.pitch, me.pan) + ColorFormatter::popColor();
  }
};

struct ChangeVehicleBGMCommand : IEventCommand {
  ~ChangeVehicleBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Vechicle_BGM; }
  VehicleType vehicle;
  Audio me;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Vehicle BGM : " + DecodeEnumName(vehicle) + ", " + (me.name == "" ? "None" : me.name) +
           " " + std::format("({}, {}, {})", me.volume, me.pitch, me.pan) + ColorFormatter::popColor();
  }
};

struct TransferPlayerCommand : IEventCommand {
  ~TransferPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transfer_Player; }
  TransferMode mode;
  int mapId;
  int x;
  int y;
  Direction direction;
  Fade fade;

  [[nodiscard]] std::string stringRep() const override {
    std::string suffix = ColorFormatter::getColor(Gray) + (direction != Direction::Retain ? "(Direction: " + DecodeEnumName(direction) : "");
    suffix += (direction != Direction::Retain ? fade != Fade::Black ? ", Fade: " + DecodeEnumName(fade) + ")" : "(Fade: " + DecodeEnumName(fade) + ")" : "") + ColorFormatter::popColor();

    if (mode == TransferMode::Variable_Designation) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Transfer Player : {[]} ({[]},{[]})" + suffix;
    }
    else {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Transfer Player : {}" + std::format("({}, {})", x, y) + suffix;
    }
  }
};

struct SetVehicleLocationCommand : IEventCommand {
  ~SetVehicleLocationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Vehicle_Location; }
  VehicleType vehicle;
  TransferMode mode;
  int mapId;
  int x;
  int y;

  [[nodiscard]] std::string stringRep() const override {
    std::string prefix = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Set Vehicle Location : " + DecodeEnumName(vehicle) + ",";
    std::string suffix = ColorFormatter::popColor();

    if (mode == TransferMode::Variable_Designation) {
      return prefix + ", {[]} ({[]},{[]}) " + suffix;
    }
    return prefix + " {}" + std::format("({}, {})", x, y) + suffix;
  }
};
struct SetEventLocationCommand : IEventCommand {
  ~SetEventLocationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Event_Location; }
  TransferMode mode;
  int event;
  int x;
  int y;
  Direction direction;

  [[nodiscard]] std::string stringRep() const override {
    std::string prefix = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Set Event Location : " + (event > 0 ? "{}" : "This Event");
    std::string suffix = "(Direction : " + DecodeEnumName(direction) + " )" + ColorFormatter::popColor();

    if (mode == TransferMode::Variable_Designation) {
      return prefix + ", ([],[])" + suffix;
    }
    if (mode == TransferMode::Exchange_With_Another_Event) {
      return prefix + ", Exchange with " + (event > 0 ? "<>" : "This Event") + suffix;
    }
    return prefix + std::format(", ({}, {})", x, y) + suffix;
  }
};

struct ErasePictureCommand : IEventCommand {
  ~ErasePictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Erase_Picture; }
  int picture;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Erase Picture : " + std::to_string(picture) + ColorFormatter::popColor();
  }
};

struct MovePictureCommand : IEventCommand {
  ~MovePictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Picture; }
  int picture;
  PictureOrigin origin;
  PictureDesignationSource pictureLocation;
  int x;
  int y;
  int width;
  int height;
  int opacity;
  Blend blendMode;
  int duration;
  bool waitForCompletion;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Move Picture : " +
        std::format("#{}, {} ({},{}), ({}%, {}%), {}, {}, {} frames",
          picture, DecodeEnumName(origin), DecodeEnumName(pictureLocation), x, y, width, height, opacity, DecodeEnumName(blendMode), duration)
    + (waitForCompletion == true ? ColorFormatter::getColor(Gray) + " (Wait)" + ColorFormatter::popColor() : "");
  }
};

struct RotatePictureCommand : IEventCommand {
  ~RotatePictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Rotate_Picture; }
  int picture;
  int rotation;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Rotate Picture : #" + std::to_string(picture) + ", " + std::to_string(rotation) + ColorFormatter::popColor();
  }
};

struct TintPictureCommand : IEventCommand {
  ~TintPictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Tint_Picture; }
  int picture;

  struct {
    int r;
    int g;
    int b;
    int gray;
  } colors;

  int duration;
  bool waitForCompletion;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Tint Picture : " +
        std::format("#{}, ({},{},{},{}), {} frames",
          picture, colors.r, colors.g, colors.b, colors.gray, duration)
    + (waitForCompletion == true ? ColorFormatter::getColor(Gray) + " (Wait)" + ColorFormatter::popColor() : "");
  }
};

struct SetWeatherEffectCommand : IEventCommand {
  ~SetWeatherEffectCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Weather_Effect; }
  WeatherEffect effect;
  int power;
  int duration;
  bool waitForCompletion;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Set Weather Effect : " + DecodeEnumName(effect) + ", " + std::to_string(duration) + " + frames"
    + (waitForCompletion == true ? ColorFormatter::getColor(Gray) + " (Wait)" + ColorFormatter::popColor() : "");
  }
};

struct PluginCommand : IEventCommand {
  ~PluginCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Plugin_Command; }
  std::string command;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Plugin Command : " + command + ColorFormatter::popColor();
  }
};

struct ScrollMapCommand : IEventCommand {
  ~ScrollMapCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Scroll_Map; }
  Direction direction;
  int distance;
  MovementSpeed speed;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + std::format("Scroll Map : {}, {}, {}", DecodeEnumName(direction), distance, DecodeEnumName(speed)) + ColorFormatter::popColor();
  }
};

struct ChangeMapNameDisplayCommand : IEventCommand {
  ~ChangeMapNameDisplayCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Map_Name_Display; }
  ValueControl checkIfOn;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Map Name Display : " + (checkIfOn == ValueControl::ON ? "ON" : "OFF") + ColorFormatter::popColor();
  }
};

struct ChangeTileSetCommand : IEventCommand {
  ~ChangeTileSetCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Tile_Set; }
  int tileset;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Tileset : {}" + ColorFormatter::popColor();
  }
};

struct ChangeBattlebackCommand : IEventCommand {
  ~ChangeBattlebackCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Battle_Back; }
  std::string battleBack1Name;
  std::string battleBack2Name;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Battle Back : "
    + battleBack1Name + (battleBack2Name == "" ? "" : " & " + battleBack2Name) + ColorFormatter::popColor();
  }
};

struct GetLocationInfoCommand : IEventCommand {
  ~GetLocationInfoCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Get_Location_Info; }
  int variable;
  TileType type;
  LocationSource source;
  int x;
  int y;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Get Location Info : {}, " + DecodeEnumName(type) +
           (source == LocationSource::Designation_with_variables ? "({[]},{[]})" : std::format("({},{})", x, y))
    + ColorFormatter::popColor();
  }
};

struct BattleProcessingCommand : IEventCommand {
  ~BattleProcessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Battle_Processing; }
  BattleProcessType type;
  int id; // troop id if type == 0; variable id if type == 2
  bool canEscape;
  bool canLose;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Battle Processing : "
    + (type == BattleProcessType::Direct_designation ? "{}" : type == BattleProcessType::Designation_with_variables ? "{[]}" : "Same as Random Encounter")
    + ColorFormatter::popColor();
  }
};

struct IfWinCommand : IEventCommand {
  ~IfWinCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::If_Win; }

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + " If Win" + ColorFormatter::popColor();
  }
};
struct IfEscapeCommand : IEventCommand {
  ~IfEscapeCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::If_Escape; }

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + " If Escape" + ColorFormatter::popColor();
  }
};

struct IfLoseCommand : IEventCommand {
  ~IfLoseCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::If_Lose; }

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + " If Lose" + ColorFormatter::popColor();
  }
};

struct ShopProcessingGoodCommand : IEventCommand {
  ~ShopProcessingGoodCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Shop_Processing_Good; }

  ShopType type;
  int id;
  PriceType priceType;
  int price;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + " {}" + ColorFormatter::popColor();
  }
};

struct ShopProcessingCommand : IEventCommand {
  ~ShopProcessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Shop_Processing; }

  ShopType type;
  int id;
  PriceType priceType;
  int price;
  bool purchaseOnly;
  std::vector<std::shared_ptr<ShopProcessingGoodCommand>> goods;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Shop Processing : {}" + ColorFormatter::popColor();
  }
};

struct NameInputCommand : IEventCommand {
  ~NameInputCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Name_Input_Processing; }

  int actorId;
  int numChar;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Name Input Processing : {}, " + std::to_string(numChar) + (numChar > 1 ? " character" : " characters" + ColorFormatter::popColor());
  }
};

struct ChangeHPCommand : IEventCommand {
  ~ChangeHPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_HP; }

  ActorComparisonSource comparison;
  int value;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;
  bool allowKnockout;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change HP : {}, " + DecodeEnumName(quantityOp) + (quantitySource == QuantityChangeSource::Variable ? " [] " : std::to_string(quantity))
    + (allowKnockout == true ? ColorFormatter::getColor(Gray) + " (Allow Knockout)" + ColorFormatter::popColor() : "");
  }
};

struct ChangeMPCommand : IEventCommand {
  ~ChangeMPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_MP; }

  ActorComparisonSource comparison;
  int value;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change MP : {}, " + DecodeEnumName(quantityOp) + (quantitySource == QuantityChangeSource::Variable ? " {} " : std::to_string(quantity)) + ColorFormatter::popColor();
  }
};


struct ChangeTPCommand : IEventCommand {
  ~ChangeTPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_TP; }

  ActorComparisonSource comparison;
  int value;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change TP : {}, " + DecodeEnumName(quantityOp) + (quantitySource == QuantityChangeSource::Variable ? " {} " : std::to_string(quantity)) + ColorFormatter::popColor();
  }
};


struct ChangeEXPCommand : IEventCommand {
  ~ChangeEXPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_EXP; }

  ActorComparisonSource comparison;
  int value;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;
  bool showLevelUp;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change EXP : {}, " + DecodeEnumName(quantityOp) + (quantitySource == QuantityChangeSource::Variable ? " {} " : std::to_string(quantity))
    + (showLevelUp == true ? ColorFormatter::getColor(Gray) + " (Show Level Up)" + ColorFormatter::popColor() : "");
  }
};


struct ChangeLevelCommand : IEventCommand {
  ~ChangeLevelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Level; }

  ActorComparisonSource comparison;
  int value;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;
  bool showLevelUp;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Level : {}, " + DecodeEnumName(quantityOp) + (quantitySource == QuantityChangeSource::Variable ? " {} " : std::to_string(quantity))
    + (showLevelUp == true ? ColorFormatter::getColor(Gray) + " (Show Level Up)" + ColorFormatter::popColor() : "");
  }
};


struct ChangeParameterCommand : IEventCommand {
  ~ChangeParameterCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Parameter; }

  ActorComparisonSource comparison;
  int value;
  ParameterSource param;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Parameter : {}, " + DecodeEnumName(param) + " " + DecodeEnumName(quantityOp)
      + (quantitySource == QuantityChangeSource::Variable ? " {} " : std::to_string(quantity)) + ColorFormatter::popColor();
  }
};

struct RecoverAllCommand : IEventCommand {
  ~RecoverAllCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Recover_All; }

  ActorComparisonSource comparison;
  int value;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Recover All : {}" + ColorFormatter::popColor();
  }
};


struct ChangeNameCommand : IEventCommand {
  ~ChangeNameCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Name; }

  int actor;
  std::string name;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Name : {}, " + name + ColorFormatter::popColor();
  }
};


struct ChangeClassCommand : IEventCommand {
  ~ChangeClassCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Class; }

  int actor;
  int classId;
  bool saveLevel;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Class : {}, {}" + (saveLevel == true ? ColorFormatter::getColor(Gray) + " (Save Level)" + ColorFormatter::popColor() : "");
  }
};


struct ChangeStateCommand : IEventCommand {
  ~ChangeStateCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_State; }

  ActorComparisonSource comparison;
  int value;
  PartyMemberOperation stateOp;
  int state;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change State : {}, " + DecodeEnumName(stateOp) + " {}" + ColorFormatter::popColor();
  }
};


struct ChangeSkillCommand : IEventCommand {
  ~ChangeSkillCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Skill; }

  ActorComparisonSource comparison;
  int value;
  SkillOperation skillOp;
  int skill;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Skill : {}, " + DecodeEnumName(skillOp) + " {}" + ColorFormatter::popColor();
  }
};


struct ChangeEquipmentCommand : IEventCommand {
  ~ChangeEquipmentCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Equipment; }

  int actorId;
  int equipType;
  int equipment;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Equipment : {}, {} = " + (equipment > 0 ? "{}" : "None") + ColorFormatter::popColor();
  }
};

struct GameOverCommand : IEventCommand {
  ~GameOverCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Game_Over; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Game Over" + ColorFormatter::popColor();
  }
};

struct ReturnToTitleCommand : IEventCommand {
  ~ReturnToTitleCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Return_To_Title_Screen; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Return To Title Screen" + ColorFormatter::popColor();
  }
};

struct OpenMenuCommand : IEventCommand {
  ~OpenMenuCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Open_Menu_Screen; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Open Menu Screen" + ColorFormatter::popColor();
  }
};

struct OpenSaveCommand : IEventCommand {
  ~OpenSaveCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Open_Save_Screen; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Open Save Screen" + ColorFormatter::popColor();
  }
};

struct AbortBattleCommand : IEventCommand {
  ~AbortBattleCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Abort_Battle; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Abort Battle" + ColorFormatter::popColor();
  }
};

struct EnemyRecoverAllCommand : IEventCommand {
  ~EnemyRecoverAllCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Enemy_Recover_All; }
  int troop;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Enemy Recover All : " + (troop > 0 ? "#" + std::to_string(troop) + " {}" : "Entire Troop") + ColorFormatter::popColor();
  }
};

struct EnemyAppearCommand : IEventCommand {
  ~EnemyAppearCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Enemy_Appear; }
  int enemy;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Enemy Appear : #" + std::to_string(enemy) + " {}" + ColorFormatter::popColor();
  }
};

struct EnemyTransformCommand : IEventCommand {
  ~EnemyTransformCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Enemy_Transform; }
  int enemy;
  int transform;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Enemy Transform : #" + std::to_string(enemy) + " {}, {}" + ColorFormatter::popColor();
  }
};

struct ChangeEnemyHPCommand : IEventCommand {
  ~ChangeEnemyHPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_HP; }
  int enemy;
  QuantityChangeOp enemyOp;
  QuantityChangeSource quantitySource;
  int quantity;
  bool allowKnockOut;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Enemy HP : " + (enemy > 0 ? "#" + std::to_string(enemy) + " {}, " : "Entire Troop, ") + DecodeEnumName(enemyOp) + (quantitySource == QuantityChangeSource::Variable ? " {} " : std::to_string(quantity))
    + " " + (allowKnockOut == true ? ColorFormatter::getColor(Gray) + " (Allow Knockout)" + ColorFormatter::popColor() : "");
  }
};

struct ChangeEnemyMPCommand : IEventCommand {
  ~ChangeEnemyMPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_MP; }
  int enemy;
  QuantityChangeOp enemyOp;
  QuantityChangeSource quantitySource;
  int quantity;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Enemy MP : " + (enemy > 0 ? "#" + std::to_string(enemy) + " {}, " : "Entire Troop, ") + DecodeEnumName(enemyOp) + (quantitySource == QuantityChangeSource::Variable ? " {} " : std::to_string(quantity))
    + " " + ColorFormatter::popColor();
  }
};

struct ChangeEnemyTPCommand : IEventCommand {
  ~ChangeEnemyTPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_TP; }
  int enemy;
  QuantityChangeOp enemyOp;
  QuantityChangeSource quantitySource;
  int quantity;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Enemy TP : " + (enemy > 0 ? "#" + std::to_string(enemy) + " {}, " : "Entire Troop, ") + DecodeEnumName(enemyOp) + (quantitySource == QuantityChangeSource::Variable ? " {} " : std::to_string(quantity))
    + " " + ColorFormatter::popColor();
  }
};
struct ChangeEnemyStateCommand : IEventCommand {
  ~ChangeEnemyStateCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_TP; }
  int enemy;
  PartyMemberOperation enemyOp;
  int state;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Enemy State : " + (enemy > 0 ? "#" + std::to_string(enemy) + " {}, " : "Entire Troop, ") + DecodeEnumName(enemyOp) + "{} "
      + ColorFormatter::popColor();
  }
};

struct ForceActionCommand : IEventCommand {
  ~ForceActionCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Force_Action; }

  SubjectComparisonSource sourceComparison;
  int source;
  int skill;
  int target;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Force Action : "
      + (sourceComparison == SubjectComparisonSource::Enemy ? ("#" + std::to_string(source) + " {},") : "{},")
      + " {},"
      + (target == -2 ? "Last Target" : target == -1 ? "Random" : "Index " + std::to_string(target))
      + ColorFormatter::popColor();
  }
};

struct ShowBattleAnimCommand : IEventCommand {
  ~ShowBattleAnimCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Battle_Animation; }

  int enemy; // equals 0 if bool is true
  int animation;
  bool targetAllEnemies;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Show Battle Animation : "
      + (targetAllEnemies == true ? "Entire Troop, {}" : "#" + std::to_string(enemy) + " {}, {}")
      + ColorFormatter::popColor();
  }
};

struct ChangeVehicleImageCommand : IEventCommand {
  ~ChangeVehicleImageCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Vehicle_Image; }

  VehicleType vehicle;
  std::string picture;
  int pictureIndex;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Vehicle Image : " + DecodeEnumName(vehicle) + ", " + picture + std::format("({})", pictureIndex)
      + ColorFormatter::popColor();
  }
};

struct ChangeActorImageCommand : IEventCommand {
  ~ChangeActorImageCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Actor_Images; }

  int actor;
  std::string charPicture;
  int charIndex;
  std::string facePicture;
  int faceIndex;
  std::string battlerPicture;

  [[nodiscard]] std::string stringRep() const override {
    std::string charString = charPicture == "" ? "None, " : charPicture + std::format("({})", charIndex) + " ";
    std::string faceString = facePicture == "" ? "None, " : facePicture + std::format("({})", faceIndex) + " ";
    std::string battlerString = battlerPicture == "" ? "None" : facePicture;

    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Actor Images : {}, " + charPicture + facePicture + battlerPicture
      + ColorFormatter::popColor();
  }
};

struct ChangeProfileCommand : IEventCommand {
  ~ChangeProfileCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Profile; }

  int actor;
  std::string profile;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Profile : {}, " + profile
      + ColorFormatter::popColor();
  }
};

struct ChangeNickCommand : IEventCommand {
  ~ChangeNickCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Nickname; }

  int actor;
  std::string nick;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Nickname : {}, " + nick
      + ColorFormatter::popColor();
  }
};

struct ChangePlayerFollowersCommand : IEventCommand {
  ~ChangePlayerFollowersCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Player_Followers; }

  bool followersEnabled;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Player Followers : " + (followersEnabled == true ? "ON" : "OFF")
      + ColorFormatter::popColor();
  }
};

struct GatherFollowersCommand : IEventCommand {
  ~GatherFollowersCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Gather_Followers; }

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Gather Followers"
      + ColorFormatter::popColor();
  }
};

struct FadeOutScreenCommand : IEventCommand {
  ~FadeOutScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Fade_Out_Screen; }

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Fadeout Screen"
      + ColorFormatter::popColor();
  }
};

struct FadeInScreenCommand : IEventCommand {
  ~FadeInScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Fade_In_Screen; }

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Fadein Screen"
      + ColorFormatter::popColor();
  }
};

struct TintScreenCommand : IEventCommand {
  ~TintScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Tint_Screen; }
  struct {
    int r;
    int g;
    int b;
    int gray;
  } colors;
  int duration;
  bool waitForCompletion;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Tint Screen : " +
        std::format("({},{},{},{}), {} frames",
          colors.r, colors.g, colors.b, colors.gray, duration)
    + (waitForCompletion == true ? ColorFormatter::getColor(Gray) + " (Wait)" + ColorFormatter::popColor() : "");
  }
};

struct FlashScreenCommand : IEventCommand {
  ~FlashScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Flash_Screen; }
  struct {
    int r;
    int g;
    int b;
    int intensity;
  } colors;
  int duration;
  bool waitForCompletion;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Flash Screen : " +
        std::format("({},{},{},{}), {} frames",
          colors.r, colors.g, colors.b, colors.intensity, duration)
    + (waitForCompletion == true ? ColorFormatter::getColor(Gray) + " (Wait)" + ColorFormatter::popColor() : "");
  }
};

struct ShakeScreenCommand : IEventCommand {
  ~ShakeScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Shake_Screen; }
  int power;
  int speed;
  int duration;
  bool waitForCompletion;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Shake Screen : " + std::format("{}, {}, {} frames", power, speed, duration)
    + (waitForCompletion == true ? ColorFormatter::getColor(Gray) + " (Wait)" + ColorFormatter::popColor() : "");
  }
};
struct SetMovementRouteCommand : IEventCommand {
  ~SetMovementRouteCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Movement_Route; }
  int character;
  MovementRoute route;
  std::vector<std::shared_ptr<IEventCommand>> editNodes;
  [[nodiscard]] std::string stringRep() const override {

    std::string characterName = character == -1 ? "Player" : character == 0 ? "This Event" : "{}";
    std::string stringSuffix = "(";
    stringSuffix += route.repeat == true ? "Repeat" : "";
    stringSuffix += route.skippable == true ?  (route.repeat == true ? ", Skip" : "Skip") : "";
    stringSuffix += route.wait == true ? ((route.repeat == true || route.skippable == true) ? ", Wait" : "") : "Wait";
    stringSuffix += ")";

    std::string moveRoute = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Set Movement Route : " + characterName + ColorFormatter::popColor() +
                            ColorFormatter::getColor(Color::Gray) + stringSuffix + ColorFormatter::popColor();

    for (const auto& t : route.list) {
      if (t->code() != EventCode::Event_Dummy) {
        moveRoute += "\n" + std::string(indent ? *indent : 0, '\t') + " :" + std::string(((t->indent ? *t->indent : 0) + 1), '\t') +
               " : " + t->stringRep();
      }
    }
    return moveRoute;

  }

};

struct MovementRouteStepCommand : IEventCommand {
  ~MovementRouteStepCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Movement_Route_Step; }
  int character;
  std::shared_ptr<IEventCommand> step;
};

struct GetOnOffVehicleCommand : IEventCommand {
  ~GetOnOffVehicleCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Get_On_Off_Vehicle; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Get on/off Vehicle" + ColorFormatter::popColor();
  }
};

struct ChangeTransparencyCommand : IEventCommand {
  ~ChangeTransparencyCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Transparency; }
  int transparency;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Transparency : " + (transparency == 0 ? "Disable" : "Enable") + ColorFormatter::popColor();
  }
};

struct ShowAnimationCommand : IEventCommand {
  ~ShowAnimationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Animation; }
  int character;
  int animation;
  bool waitForCompletion;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Show Animation : " + (character == -1 ? "Player" : character == 0 ? "This  Event" : "{}, ")+ ColorFormatter::popColor()
    + (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + "(Wait)" + ColorFormatter::popColor() : "");
  }
};

struct ShowBalloonIconCommand : IEventCommand {
  ~ShowBalloonIconCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Balloon_Icon; }
  int id;
  int index;
  bool waitForCompletion;
  // TODO:
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
      + "Show Balloon Icon :" + (id > 0 ? " {}, " : id == -2 ? " Player, " : " This Event, ")
      + (waitForCompletion == true ? ColorFormatter::getColor(Gray) + " (Wait)" + ColorFormatter::popColor() : "");
  }
};

struct EraseEventCommand : IEventCommand {
  ~EraseEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Erase_Event; }[[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Erase Event" + ColorFormatter::popColor();
  }
};

struct NextScriptCommand : IEventCommand {
  ~NextScriptCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Script; }
  std::string script;
};

struct ScriptCommand : IEventCommand {
  ~ScriptCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Script; }
  std::string script;
  std::vector<std::shared_ptr<NextScriptCommand>> moreScript;
  [[nodiscard]] std::string stringRep() const override {
    std::string ret = std::string(indent ? *indent : 0, '\t') + "◇Script: " + script;
    for (const auto& t : moreScript) {
      // TODO: How do we do this
      if (!ret.empty()) {
        ret += "\n";
      }
      ret += std::string(indent ? *indent : 0, '\t') + " :" + std::string(((t->indent ? *t->indent : 0) + 1), '\t') +
             " : " + t->script;
    }

    return ret;
  }
};
struct WaitCommand : IEventCommand {
  int duration;
  ~WaitCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Wait; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Wait : " + std::to_string(duration) + " frames";
  }
};

struct PlaySECommand : IEventCommand {
  ~PlaySECommand() override = default;
  Audio audio;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_SE; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Play SE : " + (audio.name == "" ? "None" : audio.name) + " " +
           std::format("({}, {}, {})", audio.volume, audio.pitch, audio.pan);
  }
};
struct PlayMECommand : IEventCommand {
  ~PlayMECommand() override = default;
  Audio audio;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_ME; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Play ME : " + (audio.name == "" ? "None" : audio.name) + " " +
           std::format("({}, {}, {})", audio.volume, audio.pitch, audio.pan);
  }
};
struct PlayBGSCommand : IEventCommand {
  ~PlayBGSCommand() override = default;
  Audio audio;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_BGS; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Play BGS : " + (audio.name == "" ? "None" : audio.name) +
           " " + std::format("({}, {}, {})", audio.volume, audio.pitch, audio.pan);
  }
};
struct PlayBGMCommand : IEventCommand {
  ~PlayBGMCommand() override = default;
  Audio audio;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_BGM; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Play BGM : " + (audio.name == "" ? "None" : audio.name) +
           " " + std::format("({}, {}, {})", audio.volume, audio.pitch, audio.pan);
  }
};
struct PlayMovieCommand : IEventCommand {
  ~PlayMovieCommand() override = default;
  std::string name;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_Movie; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Play Movie : " + (name == "" ? "None" : name);
  }
};
struct SaveBGMCommand : IEventCommand {
  ~SaveBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Save_BGM; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Save BGM";
  }
};
struct ResumeBGMCommand : IEventCommand {
  ~ResumeBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Resume_BGM; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Resume BGM";
  }
};
struct FadeoutBGM : IEventCommand {
  ~FadeoutBGM() override = default;
  int duration;
  [[nodiscard]] EventCode code() const override { return EventCode::Fade_Out_BGM; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Fadeout BGM :" + std::to_string(duration) + " seconds";
  }
};
struct FadeoutBGS : IEventCommand {
  ~FadeoutBGS() override = default;
  int duration;
  [[nodiscard]] EventCode code() const override { return EventCode::Fade_Out_BGS; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Fadeout BGS :" + std::to_string(duration) + " seconds";
  }
};
struct StopSECommand : IEventCommand {
  ~StopSECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Stop_SE; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Stop SE";
  }
};

struct ShowPictureCommand : IEventCommand {
  ~ShowPictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Picture; }
  int number;
  std::string imageName;
  PictureOrigin origin;
  PictureDesignationSource type;
  int value1; // direct X value or indirect from global variables
  int value2; // direct Y value or indirect from global variables
  int zoomX;
  int zoomY;
  int opacityValue;
  Blend blendMode;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Show Picture : " +
        std::format("#{}, {}, {} ({},{}), ({}%, {}%), {}, {}, {} frames",
          number, imageName, DecodeEnumName(origin), DecodeEnumName(type), value1, value2, zoomX, zoomY, opacityValue, DecodeEnumName(blendMode))
    + ColorFormatter::popColor();
  }
};

struct EndCommand : IEventCommand {
  ~EndCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End; }
  [[nodiscard]] std::string stringRep() const override { return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "End"; }
};

// START MOVEMENTROUTE

struct MovementMoveDownCommand : IEventCommand {
  ~MovementMoveDownCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Down; }
};

struct MovementMoveLeftCommand : IEventCommand {
  ~MovementMoveLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Left; }
};

struct MovementMoveRightCommand : IEventCommand {
  ~MovementMoveRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Right; }
};

struct MovementMoveUpCommand : IEventCommand {
  ~MovementMoveUpCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Up; }
};

struct MovementMoveLowerLeftommand : IEventCommand {
  ~MovementMoveLowerLeftommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Lower_Left; }
};
struct MovementMoveLowerRightCommand : IEventCommand {
  ~MovementMoveLowerRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Lower_Right; }
};
struct MovementMoveUpperLeftCommand : IEventCommand {
  ~MovementMoveUpperLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Upper_Left; }
};
struct MovementMoveUpperRightCommand : IEventCommand {
  ~MovementMoveUpperRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Upper_Right; }
};
struct MovementMoveAtRandomCommand : IEventCommand {
  ~MovementMoveAtRandomCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_at_Random; }
};
struct MovementMoveTowardPlayerCommand : IEventCommand {
  ~MovementMoveTowardPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_toward_Player; }
};
struct MovementMoveAwayFromPlayerCommand : IEventCommand {
  ~MovementMoveAwayFromPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_away_from_Player; }
};
struct MovementMove1StepFowardCommand : IEventCommand {
  ~MovementMove1StepFowardCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::_1_Step_Forward; }
};
struct MovementMove1StepBackwardCommand : IEventCommand {
  ~MovementMove1StepBackwardCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::_1_Step_Backward; }
};
struct MovementJumpCommand : IEventCommand {
  ~MovementJumpCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Jump; }
  int x;
  int y;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + std::format("◇Jump {}, {}", x, y) + ColorFormatter::popColor();
  }
};
struct MovementWaitCommand : IEventCommand {
  ~MovementWaitCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Wait_del_; }
  int duration;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Wait " + std::to_string(duration) + " frames" + ColorFormatter::popColor();
  }
};

struct MovementTurnDownCommand : IEventCommand {
  ~MovementTurnDownCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Down; }
};

struct MovementTurnLeftCommand : IEventCommand {
  ~MovementTurnLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Left; }
};
struct MovementTurnRightCommand : IEventCommand {
  ~MovementTurnRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Right; }
};
struct MovementTurnUpCommand : IEventCommand {
  ~MovementTurnUpCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Up; }
};
struct MovementTurn90DegRightCommand : IEventCommand {
  ~MovementTurn90DegRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_90_deg_Left; }
};
struct MovementTurn90DegLeftCommand : IEventCommand {
  ~MovementTurn90DegLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_90_deg_Right; }
};

struct MovementTurn180DegCommand : IEventCommand {
  ~MovementTurn180DegCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_180_deg; }
};
struct MovementTurn90DegLeftOrRightCommand : IEventCommand {
  ~MovementTurn90DegLeftOrRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_90_deg_Left_or_Right; }
};
struct MovementTurnAtRandomCommand : IEventCommand {
  ~MovementTurnAtRandomCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_at_Random; }
};
struct MovementTurnTowardPlayerCommand : IEventCommand {
  ~MovementTurnTowardPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_toward_Player; }
};

struct MovementAwayFromPlayerCommand : IEventCommand {
  ~MovementAwayFromPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_away_from_Player; }
};
struct MovementSwitchONCommand : IEventCommand {
  ~MovementSwitchONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Switch_ON; }
  int id;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Switch " + std::format("{:04}", id) + " ON" + ColorFormatter::popColor();
  }
};

struct MovementSwitchOFFCommand : IEventCommand {
  ~MovementSwitchOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Switch_OFF; }
  int id;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Switch " + std::format("{:04}", id) + " OFF" + ColorFormatter::popColor();
  }
};

struct MovementSpeedCommand : IEventCommand {
  ~MovementSpeedCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Speed; }
  int speed;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Speed : " + std::to_string(speed) + ColorFormatter::popColor();
  }
};

struct MovementFrequencyCommand : IEventCommand {
  ~MovementFrequencyCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
  int frequency;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Frequency " + std::to_string(frequency) + ColorFormatter::popColor();
  }
};

struct MovementWalkingAnimationONCommand : IEventCommand {
  ~MovementWalkingAnimationONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
};

struct MovementWalkingAnimationOFFCommand : IEventCommand {
  ~MovementWalkingAnimationOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
};

struct MovementSteppingAnimationONCommand : IEventCommand {
  ~MovementSteppingAnimationONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
};

struct MovementSteppingAnimationOFFCommand : IEventCommand {
  ~MovementSteppingAnimationOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
};
struct MovementDirectionFixONCommand : IEventCommand {
  ~MovementDirectionFixONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Direction_Fix_ON; }
};

struct MovementDirectionFixOFFCommand : IEventCommand {
  ~MovementDirectionFixOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Direction_Fix_OFF; }
};

struct MovementThroughONCommand : IEventCommand {
  ~MovementThroughONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Through_ON; }
};
struct MovementThroughOFFCommand : IEventCommand {
  ~MovementThroughOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Through_OFF; }
};

struct MovementTransparentONCommand : IEventCommand {
  ~MovementTransparentONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transparent_ON; }
};

struct MovementTransparentOFFCommand : IEventCommand {
  ~MovementTransparentOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transparent_OFF; }
};
struct MovementChangeImageCommand : IEventCommand {
  ~MovementChangeImageCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Image; }

  std::string image;
  int character;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Image : " + image + " (" +  std::to_string(character) + ")" + ColorFormatter::popColor();
  }
};

struct MovementChangeOpacityCommand : IEventCommand {
  ~MovementChangeOpacityCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Opacity; }

  int opacity;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Opacity : " + std::to_string(opacity) + ColorFormatter::popColor();
  }
};

struct MovementChangeBlendModeCommand : IEventCommand {
  ~MovementChangeBlendModeCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Blend_Mode; }

  Blend mode;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Blend Mode : " + DecodeEnumName(mode) + ColorFormatter::popColor();
  }
};

struct MovementPlaySECommand : IEventCommand {
  ~MovementPlaySECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_SE_del_Movement; }
  Audio se;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "SE : " + (se.name == "" ? "None" : se.name) + " "
    + std::format("({}, {}, {})", se.volume, se.pitch, se.pan) + ColorFormatter::popColor();
  }
};

struct MovementScriptCommand : IEventCommand {
  ~MovementScriptCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Script_del_Movement; }
  std::string script;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Script : " + script + ColorFormatter::popColor();
  }
};

// END MOVEMENTROUTE
struct CommandParser {
  nlohmann::json parser;
  int index = 0;

  std::vector<std::shared_ptr<IEventCommand>> parse(const nlohmann::json& data);
  void serialize(nlohmann::json& data, const std::vector<std::shared_ptr<IEventCommand>>& list);

  EventCode nextEventCommand() {
    EventCode ret;
    parser[index + 1].at("code").get_to(ret);
    return ret;
  }
  nlohmann::json currentCommand() { return parser[index]; }

  struct create;
};