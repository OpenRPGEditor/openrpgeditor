#pragma once
#include "Database/Globals.hpp"

#include "nlohmann/json.hpp"

#include <memory>

struct IEventCommand {
  int indent;
  virtual ~IEventCommand() = default;
  [[nodiscard]] virtual EventCode code() const = 0;
};

struct EventDummy : IEventCommand {
  ~EventDummy() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Event_Dummy; }
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
};

struct ShowChoiceCommand : IEventCommand {
  ~ShowChoiceCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Text; }
  TextBackground background;
  ChoiceWindowPosition positionType;
  std::vector<std::string> choices;
  int cancelType;
  int defaultType;
};

struct WhenSelectedCommand : IEventCommand {
  ~WhenSelectedCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Selected; }

  int param1;
  std::string choice;
};

struct WhenCancelCommand : IEventCommand {
  ~WhenCancelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Cancel; }
};

struct InputNumberCommand : IEventCommand {
  ~InputNumberCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Input_Number; }
};

struct SelectItemCommand : IEventCommand {
  ~SelectItemCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Select_Item; }
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
};

struct NextCommentCommand : IEventCommand {
  ~NextCommentCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Comment; }

  std::string text;
};
struct CommentCommand : IEventCommand {
  ~CommentCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Comment; }
  std::vector<std::shared_ptr<NextCommentCommand>> text;
};

struct ConditionalBranchCommand : IEventCommand {
  ~ConditionalBranchCommand() override {}
  [[nodiscard]] EventCode code() const override { return EventCode::Comment; }
  ConditionType type;
  union {
    struct {
      int switchIdx;
      int checkIfOn;
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
      int id; // A, B, C, D
      bool checkIfOn;
    } selfSwitch;
    struct {
      TimerComparisonType comparison;
      int sec;
    } timer;
    struct {
      int id;
      ActorConditionType type;
      union {
        std::string name;
        int checkId;
      };
    } actor;
    struct {
      int id;
      EnemyConditionType type;
      int stateId; // For "State" condition
    } enemy;
    struct {
      int id;
      FacingDirection facing;
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
    Button button;
    std::string script;
  };
};

struct ElseCommand : IEventCommand {
  ~ElseCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Else; }
};

struct LoopCommand : IEventCommand {
  ~LoopCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Loop; }
};

struct RepeatAboveCommand : IEventCommand {
  ~RepeatAboveCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Repeat_Above; }
};

struct BreakLoopCommand : IEventCommand {
  ~BreakLoopCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Break_Loop; }
};

struct ExitEventProecessingCommand : IEventCommand {
  ~ExitEventProecessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Exit_Event_Processing; }
};

struct CommonEventCommand : IEventCommand {
  ~CommonEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Common_Event; }
  int event;
};

struct LabelCommand : IEventCommand {
  ~LabelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Label; }
  std::string label;
};

struct JumpToLabelCommand : IEventCommand {
  ~JumpToLabelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Jump_To_Label; }
  std::string label;
};

struct ControlSwitches : IEventCommand {
  ~ControlSwitches() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Switches; }
  int start;
  int end;
  SwitchControl turnOff;
};

struct ControlVariables : IEventCommand {
  ~ControlVariables() override {};
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Variables; }
  int start;
  int end;
  VariableControlOperation operation;
  VariableControlOperand operand;
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
    std::string script;
  };
};

struct ControlSelfSwitch : IEventCommand {
  ~ControlSelfSwitch() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Self_Switch; }

  int selfSw; // A, B, C, D
  SwitchControl turnOff;
};

struct ControlTimer : IEventCommand {
  ~ControlTimer() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Self_Switch; }
  TimerControl control;
  int seconds;
};

struct PlaySECommand : IEventCommand {
  ~PlaySECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_SE; }
};

struct EndCommand : IEventCommand {
  ~EndCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End; }
};

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
};