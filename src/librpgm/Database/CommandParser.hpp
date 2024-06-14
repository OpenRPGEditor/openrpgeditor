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
  virtual ~IEventCommand() = default;
  [[nodiscard]] virtual EventCode code() const = 0;
  [[nodiscard]] virtual std::string stringRep() const {
    return std::string(indent ? *indent * 4 : 0, ' ') + DecodeEnumName(code());
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
    return std::string(indent ? *indent * 4 : 0, ' ') + "&push-color=0,255,0;◇&pop-color;";
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

  std::vector<std::shared_ptr<NextTextCommand>> text;

  // TODO
};

struct ShowChoiceCommand : IEventCommand {
  ~ShowChoiceCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Text; }
  TextBackground background;
  ChoiceWindowPosition positionType;
  std::vector<std::string> choices;
  int cancelType;
  int defaultType;

  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "◇Show Choices : " + std::accumulate(choices.begin(), choices.end(), std::string{})


      + ColorFormatter::popColor();
  }
};

struct WhenSelectedCommand : IEventCommand {
  ~WhenSelectedCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Selected; }

  int param1;
  std::string choice;

  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + ": When " + choice + ColorFormatter::popColor();
  }
};

struct WhenCancelCommand : IEventCommand {
  ~WhenCancelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Cancel; }
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + ": When Cancel" + ColorFormatter::popColor();
  }
};

struct ShowChoicesEndCommand : IEventCommand {
  ~ShowChoicesEndCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End_del_ShowChoices; }
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + ": End" + ColorFormatter::popColor();
  }
};

struct InputNumberCommand : IEventCommand {
  ~InputNumberCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Input_Number; }
  int variable;
  int digits;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "◇Select Item : {}, " + (digits > 1 ? " digits" : " digit") + ColorFormatter::popColor();
  }
};

struct SelectItemCommand : IEventCommand {
  ~SelectItemCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Select_Item; }
  int item;
  ItemType type;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "◇Select Item : {}, " + DecodeEnumName(type) + ColorFormatter::popColor();
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

  // TODO
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
    SwitchControl checkIfOn;
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
    SwitchControl checkIfOn;
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
      std::string test = ColorFormatter::getColorCode(DecodeEnumName(code()));
      return std::string(indent ? *indent * 4 : 0, ' ') + "&push-color=255,255,0;◇If&pop-color; " + "{} " +
             DecodeEnumName(variable.comparison) + " " +
             (variable.source == VariableComparisonSource::Constant ? std::to_string(variable.constant) : "{}");
    }
    if (type == ConditionType::Switch) {
      return std::string(indent ? *indent * 4 : 0, ' ') + "◇If " + "{}" + " is " +
             (globalSwitch.checkIfOn == SwitchControl::OFF ? "OFF" : "ON");
    }
    if (type == ConditionType::Self_Switch) {
      return std::string(indent ? *indent * 4 : 0, ' ') + "◇If Self Switch " + "{}" + " is " +
             (selfSwitch.checkIfOn == SwitchControl::OFF ? "OFF" : "ON");
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
      return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
             "◇If Timer" + ColorFormatter::popColor() + " " + DecodeEnumName(timer.comparison) + " " + min + " min " +
             sec + " sec";
    }
    if (type == ConditionType::Actor) {
      if (actor.type == ActorConditionType::Name) {
        return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
               "◇If Name of {} is []" + ColorFormatter::popColor();
      } else if (actor.type == ActorConditionType::Class) {
        return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
               "◇If Class of {} is []" + ColorFormatter::popColor();
      } else if (actor.type == ActorConditionType::Skill) {
        return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
               "◇If {} has learned []" + ColorFormatter::popColor();
      } else if (actor.type == ActorConditionType::Weapon || actor.type == ActorConditionType::Armor) {
        return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
               "◇If {} has equipped []" + ColorFormatter::popColor();
      } else if (actor.type == ActorConditionType::State) {
        return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
               "◇If {} is affected by []" + ColorFormatter::popColor();
      } else {
        return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
               "◇If {} is in party" + ColorFormatter::popColor();
      }
    }
    if (type == ConditionType::Enemy) {
      if (enemy.type == EnemyConditionType::State) {
        return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
               "◇If {} is affected by []" + ColorFormatter::popColor();
      }
      return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
             "◇If {} is appeared" + ColorFormatter::popColor();
    }
    if (type == ConditionType::Character) {
      std::string direction = character.facing == Direction::Up     ? "Up"
                              : character.facing == Direction::Down ? "Down"
                              : character.facing == Direction::Left ? "Left"
                                                                    : "Right";
      return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
             "◇If {} is facing " + direction + ColorFormatter::popColor();
    }
    if (type == ConditionType::Vehicle) {
      return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
             "◇If {} is driven" + ColorFormatter::popColor();
    }
    if (type == ConditionType::Gold) {
      return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
             "◇If Gold" + " " + DecodeEnumName<GoldComaprisonType>(gold.type) + " {}";
    }
    if (type == ConditionType::Item) {
      return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
             "◇If Party has {}" + ColorFormatter::popColor();
    }
    if (type == ConditionType::Weapon || type == ConditionType::Armor) {
      return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
             "◇If Party has {}" + ColorFormatter::popColor() + ColorFormatter::getColor(Color::Gray) +
             " (Include Equipment)" + ColorFormatter::popColor();
    }
    if (type == ConditionType::Button) {
      return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
             "◇If Button [" + "{}" + "] is pressed down" + ColorFormatter::popColor();
    }
    if (type == ConditionType::Script) {
      return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
             "◇If Script : " + ColorFormatter::popColor();
    }
    return std::string(indent ? *indent * 4 : 0, ' ') +
           "◇ &push-color=255,0,255;Condition&pop-color; &push-color=0,255,0;TBD&pop-color;";
  }
};

struct ElseCommand : IEventCommand {
  ~ElseCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Else; }
  [[nodiscard]] std::string stringRep() const override { return std::string(indent ? *indent * 4 : 0, ' ') + ":Else"; }
};

struct LoopCommand : IEventCommand {
  ~LoopCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Loop; }
  [[nodiscard]] std::string stringRep() const override { return std::string(indent ? *indent * 4 : 0, ' ') + "◇Loop"; }
};

struct RepeatAboveCommand : IEventCommand {
  ~RepeatAboveCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Repeat_Above; }
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ":Repeat Above";
  }
};

struct BreakLoopCommand : IEventCommand {
  ~BreakLoopCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Break_Loop; }
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + "◇Break Loop";
  }
};
struct ExitEventProecessingCommand : IEventCommand {
  ~ExitEventProecessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Exit_Event_Processing; }
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + "◇Exit Event Processing";
  }
};

struct CommonEventCommand : IEventCommand {
  ~CommonEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Common_Event; }
  int event;

  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + "◇Common Event: ";
  }
};

struct LabelCommand : IEventCommand {
  ~LabelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Label; }
  std::string label;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + "◇Label: " + label;
  }
};

struct JumpToLabelCommand : IEventCommand {
  ~JumpToLabelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Jump_To_Label; }
  std::string label;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + "◇Jump to Label: " + label;
  }
};

struct ControlSwitches : IEventCommand {
  ~ControlSwitches() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Switches; }
  int start;
  int end;
  SwitchControl turnOff;

  [[nodiscard]] std::string stringRep() const override {
    if (start != end) {
      return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
             "◇Control Switches : #" + std::format("{:04}", start) + ".." + std::format("{:04}", end) + " = " +
             (turnOff == SwitchControl::ON ? "ON" : "OFF") + ColorFormatter::popColor();
    } else {
      return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
             "◇Control Switches : #" + std::format("{:04}", start) + " {}" + " = " +
             (turnOff == SwitchControl::ON ? "ON" : "OFF") + ColorFormatter::popColor();
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
      strBuild = std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
                 "◇Control Variables : #" + std::format("{:04}", start) + "..#" + std::format("{:04}", end) + " " +
                 DecodeEnumName(operation) + " ";
    } else {
      strBuild = std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) +
                 "◇Control Variables : #" + std::format("{:04}", start) + " {} " + DecodeEnumName(operation) + " ";
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
  SwitchControl turnOff;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') +
           ColorFormatter::getColorCode(DecodeEnumName(code())) + "◇Control Self Switch : " + selfSw + " is " + DecodeEnumName(turnOff) + ColorFormatter::popColor();
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
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "◇Control Timer : " + DecodeEnumName(control) + ", " + min + " min " +
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
      return std::string(indent ? *indent * 4 : 0, ' ') +
             "◇Change Gold: " + (operation == QuantityChangeOp::Increase ? "+ " : "- ") + std::to_string(operand);
    return std::string(indent ? *indent * 4 : 0, ' ') +
           "◇Change Gold: " + (operation == QuantityChangeOp::Increase ? "+ " : "- "); // Add variable name at the end
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
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "◇Change Items : {} " + DecodeEnumName(operation) + (operandSource == QuantityChangeSource::Constant ? std::to_string(operand) : " []") + ColorFormatter::popColor();
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
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "◇Change Weapons : {} " + DecodeEnumName(operation) + (operandSource == QuantityChangeSource::Constant ? std::to_string(operand) : " [] ") + ColorFormatter::popColor() + (includeEquipment == true ? ColorFormatter::getColor(Color::Gray) + "(Include Equipment)" : "");
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
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "◇Change Armors : {} " + DecodeEnumName(operation) + (operandSource == QuantityChangeSource::Constant ? std::to_string(operand) : " [] ") + ColorFormatter::popColor() + (includeEquipment == true ? ColorFormatter::getColor(Color::Gray) + "(Include Equipment)" : "");
  }
};

struct ChangePartyMemberCommand : IEventCommand {
  ~ChangePartyMemberCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Party_Member; }
  int member;
  PartyMemberOperation operation;
  bool initialize;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "◇Change Party Member : " + DecodeEnumName(operation) + " {} " + ColorFormatter::popColor() + (initialize == true ? ColorFormatter::getColor(Color::Gray) + "(Initialize)" : "");
  }
};
struct ChangeBattleBGMCommand : IEventCommand {
  ~ChangeBattleBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Battle_BGM; }
  Audio bgm;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) + "◇Change Battle BGM : " + (bgm.name == "" ? "None" : bgm.name) +
           " " + std::format("({}, {}, {})", bgm.volume, bgm.pitch, bgm.pan) + ColorFormatter::popColor();
  }
};

struct ChangeVictoryMECommand : IEventCommand {
  ~ChangeVictoryMECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Victory_ME; }
  Audio me;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) + "◇Change Victory ME : " + (me.name == "" ? "None" : me.name) +
           " " + std::format("({}, {}, {})", me.volume, me.pitch, me.pan) + ColorFormatter::popColor();
  }
};

struct ChangeSaveAccessCommand : IEventCommand {
  ~ChangeSaveAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Save_Access; }
  AccessMode access;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "◇Change Save Access : " + DecodeEnumName(access) + ColorFormatter::popColor();
  }
};

struct ChangeMenuAccessCommand : IEventCommand {
  ~ChangeMenuAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Menu_Access; }
  AccessMode access;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "◇Change Menu Access : " + DecodeEnumName(access) + ColorFormatter::popColor();
  }
};

struct ChangeEncounterDisableCommand : IEventCommand {
  ~ChangeEncounterDisableCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Encounter_Disable; }
  AccessMode access;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "◇Change Encounter : " + DecodeEnumName(access) + ColorFormatter::popColor();
  }
};

struct ChangeFormationAccessCommand : IEventCommand {
  ~ChangeFormationAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Formation_Access; }
  AccessMode access;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "◇Change Formation Access : " + DecodeEnumName(access) + ColorFormatter::popColor();
  }
};

struct ChangeWindowColorCommand : IEventCommand {
  ~ChangeWindowColorCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Window_Color; }
  int r;
  int g;
  int b;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "◇Change Window Color : " + std::format("({}, {}, {})", r, g, b) + ColorFormatter::popColor();
  }
};

struct ChangeDefeatMECommand : IEventCommand {
  ~ChangeDefeatMECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Defeat_ME; }
  Audio me;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code())) + "◇Change Defeat ME : " + (me.name == "" ? "None" : me.name) +
           " " + std::format("({}, {}, {})", me.volume, me.pitch, me.pan) + ColorFormatter::popColor();
  }
};

struct ChangeVehicleBGMCommand : IEventCommand {
  ~ChangeVehicleBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Vechicle_BGM; }
  VehicleType vehicle;
  Audio me;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "◇Change Vehicle BGM : " + DecodeEnumName(vehicle) + ", " + (me.name == "" ? "None" : me.name) +
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
      return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
      + "◇Transfer Player : {[]} ({[]},{[]})" + suffix;
    }
    else {
      return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
      + "◇Transfer Player : {}" + std::format("({}, {})", x, y) + suffix;
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
    std::string prefix = std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
      + "◇Set Vehicle Location : " + DecodeEnumName(vehicle) + ",";
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
    std::string prefix = std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
      + "◇Set Event Location : " + (event > 0 ? "{}" : "This Event");
    std::string suffix = "(Direction : " + DecodeEnumName(direction) + " )" + ColorFormatter::popColor();

    if (mode == TransferMode::Variable_Designation) {
      return prefix + ", ([],[])" + suffix;
    }
    if (mode == TransferMode::Exchange_With_Another_Event) {
      return prefix + ", Exchange with " + (event > 0 ? "<>" : "This Event") + suffix;
    }
    return  prefix + std::format(", ({}, {})", x, y) + suffix;
  }
};

struct ErasePictureCommand : IEventCommand {
  ~ErasePictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Erase_Picture; }
  int picture;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "Erase Picture : " + std::to_string(picture) + ColorFormatter::popColor();
  }
};

struct ScrollMapCommand : IEventCommand {
  ~ScrollMapCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Scroll_Map; }
  Direction direction;
  int distance;
  MovementSpeed speed;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + std::format("◇Scroll Map : {}, {}, {}", DecodeEnumName(direction), distance, DecodeEnumName(speed)) + ColorFormatter::popColor();
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

    std::string moveRoute = std::string(indent ? *indent * 4 : 0, ' ') +
                            ColorFormatter::getColorCode(DecodeEnumName(code())) +
                            "◇Set Movement Route : " + characterName + ColorFormatter::popColor() +
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
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "◇Get on/off Vehicle" + ColorFormatter::popColor();
  }
};

struct ChangeTransparencyCommand : IEventCommand {
  ~ChangeTransparencyCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Transparency; }
  int transparency;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "◇Change Transparency : " + (transparency == 0 ? "Disable" : "Enable") + ColorFormatter::popColor();
  }
};

struct ShowAnimationCommand : IEventCommand {
  ~ShowAnimationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Animation; }
  int character;
  int animation;
  bool waitForCompletion;
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "◇Show Animation : " + (character == -1 ? "Player" : character == 0 ? "This  Event" : "{}, ")+ ColorFormatter::popColor()
    + (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + "(Wait)" + ColorFormatter::popColor() : "");
  }
};

struct EraseEventCommand : IEventCommand {
  ~EraseEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Erase_Event; }[[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + ColorFormatter::getColorCode(DecodeEnumName(code()))
    + "◇Erase Event" + ColorFormatter::popColor();
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
    return std::string(indent ? *indent * 4 : 0, ' ') + "◇Wait : " + std::to_string(duration) + " frames";
  }
};

struct PlaySECommand : IEventCommand {
  ~PlaySECommand() override = default;
  Audio audio;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_SE; }
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + "◇Play SE : " + (audio.name == "" ? "None" : audio.name) + " " +
           std::format("({}, {}, {})", audio.volume, audio.pitch, audio.pan);
  }
};
struct PlayMECommand : IEventCommand {
  ~PlayMECommand() override = default;
  Audio audio;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_ME; }
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + "◇Play ME : " + (audio.name == "" ? "None" : audio.name) + " " +
           std::format("({}, {}, {})", audio.volume, audio.pitch, audio.pan);
  }
};
struct PlayBGSCommand : IEventCommand {
  ~PlayBGSCommand() override = default;
  Audio audio;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_BGS; }
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + "◇Play BGS : " + (audio.name == "" ? "None" : audio.name) +
           " " + std::format("({}, {}, {})", audio.volume, audio.pitch, audio.pan);
  }
};
struct PlayBGMCommand : IEventCommand {
  ~PlayBGMCommand() override = default;
  Audio audio;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_BGM; }
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + "◇Play BGM : " + (audio.name == "" ? "None" : audio.name) +
           " " + std::format("({}, {}, {})", audio.volume, audio.pitch, audio.pan);
  }
};
struct PlayMovieCommand : IEventCommand {
  ~PlayMovieCommand() override = default;
  std::string name;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_Move; }
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + "◇Play Movie : " + (name == "" ? "None" : name);
  }
};
struct SaveBGMCommand : IEventCommand {
  ~SaveBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Save_BGM; }
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + "◇Save BGM";
  }
};
struct ResumeBGMCommand : IEventCommand {
  ~ResumeBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Resume_BGM; }
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + "◇Resume BGM";
  }
};
struct FadeoutBGM : IEventCommand {
  ~FadeoutBGM() override = default;
  int duration;
  [[nodiscard]] EventCode code() const override { return EventCode::Fade_Out_BGM; }
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + "◇Fadeout BGM :" + std::to_string(duration) + " seconds";
  }
};
struct FadeoutBGS : IEventCommand {
  ~FadeoutBGS() override = default;
  int duration;
  [[nodiscard]] EventCode code() const override { return EventCode::Fade_Out_BGS; }
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + "◇Fadeout BGS :" + std::to_string(duration) + " seconds";
  }
};
struct StopSECommand : IEventCommand {
  ~StopSECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Stop_SE; }
  [[nodiscard]] std::string stringRep() const override {
    return std::string(indent ? *indent * 4 : 0, ' ') + "◇Stop SE";
  }
};

struct EndCommand : IEventCommand {
  ~EndCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End; }
  [[nodiscard]] std::string stringRep() const override { return std::string(indent ? *indent * 4 : 0, ' ') + ":End"; }
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
    return ColorFormatter::getColorCode(DecodeEnumName(code()))
    + std::string(indent ? *indent * 4 : 0, ' ') + std::format("◇Jump {}, {}", x, y) + ColorFormatter::popColor();
  }
};
struct MovementWaitCommand : IEventCommand {
  ~MovementWaitCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Wait_del_; }
  int duration;
  [[nodiscard]] std::string stringRep() const override {
    return ColorFormatter::getColorCode(DecodeEnumName(code()))
    + std::string(indent ? *indent * 4 : 0, ' ') + "◇Wait " + std::to_string(duration) + " frames" + ColorFormatter::popColor();
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
    return ColorFormatter::getColorCode(DecodeEnumName(code()))
    + std::string(indent ? *indent * 4 : 0, ' ') + "◇Switch " + std::format("{:04}", id) + " ON" + ColorFormatter::popColor();
  }
};

struct MovementSwitchOFFCommand : IEventCommand {
  ~MovementSwitchOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Switch_OFF; }
  int id;
  [[nodiscard]] std::string stringRep() const override {
    return ColorFormatter::getColorCode(DecodeEnumName(code()))
    + std::string(indent ? *indent * 4 : 0, ' ') + "◇Switch " + std::format("{:04}", id) + " OFF" + ColorFormatter::popColor();
  }
};

struct MovementSpeedCommand : IEventCommand {
  ~MovementSpeedCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Speed; }
  int speed;
  [[nodiscard]] std::string stringRep() const override {
    return ColorFormatter::getColorCode(DecodeEnumName(code()))
    + std::string(indent ? *indent * 4 : 0, ' ') + "◇Speed : " + std::to_string(speed) + ColorFormatter::popColor();
  }
};

struct MovementFrequencyCommand : IEventCommand {
  ~MovementFrequencyCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
  int frequency;
  [[nodiscard]] std::string stringRep() const override {
    return ColorFormatter::getColorCode(DecodeEnumName(code()))
    + std::string(indent ? *indent * 4 : 0, ' ') + "◇Frequency " + std::to_string(frequency) + ColorFormatter::popColor();
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
    return ColorFormatter::getColorCode(DecodeEnumName(code()))
    + std::string(indent ? *indent * 4 : 0, ' ') + "◇Image : " + image + " (" +  std::to_string(character) + ")" + ColorFormatter::popColor();
  }
};

struct MovementChangeOpacityCommand : IEventCommand {
  ~MovementChangeOpacityCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Opacity; }

  int opacity;
  [[nodiscard]] std::string stringRep() const override {
    return ColorFormatter::getColorCode(DecodeEnumName(code()))
    + std::string(indent ? *indent * 4 : 0, ' ') + "◇Opacity : " + std::to_string(opacity) + ColorFormatter::popColor();
  }
};

struct MovementChangeBlendModeCommand : IEventCommand {
  ~MovementChangeBlendModeCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Blend_Mode; }

  Blend mode;
  [[nodiscard]] std::string stringRep() const override {
    return ColorFormatter::getColorCode(DecodeEnumName(code()))
    + std::string(indent ? *indent * 4 : 0, ' ') + "◇Blend Mode : " + DecodeEnumName(mode) + ColorFormatter::popColor();
  }
};

struct MovementPlaySECommand : IEventCommand {
  ~MovementPlaySECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_SE_del_Movement; }
  Audio se;
  [[nodiscard]] std::string stringRep() const override {
    return ColorFormatter::getColorCode(DecodeEnumName(code()))
    + std::string(indent ? *indent * 4 : 0, ' ') + "◇SE : " + (se.name == "" ? "None" : se.name) + " "
    + std::format("({}, {}, {})", se.volume, se.pitch, se.pan) + ColorFormatter::popColor();
  }
};

struct MovementScriptCommand : IEventCommand {
  ~MovementScriptCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Script_del_Movement; }
  std::string script;
  [[nodiscard]] std::string stringRep() const override {
    return ColorFormatter::getColorCode(DecodeEnumName(code()))
    + std::string(indent ? *indent * 4 : 0, ' ') + "◇Script : " + script + ColorFormatter::popColor();
  }
};

// END MOVEMENTROUTE
struct CommandParser {
  nlohmann::json parser;
  int index = 0;
  const std::string diamond = "\u25c6";
  const std::string colon = "\uff1a";

  std::vector<std::shared_ptr<IEventCommand>> parse(const nlohmann::json& data);
  void serialize(nlohmann::json& data, const std::vector<std::shared_ptr<IEventCommand>>& list);

  EventCode nextEventCommand() {
    EventCode ret;
    parser[index + 1].at("code").get_to(ret);
    return ret;
  }

  std::string symbol(std::shared_ptr<IEventCommand> data) {
    return static_cast<int>(data->code()) < 400 ? diamond : colon;
  }
 std::string indentText(std::shared_ptr<IEventCommand> data) {
    std::string text = "";
    for(int i = 0; i < data->indent; ++i) {
      text += "  ";
    }
    return text;
  }

  nlohmann::json currentCommand() { return parser[index]; }

  struct symbol;
  struct indentText;
  struct create;
};